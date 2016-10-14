/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
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
