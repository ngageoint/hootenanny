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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ConflictsNetworkMatcherSettingsOptimizer.h"

// Hoot
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>
#include "RegressionReleaseTestFitnessFunction.h"

// Qt
#include <QTextStream>

// Tgs
#include <tgs/Optimization/SimulatedAnnealing.h>

namespace hoot
{

ConflictsNetworkMatcherSettingsOptimizer::ConflictsNetworkMatcherSettingsOptimizer()
{
}

Tgs::StateDescriptionPtr ConflictsNetworkMatcherSettingsOptimizer::_initStateDescription()
{
  Tgs::StateDescriptionPtr stateDescription(new Tgs::StateDescription());

  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkConflictsAggressionKey(),
      //Tgs::VariableDescription::Real, 4.4, 4.4)); //original default
      //Tgs::VariableDescription::Real, 8.8, 8.8)); //current default
      //Tgs::VariableDescription::Real, 0.0, 10.0)); //min/max
      Tgs::VariableDescription::Real, 4.0, 10.0)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkConflictsPartialHandicapKey(),
      //Tgs::VariableDescription::Real, 0.2, 0.2)); //original default
      //Tgs::VariableDescription::Real, 0.2, 0.2)); //current default
      //Tgs::VariableDescription::Real, 0.0, 2.0)); //min/max
      Tgs::VariableDescription::Real, 0.1, 0.3)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkConflictsStubHandicapKey(),
      //Tgs::VariableDescription::Real, 0.86, 0.86)); //original default
      //Tgs::VariableDescription::Real, 1.7, 1.7)); //current default
      //Tgs::VariableDescription::Real, 0.0, 2.0)); //min/max
      Tgs::VariableDescription::Real, 0.76, 0.96)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkConflictsWeightInfluenceKey(),
      //Tgs::VariableDescription::Real, 0.0, 0.0)); //original default
      //Tgs::VariableDescription::Real, 0.0, 0.0)); //current default
      //Tgs::VariableDescription::Real, 0.0, 2.0)); //min/max
      Tgs::VariableDescription::Real, 0.0, 0.1)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkConflictsOutboundWeightingKey(),
      //Tgs::VariableDescription::Real, 0.0, 0.0)); //original default
      //Tgs::VariableDescription::Real, 0.25, 0.25)); //current default
      //Tgs::VariableDescription::Real, 0.0, 2.0)); //min/max
      Tgs::VariableDescription::Real, 0.0, 0.1)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkConflictsStubThroughWeightingKey(),
      //Tgs::VariableDescription::Real, 0.59, 0.59)); //original default
      //Tgs::VariableDescription::Real, 0.5, 0.5)); //current default
      //Tgs::VariableDescription::Real, 0.0, 10.0)); //min/max
      Tgs::VariableDescription::Real, 0.49, 0.69)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkMaxStubLengthKey(),
      //Tgs::VariableDescription::Real, 20.0, 20.0)); //original default
      //Tgs::VariableDescription::Real, 20.0, 20.0)); //current default
      //Tgs::VariableDescription::Real, 1.0, 100.0));  //min/max??
      Tgs::VariableDescription::Real, 15.0, 25.0)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkMatchThresholdKey(),
      //Tgs::VariableDescription::Real, 0.15, 0.15)); //original default
      //Tgs::VariableDescription::Real, 0.15, 0.15)); //current default
      //Tgs::VariableDescription::Real, 0.01, 0.99));  //min/max
      Tgs::VariableDescription::Real, 0.05, 0.25)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkMissThresholdKey(),
      //Tgs::VariableDescription::Real, 0.85, 0.85)); //original default
      //Tgs::VariableDescription::Real, 0.85, 0.85)); //current default
      //Tgs::VariableDescription::Real, 0.01, 0.99));  //min/max
      Tgs::VariableDescription::Real, 0.75, 0.95)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkReviewThresholdKey(),
      //Tgs::VariableDescription::Real, 0.5, 0.5)); //original default
      //Tgs::VariableDescription::Real, 0.5, 0.5)); //current default
      //Tgs::VariableDescription::Real, 0.01, 0.99));  //min/max
      Tgs::VariableDescription::Real, 0.4, 0.6)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getWayMergerMinSplitSizeKey(),
      //Tgs::VariableDescription::Real, 5.0, 5.0)); //original default
      //Tgs::VariableDescription::Real, 5.0, 5.0)); //current default
      //Tgs::VariableDescription::Real, 0.01, 100.0));  //min/max??
      Tgs::VariableDescription::Real, 4.0, 6.0)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getWayMatcherMaxAngleKey(),
      //Tgs::VariableDescription::Real, 60.0, 60.0)); //original default
      //Tgs::VariableDescription::Real, 60.0, 60.0)); //current default
      //Tgs::VariableDescription::Real, 0.01, 90.0));  //min/max
      Tgs::VariableDescription::Real, 50.0, 70.0)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getWayMatcherHeadingDeltaKey(),
      //Tgs::VariableDescription::Real, 5.0, 5.0)); //original default
      //Tgs::VariableDescription::Real, 5.0, 5.0)); //new default
      //Tgs::VariableDescription::Real, 0.01, 100.0));  //min/max??
      Tgs::VariableDescription::Real, 4.0, 6.0)); //test values
  stateDescription->addVariable(
    new Tgs::VariableDescription(ConfigOptions::getNetworkOptimizationIterationsKey(),
      //Tgs::VariableDescription::Real, 10, 10)); //original default
      Tgs::VariableDescription::Real, 10, 10)); //new default
      //Tgs::VariableDescription::Real, 0, 100));  //min/max??
      //Tgs::VariableDescription::Real, 10, 20)); //test values

  return stateDescription;
}

