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
#include <list>

using namespace std;

// Class encapsulating the functionality of a single router object in a graph
// of Router objects which is contained in a Network Object. Each Router object
// basically contains information about it's own ID, a list of references to 
// adjacent Router objects in the Graph along with the shortest paths and their
// costs to every other Router object. It also contains the broadcast cost of
// respective Router object.
// Example:
//    Router *router = new Network(3, 4);
// This example creates a Router object with Router ID 3 (1 indexed) to be used
// in a Network Object having a graph of 4 Router Objects.
class Router {

public:
  // Class constructor which simply updates object properties with input
  // arguments and allocates required memory.
  // Arguments:
  //    router_id_arg     - ID (1 indexed) of the Router object.
  //    num_routers_arg   - Total number of routers in the network.
  Router(int router_id_arg, int num_network_routers_arg);

  // ID (1 indexed) of the Router object.
  int router_id;
  
  // Total number of routers in the network of which this router is a part.
  int num_routers_in_network;

  // Sum of shortest path costs from this router to all other routers in the 
  // Network graph of Router objects.
  int broadcast_cost;

  // Vector containing pointers to other Router objects in the Network Graph.
  std::vector<Router *> connections;

  // Costs of links incoming to the Router object from adjacent Router objects
  // in the Network graph.
  std::vector<int> in_costs;

  // Costs of links outgoing to the adjacent Router objects in the Network
  // graph.
  std::vector<int> out_costs;

  // Vector of shortest path costs to all other Router objects in the Network
  // graph.
  std::vector<int> all_path_costs;

  // Vector of Lists where each list of Router IDs represents the shortest path
  // to the target Router. The target Router's ID is the Index of the list in
  // the Vector plus 1 (as Router IDs are 1 indexed).
  // Each list is organized in a source ---> destination fashion; that is,
  // the start() iterator will return the source Router ID (which is this one)
  // and the end() iterator returns the destination Router ID
  // (where the path terminates)
  std::vector<std::list<int>> shortest_paths;

  // Connection table of the given Router. This maps every router in the
  // Network to the Router ID of one of the adjacent Routers of this Router
  // in the Network graph which every packet must be immediately redirected to
  // ensure a minimum transmission cost. The keys are target Router ID - 1
  // and the values are the Router IDs of the immediately next Router where the
  // packet is to redirected.
  std::vector<int> connection_table; //Maps destination to next Router ids
};

