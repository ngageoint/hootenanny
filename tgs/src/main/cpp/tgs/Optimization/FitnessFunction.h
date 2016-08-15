#ifndef FITNESSFUNCTION_H
#define FITNESSFUNCTION_H

#include "State.h"

namespace Tgs
{

class FitnessFunction
{
public:
  /**
   * Calculates the fitness of the specified state. Lower values are better.
   *
   * While not required fitness functions that return values from [0, 1] may work better.
   */
  virtual double f(const ConstStatePtr& s) const = 0;
};

typedef shared_ptr<FitnessFunction> FitnessFunctionPtr;
typedef shared_ptr<const FitnessFunction> ConstFitnessFunctionPtr;

}

#endif // FITNESSFUNCTION_H
