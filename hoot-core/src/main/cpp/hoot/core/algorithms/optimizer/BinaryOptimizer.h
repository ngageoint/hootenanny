#ifndef BINARYOPTIMIZER_H
#define BINARYOPTIMIZER_H

// Standard
#include <vector>

namespace hoot
{

using namespace std;

/**
 * A simple abstract base class for wrapping binary optimizers.
 */
class BinaryOptimizer
{
public:
  BinaryOptimizer();

  /**
   * Reserve this many columns. This helps make the process more efficient, but isn't necessary.
   */
  void reserveColumns(size_t reserveSize) { _weights.reserve(reserveSize); }

  /**
   * Adds a new column to the problem. It is assumed that the column numbers will be contiguous. If
   * you use large non-contiguous values you may have a lot of memory bloat.
   */
  void addColumn(size_t c, double weight);

  /**
   * At most one of c1 or c2 can be included in the final solution.
   */
  void addConstraint(size_t c1, size_t c2) { _constraints.push_back(pair<size_t, size_t>(c1, c2)); }

  double getTimeLimit() const { return _timeLimit; }

  /**
   * Set a suggested time limit in seconds. This may be approximately honored or completely ignored.
   */
  void setTimeLimit(double t) { _timeLimit = t; }

  /**
   * Solve the problem and return the score of the best solution.
   */
  double solve(vector<size_t>& solution);

protected:
  virtual double _solve(const vector<double>& weights,
    const vector< pair<size_t, size_t> >& constraints, vector<size_t>& solution) = 0;

private:
  double _timeLimit;
  vector<double> _weights;
  vector< pair<size_t, size_t> > _constraints;
};

}

#endif // BINARYOPTIMIZER_H
