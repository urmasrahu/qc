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
  PrintUtil::PrintMatrixToConsole(MatrixConstants::HADAMARD, "HADAMARD");

  cdouble_matrix H2_pwr2 = MatrixConstants::HADAMARD.TensorProduct(MatrixConstants::HADAMARD);
  PrintUtil::PrintMatrixToConsole(H2_pwr2, "HADAMARD at tensor power 2");

  cdouble_matrix H2_pwr3 = H2_pwr2.TensorProduct(MatrixConstants::HADAMARD);
  //PrintUtil::PrintMatrixToConsole(H2_pwr3, "HADAMARD at tensor power 3");

  cdouble_matrix H2_pwr3_reducedByScalar = H2_pwr3 * cdouble(2 * M_SQRT2);
  //PrintUtil::PrintMatrixToConsole(H2_pwr3_reducedByScalar, "HADAMARD at tensor power 3 reduced by 0.5 * 1/sqrt(2)"); // this does not look right in output, so tensor product must be wrong
}
