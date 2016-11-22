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

#include "FileProcessor.h"

FileProcessor::FileProcessor() {
  num_routers = 0;
}

// Implementation of LoadFile method in FileProcessor.h;
// This method takes the filename string as a constant pointer to character
// array.
// Arguments:
//    filename_arg      - String denoting input file name
void FileProcessor::LoadFile(const char *filename_arg) {
  filename = filename_arg;
  network_def_file_stream.open(filename);
  if (network_def_file_stream.is_open()) {
    std::string line;
    while (std::getline(network_def_file_stream, line)) {
      num_routers++;
      file_input.push_back(line);
    }
    network_def_file_stream.close();
  }
}

// Implementation of GetLinkMatrix method in FileProcessor.h;
// Allocates memory to store adjacency matrix and populates it with values
// read from the input text file.
void FileProcessor::GetLinkMatrix() {
  link_matrix = (int **) malloc(num_routers * num_routers * sizeof(int *));
  for (int i = 0; i < num_routers; i++) {
    link_matrix[i] = (int *) malloc(num_routers * sizeof(int));
    std::vector<std::string> row = split_(file_input[i], ' ');
    for (int j = 0; j < num_routers; j++) {
      link_matrix[i][j] = std::stoi(row[j], nullptr, 10);
    }
  }
}

// Implementation of PrintLinkMatrix method in FileProcessor.h;
// Prints the 2D adjacency matrix to standard out.
void FileProcessor::PrintLinkMatrix() {
  for (int i = 0; i < num_routers; i++) {
    for (int j = 0; j < num_routers; j++) {
      std::cout << link_matrix[i][j] << "\t";
    }
    std::cout << std::endl;
  }
}

// Implementation of private worker method of split_ method in FileProcessor.h;
// Worker method implementing method to split string based on delimiter.
// Arguments:
//    s                 - Input string to be split
//    delim             - Character at which the split has to be made in the
//                        input string.
//    elems             - Output string vector which will contain the
//                        individual chunks from the input string after
//                        splitting.
void FileProcessor::split_(const std::string &s, char delim, 
                           std::vector<std::string> &elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

// Implementation of private split_ method in FileProcessor.h;
// Main method implementing method to split string based on delimiter.
// Arguments:
//    s                 - Input string to be split
//    delim             - Character at which the split has to be made in the
//                        input string.
std::vector<std::string> FileProcessor::split_(const std::string &s, 
                                               char delim) {
  std::vector<std::string> elems;
  split_(s, delim, elems);
  return elems;
}