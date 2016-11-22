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

#include "Router.h"

// Class constructor implementation which simply updates object properties
// with input arguments and allocates required memory.
// Arguments:
//    router_id_arg     - ID (1 indexed) of the Router object.
//    num_routers_arg   - Total number of routers in the network.
Router::Router(int router_id_arg,
               int num_network_routers_arg)
    : router_id(router_id_arg),
      num_routers_in_network(num_network_routers_arg) {
  all_path_costs.resize(num_network_routers_arg,
                        std::numeric_limits<int>::max());
  shortest_paths.resize(num_network_routers_arg);
  connection_table.resize(num_network_routers_arg, -1);
  broadcast_cost = -1;
  all_path_costs[router_id_arg - 1] = 0;
}