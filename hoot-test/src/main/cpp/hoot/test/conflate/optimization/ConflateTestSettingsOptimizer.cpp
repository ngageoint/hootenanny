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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "ConflateTestSettingsOptimizer.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

#include <hoot/test/conflate/optimization/AbstractRegressionTestFitnessFunction.h>

// Qt
#include <QTextStream>

// Tgs
#include <tgs/Optimization/SimulatedAnnealing.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// Standard
#include <float.h>
#include <set>

namespace hoot
{

ConflateTestSettingsOptimizer::ConflateTestSettingsOptimizer(bool verbose) :
_verbose(verbose)
{
}

Tgs::StateDescriptionPtr ConflateTestSettingsOptimizer::_initStateDescription(
  const QString& testSettingsFile)
{
  LOG_VARD(testSettingsFile);

  boost::property_tree::ptree propTree;
  try
  {
    boost::property_tree::read_json(testSettingsFile.toStdString(), propTree);
  }
  catch (boost::property_tree::json_parser::json_parser_error& e)
  {
    throw HootException(
      QString("Error parsing JSON: %1 (line %2)")
        .arg(QString::fromStdString(e.message()))
        .arg(QString::number(e.line())));
  }
  catch (const std::exception& e)
  {
    const QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }

  Tgs::StateDescriptionPtr stateDescription = std::make_shared<Tgs::StateDescription>();
  for (boost::property_tree::ptree::value_type& setting : propTree.get_child("settings"))
  {
    const QString settingName = QString::fromStdString(setting.second.get<std::string>("name", ""));
    LOG_VART(settingName);
    if (!conf().hasKey(settingName))
    {
      throw HootException("Invalid setting: " + settingName);
    }
    Tgs::ConstVariableDescriptionPtr var =
      std::make_shared<Tgs::VariableDescription>(
        settingName,
        Tgs::VariableDescription::Real,
        setting.second.get<double>("min", DBL_MIN),
        setting.second.get<double>("max", DBL_MAX));
    LOG_VART(var->toString());
    stateDescription->addVariable(var);
  }

  return stateDescription;
}

void ConflateTestSettingsOptimizer::runOptimization(
  std::shared_ptr<AbstractTestFitnessFunction> fitnessFunction, const int numIterations,
  const QString& testSettingsFile, const QString& outputFile)
{
  Tgs::SimulatedAnnealing sa(_initStateDescription(testSettingsFile), fitnessFunction);
  sa.setPickFromBestScores(true);
  sa.setVerbose(_verbose);
  /*bestScore =*/ sa.iterate(numIterations);
  _writeOutput(fitnessFunction, sa.getBestStates(), numIterations, outputFile);
}

void ConflateTestSettingsOptimizer::_writeOutput(
  const std::shared_ptr<AbstractTestFitnessFunction>& fitnessFunction,
  const QSet<Tgs::ConstStatePtr>& bestStates, const int numIterations, const QString& outputFile)
{
  QString output =
    "Results for Conflicts Network Matcher Configuration Option Optimization with Simulated Annealing\n\n";

  QString temp = "Number of test iterations: " + QString::number(numIterations);
  LOG_INFO(temp);
  output += temp + "\n\n";

  temp = "Number of tests in test suite: " + QString::number(fitnessFunction->getTestCount());
  LOG_INFO(temp);
  output += temp + "\n\n";

  temp =
    "Lowest number of test failures in test iteration: " +
    QString::number(fitnessFunction->getLowestNumFailingTestsPerRun());
  LOG_INFO(temp);
  output += temp + "\n\n";

  temp = "Number of best states found: " + QString::number(bestStates.size());
  LOG_INFO(temp);
  output += temp + "\n\n";

  std::shared_ptr<AbstractRegressionTestFitnessFunction> regressionTestFitnessFunction =
    std::dynamic_pointer_cast<AbstractRegressionTestFitnessFunction>(fitnessFunction);
  if (regressionTestFitnessFunction)
  {
    temp = regressionTestFitnessFunction->bestScoresPerTestToString();
    LOG_INFO(temp);
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
  LOG_INFO(temp);
  output += temp + "\n\n";

  LOG_INFO("Writing best states and failing test groups to: " << outputFile << "...");
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
  FileUtils::writeFully(outputFile, output);
}

}
