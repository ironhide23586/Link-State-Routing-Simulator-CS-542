// MIT License
// 
// Copyright(c) 2016 Souham Biswas
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Network.h"

// Class constructor
Network::Network() {
  num_routers = 0;
  first_init = true;
}

// Implementation of CreateNetwork method in Network.h;
// This method creates a Network Graph from adjacency matrix as read from input
// file.
// Arguments:
//    link_matrix_arg   - Double pointer to 2D Adjacency Matrix.
//    num_routers_arg   - Total number of routers in the network.
void Network::CreateNetwork(int **link_matrix_arg, int num_routers_arg) {
  num_routers = num_routers_arg;
  link_matrix = link_matrix_arg;
  routers.clear();
  for (int i = 0; i < num_routers; i++) {
    routers.push_back(*(new Router(i + 1, num_routers_arg)));
  }
  for (int src_router = 0; src_router < num_routers; src_router++) {
    for (int dest_router = 0; dest_router < num_routers; dest_router++) {
      if (dest_router != src_router 
          && link_matrix[src_router][dest_router] > 0) {
        routers[src_router].connections.push_back(
          &routers[dest_router]);
        routers[src_router].out_costs.push_back(
          link_matrix[src_router][dest_router]);
        routers[src_router].in_costs.push_back(
          link_matrix[dest_router][src_router]);
      }
    }
  }
  if (first_init) {
    first_init = false;
    router_down.resize(num_routers, false);
  }
}

// Implementation of PopulateRouterConnection method in Network.h;
// Populates the connection table of the router specified in argument.
// Other things also populated include the list of shortest paths and their
// costs to every other router in the network.
// Arguments:
//    src_router_id     - Source router whose connection table is to be built
//                        and from where the shortest paths to all other
//                        routers in the network are to be calculated.
// Returns:
//    A boolean value which is true if the method executes successfully.
//    Typically returns false when source node is isolated.
bool Network::PopulateRouterConnectionTable(int src_router_id) {
  if (router_down[src_router_id - 1])
    return false;
  bool ret_val = DijkstrasAlgorithm(src_router_id);
  int num_path_nodes, src_router_index = src_router_id - 1;
  std::list<int>::iterator path_iter;
  for (int dest_router_index = 0;
       dest_router_index < num_routers; dest_router_index++) {
    path_iter = routers[src_router_index].shortest_paths[dest_router_index]
      .begin();
    num_path_nodes = routers[src_router_index]
      .shortest_paths[dest_router_index].size();
    if (num_path_nodes < 1)
      continue;
    if (num_path_nodes == 1)
      routers[src_router_index].connection_table[dest_router_index]
      = *path_iter;
    else {
      routers[src_router_index].connection_table[dest_router_index]
        = *(std::next(path_iter, 1));
    }
  }
  for (int i = 0; i < num_routers; i++) {
    if (router_down[i])
      continue;
    routers[src_router_index].broadcast_cost
      += routers[src_router_index].all_path_costs[i];
  }
  routers[src_router_index].broadcast_cost++;
  return ret_val;
}

// Implementation of RemoveRouter method in Network.h;
// Simulates a router which is malfunctioning in the network. This method
// in effect, modifies the original adjacency matrix by replacing all the
// connections to the input router with negative weight edges which symbolize
// no edges and rebuilds the graph of router objects.
// Arguments:
//    down_router_id    - Router ID (1 indexed) of defective router.
void Network::RemoveRouter(int down_router_id) {
  int down_router_index = down_router_id - 1;
  for (int i = 0; i < num_routers; i++) {
    link_matrix[down_router_index][i] = -1;
    link_matrix[i][down_router_index] = -1;
  }
  CreateNetwork(link_matrix, num_routers);
  router_down[down_router_id - 1] = true;
}