void ConflictsNetworkMatcherSettingsOptimizer::_writeOutput(
  boost::shared_ptr<AbstractTestFitnessFunction> fitnessFunction,
  const QSet<Tgs::ConstStatePtr>& bestStates, const int numIterations)
{
  QString output =
    "Results for Conflicts Network Matcher Configuration Option Optimization with Simulated Annealing\n\n";

  QString temp = "Number of test iterations: " + QString::number(numIterations);
  LOG_ERROR(temp);
  output += temp + "\n\n";

  temp = "Number of tests in test suite: " + QString::number(fitnessFunction->getTestCount());
  LOG_ERROR(temp);
  output += temp + "\n\n";

  temp =
    "Lowest number of test failures in test iteration: " +
    QString::number(fitnessFunction->getLowestNumFailingTestsPerRun());
  LOG_ERROR(temp);
  output += temp + "\n\n";

  temp = "Number of best states found: " + QString::number(bestStates.size());
  LOG_ERROR(temp);
  output += temp + "\n\n";

  boost::shared_ptr<RegressionReleaseTestFitnessFunction> regressionReleaseTestFitnessFunction =
    boost::dynamic_pointer_cast<RegressionReleaseTestFitnessFunction>(fitnessFunction);
  if (regressionReleaseTestFitnessFunction)
  {
    temp = regressionReleaseTestFitnessFunction->highestOverallScoresToString();
    LOG_ERROR(temp);
    output += temp + "\n\n";
  }

  if (fitnessFunction->getLowestNumFailingTestsPerRun() == 0)
  {
    temp = "***YOU FOUND A SOLUTION! :-)***";
  }
  else
  {
    temp = "No solution was found. :-(";
  }
  LOG_ERROR(temp);
  output += temp + "\n\n";

  if (!QDir().mkpath("test-output/algorithms"))
  {
    throw HootException("Unable to create test output directory.");
  }
  const QString statesOutputPath =
    "test-output/algorithms/ConflictsNetworkMatcherSettingsOptimizer-states-out";
  LOG_ERROR("Writing best states and failing test groups to: " << statesOutputPath << "...");
  const QStringList failingTestsForBestRuns = fitnessFunction->getFailingTestsForBestRuns();
  output +=
    "Failing Test Groups For Best States (" + QString::number(failingTestsForBestRuns.size()) +
    "):\n\n";
  if (failingTestsForBestRuns.size() == 0)
  {
    output += "no failing test groups\n\n";
  }
  else
  {
    for (int i = 0; i < failingTestsForBestRuns.size(); i++)
    {
      output += "#" + QString::number(i + 1) + ":\n\n";
      QString failingTestsStr = failingTestsForBestRuns.at(i);
      if (failingTestsStr.contains(";"))
      {
        QStringList failingTests = failingTestsStr.split(";");
        for (int j = 0; j < failingTests.size(); j++)
        {
          output += failingTests.at(j) + "\n";
        }
      }
      else
      {
        output += failingTestsStr + "\n";
      }
      output += "\n";
    }
  }
  output += "Best States (" + QString::number(bestStates.size()) + "):\n\n";
  int statesCtr = 1;
  foreach (Tgs::ConstStatePtr state, bestStates)
  {
    output += "#" + QString::number(statesCtr) + ":\n\n";
    output += state->toString() + "\n\n";
    statesCtr++;
  }
  LOG_VARD(QDir::currentPath());
  FileUtils::writeFully(statesOutputPath, output);
}

void ConflictsNetworkMatcherSettingsOptimizer::runOptimization(
  boost::shared_ptr<AbstractTestFitnessFunction> fitnessFunction, const int numIterations)
{
  Tgs::SimulatedAnnealing sa(_initStateDescription(), fitnessFunction);
  sa.setPickFromBestScores(true);
  /*bestScore =*/ sa.iterate(numIterations);
  _writeOutput(fitnessFunction, sa.getBestStates(), numIterations);
}
}
