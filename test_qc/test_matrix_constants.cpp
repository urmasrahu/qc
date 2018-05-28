#include <gtest\gtest.h>
#include <iostream>

#include "matrix_constants.h"
#include "print_util.h"

using namespace testing;

class MatrixConstants_Test : public Test
{
public:
};


TEST_F(MatrixConstants_Test, HADAMARD_2)
{
  PrintUtil::PrintMatrixToConsole(MatrixConstants::HADAMARD_2, "HADAMARD_2");

  PrintUtil::PrintMatrixToConsole(MatrixConstants::HADAMARD_2.TensorProduct(MatrixConstants::HADAMARD_2), "HADAMARD_2 at tensor power 2");
}