// Implementation of FindBroadcastRouter method in Network.h;
// Updates the broadcast_router_id property of the network object. This
// method finds the router most suitable for a broadcast operation in the
// network; it basically finds the router in the network which has a minimum
// sum of costs of paths to every router in the network.
// Returns:
//    A boolean value which is true if the method executes successfully.
//    Typically returns false when network graph is disjoint and when 
//    clusters exits in the network.
bool Network::FindBroadcastRouter() {
  int min_broadcast_cost = std::numeric_limits<int>::max();
  bool broadcast_router_found = false;
  for (int router_index = 0; router_index < num_routers; router_index++) {
    if (routers[router_index].broadcast_cost < 0)
      PopulateRouterConnectionTable(router_index + 1);
    if (min_broadcast_cost > routers[router_index].broadcast_cost
        && routers[router_index].broadcast_cost >= 0) {
      min_broadcast_cost = routers[router_index].broadcast_cost;
      broadcast_router_id = router_index + 1;
      broadcast_router_found = true;
    }
  }
  if (!broadcast_router_found)
    broadcast_router_id = -1;
  return broadcast_router_found;
}

// Implementation of DijkstrasAlgorithm private method in Network.h;
// Contains implementation of the Dijkstra's algorithm to find shortest paths
// to all nodes from a given source node node in a graph. It takes the Router
// ID of the source router as an input and updates the corresponding Router
// object's following properties -
// connection_table, shortest_paths, all_path_costs
// Arguments:
//    src_router_id     - Source router whose connection table is to be built
//                        and from where the shortest paths to all other
//                        routers in the network are to be calculated.
// Returns:
//    A boolean value which is true if the method executes successfully.
//    Typically returns false when source node is isolated.
bool Network::DijkstrasAlgorithm(int src_router_id) {
  int src_router_index = src_router_id - 1, num_visited = 1;
  int curr_router_index = src_router_index, next_router_index;
  int min_next_router_cost;
  bool ret_val = false;
  bool isolated_node;
  std::vector<bool> visited(num_routers, false);
  routers[src_router_index].all_path_costs[src_router_index] = 0;
  visited[src_router_index] = true;
  routers[src_router_index].shortest_paths[src_router_index].clear();
  routers[src_router_index].shortest_paths[src_router_index].push_back(
    src_router_id);
  while (num_visited <= num_routers) {
    UpdateCosts(curr_router_index, routers[src_router_index].all_path_costs,
                visited, routers[src_router_index].shortest_paths);
    min_next_router_cost = std::numeric_limits<int>::max();
    isolated_node = true;
    for (int i = 0; i < num_routers; i++) {
      if (!visited[i] &&
          routers[src_router_index].all_path_costs[i] < min_next_router_cost) {
        min_next_router_cost = routers[src_router_index].all_path_costs[i];
        next_router_index = i;
        isolated_node = false;
      }
    }
    if (!isolated_node) {
      visited[next_router_index] = true;
      curr_router_index = next_router_index;
      ret_val = true;
    }
    num_visited++;
  }
  return ret_val;
}

// Implementation of UpdateCosts private method in Network.h;
// Used in the implementation of Dijkstra's algorithm. This method updates
// the path costs to the routers immediately adjacent to the input router.
// Arguments:
//    curr_router_index - Index of router from which the path cost is to be
//                        measured.
//    costs             - Costs to routers encountered so far from the source
//                        router.
//    visited           - Boolean array keeping track of visited nodes which
//                        shall not be probed.
//    shortest paths    - This keeps track of all the shortest paths. This
//                        is a vector of lists. Lists are more efficient at
//                        cell insertion/deletion than vectors and hence were
//                        chosen to store the paths over vectors which are
//                        good for random lookup.
void Network::UpdateCosts(int curr_router_index, std::vector<int> &costs,
                          std::vector<bool> &visited,
                          std::vector<std::list<int>> &shortest_paths) {
  int linked_router_index, linked_router_cost;
  for (int link = 0; link < routers[curr_router_index].connections.size();
       link++) {
    linked_router_index 
      = routers[curr_router_index].connections[link]->router_id - 1;
    linked_router_cost = costs[curr_router_index] 
      + routers[curr_router_index].out_costs[link];
    if (visited[linked_router_index]
        || costs[linked_router_index] < linked_router_cost)
        continue;
    costs[linked_router_index] = linked_router_cost;
    shortest_paths[linked_router_index].clear();
    shortest_paths[linked_router_index].insert(
      shortest_paths[linked_router_index].end(),
      shortest_paths[curr_router_index].begin(),
      shortest_paths[curr_router_index].end()); //src -> ---- -> dest
    shortest_paths[linked_router_index].push_back(linked_router_index + 1);
  }
}