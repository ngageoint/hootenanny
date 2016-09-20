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
#include "SimulatedAnnealing.h"

// Standard
#include <math.h>
#include <iostream>

// Tgs
#include <tgs/TgsException.h>

namespace Tgs
{
using namespace std;

SimulatedAnnealing::SimulatedAnnealing(ConstStateDescriptionPtr sd,
  ConstFitnessFunctionPtr fitness) : _rand(0)
{
  _stateDescription = sd;
  _fitness = fitness;
  _pickFromBestScores = false;
}

StatePtr SimulatedAnnealing::_generateNeighbor(ConstStatePtr from, double T) const
{
  StatePtr result(new State());

  double scale = pow(T, 1.0);

  foreach (ConstVariableDescriptionPtr vd, _stateDescription->getVariables())
  {
    if (vd->getType() == VariableDescription::Real)
    {
      double range = vd->getRange() * scale;
      double v = 2.0 * range * _rand.generateUniform() - range + from->get(vd->getName());
      v = std::max(vd->getMin(), std::min(vd->getMax(), v));
      result->set(vd, v);
    }
    else if (vd->getType() == VariableDescription::Integer)
    {
      int range = std::max(1, (int)(vd->getRange() * scale + 0.5));
      if (range > 1 || T <= _rand.generateUniform())
      {
        int v = _rand.generateInt() % (range * 2 + 1) - range + from->getInt(vd->getName());
        v = std::max(vd->getMinInt(), std::min(vd->getMaxInt(), v));
        result->set(vd, v);
      }
      else
      {
        result->set(vd, from->getInt(vd->getName()));
      }
    }
    else
    {
      throw Tgs::Exception("Unexpected argument type. (expected int or real)");
    }
  }

  return result;
}

StatePtr SimulatedAnnealing::_generateRandomState() const
{
  StatePtr result(new State());

  foreach (ConstVariableDescriptionPtr vd, _stateDescription->getVariables())
  {
    if (vd->getType() == VariableDescription::Real)
    {
      double v = vd->getRange() * _rand.generateUniform() + vd->getMin();
      result->set(vd, v);
    }
    else if (vd->getType() == VariableDescription::Integer)
    {
      int v = _rand.generateInt() % (int)(vd->getRange() + 0.5) + (int)(vd->getMin() + 0.5);
      result->set(vd, v);
    }
  }

  return result;
}

double SimulatedAnnealing::iterate(int kmax)
{
  StatePtr s1 = _generateRandomState();
  s1->setScore(_fitness->f(s1));
  ConstStatePtr s = s1;
  _bestScore = s->getScore();
  _bestStates.insert(s);
  //std::cout << "s: " << s->toString().toUtf8().data() << std::endl;

  for (int k = 0; k < kmax; ++k)
  {
    double T = 1.0 - (double)k / (double)kmax;
    StatePtr sNew = _generateNeighbor(s, T);
//    std::cout << "s: " << s->toString().toUtf8().data() << std::endl;
//    std::cout << "sNew: " << sNew->toString().toUtf8().data() << std::endl;
    sNew->setScore(_fitness->f(sNew));
//    cout << "sNew score: " << sNew->getScore() << endl;

    if (sNew->getScore() < _bestScore)
    {
      _bestScore = sNew->getScore();
      _bestStates.clear();
      _bestStates.insert(sNew);
    }
    else if (sNew->getScore() == _bestScore)
    {
      _bestStates.insert(sNew);
    }
//    cout << "_bestScore: " << _bestScore << endl;

    if (_pickFromBestScores)
    {
      s = _bestStates.toList()[_rand.generateInt() % _bestStates.size()];
    }
    else if (_P(s->getScore(), sNew->getScore(), T) >= _rand.generateUniform())
    {
      s = sNew;
    }
  }

  return _bestScore;
}

double SimulatedAnnealing::_P(double e, double eNew, double T) const
{
  // Taken from https://en.wikipedia.org/wiki/Simulated_annealing#Acceptance_probabilities_2
  // Seems to suffer from the assumption that e/eNew will be in the range 0 to 1.
  return exp(-(eNew - e) / T);
}

}
