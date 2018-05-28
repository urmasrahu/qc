#include <gtest\gtest.h>
#include <iostream>

#include "quantum_gates.h"
#include "print_util.h"

using namespace testing;

class QuantumGates_Test : public Test
{
public:
  QuantumGates_Test() = default;
};


TEST_F(QuantumGates_Test, SquareRootOfNot)
{
  cdouble_matrix gate = QuantumGates::SquareRootOfNot();
  PrintUtil::PrintMatrixToConsole(gate, "gate: sqrt(not)");

  cdouble_vector v;
  v.FromStringList({ "2-3i", "1+2i" });
  v = v.Normalize();

  cdouble_matrix m;
  m.FromVector(v);

  cdouble_matrix result = gate * m.Conjugate();

  PrintUtil::PrintMatrixToConsole(result, "reversed");
  std::cout << "reversed norm (check): " << result.Norm().Real() << "\n";
}