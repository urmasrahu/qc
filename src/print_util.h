#pragma once

#include <iostream>

#include "cvector.h"
#include "cmatrix.h"

namespace PrintUtil
{
  template <typename T> void PrintMatrixToConsole(const complex_matrix<T> & matrix, const char * title = nullptr)
  {
    if (title)
      std::cout << title << "\n";

    std::vector<std::vector<std::string>> dbg = matrix.ToStringListList();
    for (auto row : dbg)
    {
      for (auto value : row)
      {
        std::cout << value << " ";
      }
      std::cout << "\n";
    }
  }

  template <typename T> void PrintVectorToConsole(const complex_vector<T> & vector, const char * title = nullptr) // TBD: duplicate code with cvectorTest, put elsewhere
  {
    if (title)
      std::cout << title << "\n";

    std::vector<std::string> dbg = vector.ToStringList();
    for (auto value : dbg)
    {
      std::cout << value << " ";
    }
    std::cout << "\n";
  }
}

