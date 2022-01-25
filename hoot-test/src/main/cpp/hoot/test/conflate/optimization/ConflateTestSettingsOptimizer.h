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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef CONFLATETESTSETTINGSOPTIMIZER_H
#define CONFLATETESTSETTINGSOPTIMIZER_H

// Tgs
#include <tgs/Optimization/State.h>
#include <tgs/Optimization/StateDescription.h>

namespace hoot
{

class AbstractTestFitnessFunction;

/**
 * @brief the ConflateTestSettingsOptimizer class uses simulated annealing to attempt to determine
 * the optimal configuration for conflation related configuration options, given a fitness function
 * that describes how well a conflation test performs.
 */
class ConflateTestSettingsOptimizer
{
public:

  ConflateTestSettingsOptimizer(bool verbose);

  /**
   * @brief runOptimization Runs the optimization process
   * @param fitnessFunction a user specified fitness function that determines the value of test
   * outcome
   * @param numIterations the number of iterations to run the optimization
   * @param testSettingsFile path to a file containing configuration options with min/max value
   * ranges
   * @param outputFile file path which to write the optimization summary to
   */
  void runOptimization(std::shared_ptr<AbstractTestFitnessFunction> fitnessFunction, const int numIterations,
                       const QString& testSettingsFile, const QString& outputFile);

private:

  bool _verbose;

  /*
   * Configure hardcoded option value ranges here per test (could move this to a file)
   */
  Tgs::StateDescriptionPtr _initStateDescription(const QString& testSettingsFile);

  /*
   * Writes a summary of the optimization results to file
   */
  void _writeOutput(const std::shared_ptr<AbstractTestFitnessFunction>& fitnessFunction,
                    const QSet<Tgs::ConstStatePtr>& bestStates, const int numIterations,
                    const QString& outputFile);
};

}

#endif // CONFLATETESTSETTINGSOPTIMIZER_H
