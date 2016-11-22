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

#pragma once

#include <vector>
#include <limits>

#include "Router.h"

using namespace std;

// Class encapsulating the network architecture of the Routers for simulating
// Link State Routing Protocol. A Network object actually creates a graph of
// Router Objects.
// The CreateNetwork method is used to instantiate a Network object which
// accepts an adjacency matrix (as a 2D matrix) and the number of routers as
// arguments.
// Example:
//    Network *network = new Network();
//    network->CreateNetwork(link_matrix, num_routers);
class Network {

public:
  // Class constructor.
  Network();

  // Method to create Network Graph from adjacency matrix as read from input
  // file.
  // Arguments:
  //    link_matrix_arg   - Double pointer to 2D Adjacency Matrix.
  //    num_routers_arg   - Total number of routers in the network.
  void CreateNetwork(int **link_matrix_arg, int num_routers_arg);

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
  bool PopulateRouterConnectionTable(int src_router_id);

  // Simulates a router which is malfunctioning in the network. This method
  // in effect, modifies the original adjacency matrix by replacing all the
  // connections to the input router with negative weight edges which symbolize
  // no edges and rebuilds the graph of router objects.
  // Arguments:
  //    down_router_id    - Router ID (1 indexed) of defective router.
  void RemoveRouter(int down_router_id);

  // Updates the broadcast_router_id property of the network object. This
  // method finds the router most suitable for a broadcast operation in the
  // network; it basically finds the router in the network which has a minimum
  // sum of costs of paths to every router in the network.
  // Returns:
  //    A boolean value which is true if the method executes successfully.
  //    Typically returns false when network graph is disjoint and when 
  //    clusters exits in the network.
  bool FindBroadcastRouter();

  // Double pointer to the adjacency matrix describing the network topology
  // represented in memory as a 2D matrix (Array of pointers to arrays).
  int **link_matrix;
  
  int num_routers;
  int broadcast_router_id;

  // Vector of actual Router objects which are used to create a graph of Router
  // objects simulating a network.
  std::vector<Router> routers;

  // Boolean array used to keep track of defective routers which cannot be used
  // in the network. All values initially set to false.
  std::vector<bool> router_down;

private:
  // Contains implementation of the Dijkstra's algorithm to find shortest paths
  // to all nodes from a given source node in a graph. It takes the Router
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
  bool DijkstrasAlgorithm(int src_router_id);

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
  void UpdateCosts(int curr_router_index, std::vector<int> &costs,
                   std::vector<bool> &visited,
                   std::vector<std::list<int>> &shortest_paths);

  // Is true only when the network is initialized for the first time. This flag
  // is used to initialize the router_down boolean array keeping track of
  // 'defective' routers.
  bool first_init;
};