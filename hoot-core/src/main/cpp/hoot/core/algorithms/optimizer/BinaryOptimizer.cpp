#include "BinaryOptimizer.h"

namespace hoot
{

BinaryOptimizer::BinaryOptimizer() :
  _timeLimit(-1)
{
}

void BinaryOptimizer::addColumn(size_t c, double weight)
{
  if (_weights.size() <= c)
  {
    _weights.resize(c + 1, 0);
  }
  _weights[c] = weight;
}

double BinaryOptimizer::solve(vector<size_t>& solution)
{
  return _solve(_weights, _constraints, solution);
}

}
