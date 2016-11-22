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

// CS 542 - Computer Networks I: Fundamentals final project, Fall 2016
// Link State Routing Simulator
// Author: Souham Biswas
// CWID: A20365242
// Email: sbiswas7@hawk.iit.edu

#include <iostream>

#include "FileProcessor.h"
#include "Network.h"

using namespace std;

// Prints the connection table stored in the input Router Object to standard
// out in a human readable, tabular format.
// Arguments:
//    router            - Router object whose connection table is to be
//                        printed.
void print_connection_table(Router&);

// Implementation of print_path method.
// Prints a path (Composed of Router IDs which are 1 indexed) contained 
// in a list object to the standard out.
// This is used to print the shortest path between two Router objects in the
// Network graph in a human readable format.
// Arguments:
//    path_iter         - Iterator pointing to the beginning of the list
//                        containing the path. This points to the source
//                        Router ID.
//    end_iter          - Iterator pointing to the end of the list containing
//                        the path. This points to the destination Router ID.
void print_path(std::list<int>::iterator,
                std::list<int>::iterator);

int main() {
  // This is used to read the simulation command issued by the user.
  char command;

  // Stores the name of the input text file name containing the adjacency
  // matrix representing the Network graph as entered by the user on standard
  // input.
  char input_file[256];

  // This object is used to parse and read input text files containing the
  // adjacency matrix representing the network.
  FileProcessor *file_processor = new FileProcessor();

  // This network object shall contain the graph of Router objects which will
  // be used to perform the simulation.
  Network *network = new Network();

  // These variables are used to take input from user from standard input.
  int source_router_id = -1, dest_router_id = -1, down_router_id = -1;

  // Set to true once a valid input file is loaded. It is used to prevent
  // crashes if simulation commands are issued before loading a file.
  bool file_loaded = false;

  std::cout << "Developer: Souham Biswas" << std::endl;
  std::cout << "CWID: A20365242" << std::endl;
  std::cout << "Email: sbiswas7@hawk.iit.edu" << std::endl;

  // This infinite loop executes every time a command is issued and exits only
  // upon receiving command '6' which exits the program.
  while (1) {
    std::cout << "\n------------------------------------" << std::endl;
    std::cout << "CS542 Link State Routing Simulator\n" << std::endl;
    std::cout << "(1) Create a Network Topology" << std::endl;
    std::cout << "(2) Build a Connection Table" << std::endl;
    std::cout << "(3) Shortest Path to Destination Router" << std::endl;
    std::cout << "(4) Modify a Topology" << std::endl;
    std::cout << "(5) Best Router for Broadcast" << std::endl;
    std::cout << "(6) Exit" << std::endl;
    std::cout << "\nCommand: ";
    std::cin >> command;
    std::cout << std::endl;

    switch (command) {

      // Command: "Create a Network Topology"
      // This reads the an input text file containing an adjacency matrix of a
      // Network and creates a Network graph to be used for simulation.
      case '1': {
        std::cout << "Input original network topology matrix data file: ";
        std::cin >> input_file;
        file_processor->LoadFile(input_file);
        file_processor->GetLinkMatrix();
        std::cout << "Review original topology matrix:" << std::endl;
        file_processor->PrintLinkMatrix();
        network->CreateNetwork(file_processor->link_matrix,
                               file_processor->num_routers);
        file_loaded = true;
        continue;
      }

      // Command: "Build a Connection Table"
      // This takes a router ID as input from the user through standard input
      // and prints out the connection table pertaining to that router in
      // the Network graph.
      case '2': {
        if (!file_loaded) {
          std::cout << "Please load a file before performing simulation!"
            << std::endl;
          std::cout << "(Command 1 not executed)" << std::endl;
          continue;
        }
        std::cout << "Select a source router : ";
        std::cin >> source_router_id;
        network->PopulateRouterConnectionTable(source_router_id);
        print_connection_table(network->routers[source_router_id - 1]);
        continue;
      }

      // Command: "Shortest Path to Destination Router"
      // Computes the shortest path from source to destination router IDs as
      // as input by the user through standard input. Prints out the path so
      // obtained along with it's cost.
      case '3': {
        if (!file_loaded) {
          std::cout << "Please load a file before performing simulation!"
            << std::endl;
          std::cout << "(Command 1 not executed)" << std::endl;
          continue;
        }
        bool path_exists; //Set to true when no path exists
        if (source_router_id == -1) {
          std::cout << "No source router specified" << std::endl;
          std::cout << "Source Router : ";
          std::cin >> source_router_id;
        }
        path_exists
          = network->PopulateRouterConnectionTable(source_router_id);
        std::cout << "Select a destination router : ";
        std::cin >> dest_router_id;
        std::cout << "Shortest Path from router " << source_router_id
          << " to router " << dest_router_id << " is -" << std::endl;
        if (!path_exists) {
          std::cout << "No path exists from source router "
            << source_router_id << " to destination router "
            << dest_router_id << " in the network" << std::endl;
        }
        else {
          print_path(network
                     ->routers[source_router_id - 1]
                     .shortest_paths[dest_router_id - 1].begin(),
                     network
                     ->routers[source_router_id - 1]
                     .shortest_paths[dest_router_id - 1].end());
          std::cout << "Cost = "
            << network->routers[source_router_id - 1]
            .all_path_costs[dest_router_id - 1];
        }
        continue;
      }

      // Command: "Modify a Topology"
      // This simulates the malfunction of a given router whose router ID is
      // obtained from the user through standard input. The malfunctioning
      // router is then removed from the graph.
      case '4': {
        if (!file_loaded) {
          std::cout << "Please load a file before performing simulation!"
            << std::endl;
          std::cout << "(Command 1 not executed)" << std::endl;
          continue;
        }
        std::cout << "Select Router to be removed : ";
        std::cin >> down_router_id;
        network->RemoveRouter(down_router_id);
        if (source_router_id == -1) {
          std::cout << "No source router specified" << std::endl;
          std::cout << "Source Router : ";
          std::cin >> source_router_id;
        }
        if (dest_router_id == -1) {
          std::cout << "No destination router specified" << std::endl;
          std::cout << "Destination Router : ";
          std::cin >> dest_router_id;
        }
        bool path_exists
          = network->PopulateRouterConnectionTable(source_router_id);
        if (path_exists) {
          print_connection_table(network->routers[source_router_id - 1]);
          std::cout << "\nShortest Path from router " << source_router_id
            << " to router " << dest_router_id << " is -" << std::endl;
          print_path(network
                     ->routers[source_router_id - 1]
                     .shortest_paths[dest_router_id - 1].begin(),
                     network
                     ->routers[source_router_id - 1]
                     .shortest_paths[dest_router_id - 1].end());
          std::cout << "Cost = "
            << network->routers[source_router_id - 1]
            .all_path_costs[dest_router_id - 1];
        }
        else
          std::cout << "No path exists from source router "
                    << source_router_id << " to destination router "
                    << dest_router_id << " in the network" << std::endl;
        continue;
      }

      // Command: "Best Router for Broadcast"
      // The router ID of the router with the least sum of shortest path costs
      // to all other routers in the network is printed along with the bradcast
      // cost which is the sum of the shortest path costs to all other routers
      // in the network.
      case '5': {
        if (!file_loaded) {
          std::cout << "Please load a file before performing simulation!"
            << std::endl;
          std::cout << "(Command 1 not executed)" << std::endl;
          continue;
        }
        bool broadcast_router_found = network->FindBroadcastRouter();
        if (!broadcast_router_found) {
          std::cout << "Network Graph is disjoint ";
          std::cout << "(Multiple Network clusters exist)" << std::endl;
          std::cout << "No global broadcast router exists" << std::endl;
          continue;
        }
        int broadcast_router_id = network->broadcast_router_id;
        std::cout << "Broadcast Router is Router "
          << broadcast_router_id << std::endl;
        std::cout
          << "Broadcast Cost (sum of path costs to all other routers) = "
          << network->routers[broadcast_router_id - 1].broadcast_cost 
          << std::endl;
        print_connection_table(network->routers[broadcast_router_id - 1]);
        continue;
      }

      // Command: "Exit"
      // Breaks the infinite loop and exits the program
      case '6': {
        std::cout << "Exit CS542-04 2016 Fall Project. Good Bye!";
        return 0;
      }

      // Executed when an invalid simulation command is entered.
      default: {
        std::cout << "Please enter a valid response :|" << std::endl;
        continue;
      }
    }
  }
}

