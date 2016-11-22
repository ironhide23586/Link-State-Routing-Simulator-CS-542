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
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Class encapsulating functionality to process input text files containing
// details of the adjacency matrix which are parsed to generate the network
// graphs for link state router simulation.
class FileProcessor {

public:
  // Class constructor.
  FileProcessor();

  // This method takes the filename string as a constant pointer to character
  // array.
  // Arguments:
  //    filename_arg      - String denoting input file name
  void LoadFile(const char *filename_arg);

  // Allocates memory to store adjacency matrix and populates it with values
  // read from the input text file.
  void GetLinkMatrix();

  // Prints the 2D adjacency matrix to standard out.
  void PrintLinkMatrix();

  // Holds the string denoting input file name.
  const char *filename;

  // Number of routers in the adjacency matrix; that is, edge length of the
  // adjacency matrix.
  int num_routers;

  // Double pointer to the memory representation of the 2D adjacency matrix.
  int **link_matrix;

  // Vector of strings where each element is a string representation of each
  // row of the adjacency matrix as read from it.
  std::vector<std::string> file_input;

private:
  // Worker method implementing method to split string based on delimiter.
  // Arguments:
  //    s                 - Input string to be split
  //    delim             - Character at which the split has to be made in the
  //                        input string.
  //    elems             - Output string vector which will contain the
  //                        individual chunks from the input string after
  //                        splitting.
  void split_(const std::string &s, char delim,
              std::vector<std::string> &elems);

  // Main method implementing method to split string based on delimiter.
  // Arguments:
  //    s                 - Input string to be split
  //    delim             - Character at which the split has to be made in the
  //                        input string.
  std::vector<std::string> split_(const std::string &s, char delim);

  // Filestream object used to read input file.
  std::ifstream network_def_file_stream;
};