#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

// Qt
#include <QSet>

// Tgs
#include <tgs/Statistics/Random.h>

#include "FitnessFunction.h"
#include "State.h"

namespace Tgs
{

class SimulatedAnnealing
{
public:
  SimulatedAnnealing(ConstStateDescriptionPtr sd, ConstFitnessFunctionPtr fitness);

  QSet<ConstStatePtr> getBestStates() const { return _bestStates; }

  double iterate(int kmax);

  /**
   * If set to true then the next neighbor will always be generated from a state in the set of
   * best states found so far.
   */
  void setPickFromBestScores(bool best) { _pickFromBestScores = best; }
private:

  mutable Random _rand;
  double _bestScore;
  QSet<ConstStatePtr> _bestStates;
  ConstFitnessFunctionPtr _fitness;
  bool _pickFromBestScores;
  ConstStateDescriptionPtr _stateDescription;

  StatePtr _generateNeighbor(ConstStatePtr from, double T) const;
  StatePtr _generateRandomState() const;

  double _P(double e, double eNew, double T) const;

};

}

#endif // SIMULATEDANNEALING_H