// Implementation of print_connection_table method.
// Prints the connection table stored in the input Router Object to standard
// out in a human readable, tabular format.
// Arguments:
//    router            - Router object whose connection table is to be
//                        printed.
void print_connection_table(Router &router) {
  std::cout << "Router " << router.router_id << " Connection Table"
    << std::endl;
  std::cout << "Destination\tInterface" << std::endl;
  std::cout << "========================" << std::endl;
  int src_router_index = router.router_id - 1;
  for (int dest_router_index = 0;
       dest_router_index < router.num_routers_in_network;
       dest_router_index++) {
    std::cout << dest_router_index + 1 << "\t\t";
    if (dest_router_index == src_router_index
        || router.connection_table[dest_router_index] < 1)
        std::cout << "-";
    else
      std::cout << router.connection_table[dest_router_index];
    std::cout << std::endl;
  }
}

// Implementation of print_path method.
// Prints a path (Composed of Router IDs which are 1 indexed) contained 
// in a list object to the standard out.
// This is used to print the shortest path between two Router objects in the
// Network graph in a human readable format.
// Arguments:
//    path_iter         - Iterator pointing to the beginning of the list
//                        containing the path. This points to the source
//                        Router ID.
//    end_iter          - Iterator pointing to the end of the list containing
//                        the path. This points to the destination Router ID.
void print_path(std::list<int>::iterator path_iter,
                std::list<int>::iterator end_iter) {
  bool first_node = true;
  for (; path_iter != end_iter; path_iter++) {
    if (!first_node)
      std::cout << " -> ";
    std::cout << *path_iter;
    first_node = false;
  }
  std::cout << std::endl;
}