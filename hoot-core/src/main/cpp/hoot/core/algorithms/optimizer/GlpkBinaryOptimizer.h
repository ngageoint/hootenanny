#ifndef GLPKBINARYOPTIMIZER_H
#define GLPKBINARYOPTIMIZER_H

#include "BinaryOptimizer.h"
#include "IntegerProgrammingSolver.h"

namespace hoot
{

class GlpkBinaryOptimizer : public BinaryOptimizer
{
public:
  static const double EPSILON;

  GlpkBinaryOptimizer();

protected:

  void _populateSolver(IntegerProgrammingSolver& solver, const vector<double>& weights,
    const vector< pair<size_t, size_t> >& constraints);

  virtual double _solve(const vector<double>& weights,
    const vector< pair<size_t, size_t> >& constraints, vector<size_t>& solution);


};

}

#endif // GLPKBINARYOPTIMIZER_H
