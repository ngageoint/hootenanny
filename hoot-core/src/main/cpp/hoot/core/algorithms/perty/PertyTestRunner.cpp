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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PertyTestRunner.h"

// hoot
#include <hoot/core/algorithms/perty/PertyTestRunResult.h>
#include <hoot/core/algorithms/perty/PertyMatchScorer.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

// Standard
#include <cmath>

using namespace std;

namespace hoot
{

PertyTestRunner::PertyTestRunner() :
_settings(conf()),
_returnTestScores(false)
{
  ConfigOptions configOptions;
  setNumTestRuns(configOptions.getPertyTestNumRuns());
  setNumTestSimulations(configOptions.getPertyTestNumSimulations());
  setDynamicVariables(configOptions.getPertyTestDynamicVariables());
  setDynamicVariableStartValue(configOptions.getPertyTestDynamicVariableStartValue());
  setDynamicVariableIncrement(configOptions.getPertyTestDynamicVariableIncrement());
  setExpectedScores(configOptions.getPertyTestExpectedScores());
  setAllowedScoreVariance(configOptions.getPertyTestAllowedScoreVariance());
  setFailOnBetterScore(configOptions.getPertyTestFailOnBetterScore());
  setGenerateMapStats(configOptions.getPertyTestGenerateMapStats());
}

void PertyTestRunner::_writeStatsForOutputFiles(const QString& inputMapPath, const QString& sep)
{
  QString statsOutputPath = inputMapPath;
  statsOutputPath = statsOutputPath.replace(".osm", "-stats");
  MapStatsWriter().writeStats(inputMapPath, statsOutputPath, sep);
}

QList<std::shared_ptr<const PertyTestRunResult>> PertyTestRunner::runTest(
  const QString& referenceMapInputPath, const QString& outputPath)
{
  if (_expectedScores.size() != _numTestRuns)
  {
    throw HootException(
      "The number of test runs: " + QString::number(_numTestRuns) + " does not equal the number " +
      "of expected scores: " + QString::number(_expectedScores.size()));
  }

  LOG_INFO(
    "Running PERTY test with " << _numTestRuns << " test runs and " << _numTestSimulations <<
    " simulations per test run on input: " << referenceMapInputPath << " ...");

  FileUtils::makeDir(outputPath);

  const QString sep = "\t";
  if (_generateMapStats)
  {
    QFileInfo inputFileInfo(referenceMapInputPath);
    QString statsOutputPath = outputPath + "/" + inputFileInfo.fileName();
    statsOutputPath = statsOutputPath.replace(".osm", "-stats");
    MapStatsWriter().writeStats(referenceMapInputPath, statsOutputPath, sep);
  }

  QFile resultsFile(outputPath + "/results");
  if (resultsFile.exists())
  {
    resultsFile.remove();
  }

  QList<std::shared_ptr<const PertyTestRunResult>> testRunResults;
  double dynamicVariableValue = _dynamicVariableStartValue;
  _matchScorer.reset(new PertyMatchScorer());
  int testScoreCtr = 0;
  for (int i = 0; i < _numTestRuns; i++)
  {
    for (int j = 0; j < _dynamicVariables.size(); j++)
    {
      // this code does nothing if there are no dynamic variables present
      _settings.set(_dynamicVariables[j], QString::number(dynamicVariableValue));
    }
    _matchScorer->setConfiguration(_settings);

    QList<double> simulationScores;
    double score;
    double scoreSum = 0;
    for (int j = 0; j < _numTestSimulations; j++)
    {
      LOG_INFO(
        "Running test run #" << QString::number(i + 1) << ", simulation #" <<
        QString::number(j + 1));
      if (!_dynamicVariables.empty())
      {
        LOG_INFO(" with dynamic variables: " << _dynamicVariables << " having value: " <<
                 dynamicVariableValue);
      }

      if (!_returnTestScores)
      {
        const QString testRunOutputPath =
          outputPath + "/test-" + QString::number(i + 1) + "-" + QString::number(j + 1);
        std::shared_ptr<const MatchComparator> matchComparator =
          _matchScorer->scoreMatches(referenceMapInputPath, testRunOutputPath);
        score = matchComparator->getPertyScore();
        simulationScores.append(score);
        scoreSum += score;

        if (_generateMapStats)
        {
          _writeStatsForOutputFiles(_matchScorer->getReferenceMapOutput(), sep);
          _writeStatsForOutputFiles(_matchScorer->getPerturbedMapOutput(), sep);
          _writeStatsForOutputFiles(_matchScorer->getConflatedMapOutput(), sep);
        }
      }
      else
      {
        score = _testScores.at(testScoreCtr);
        simulationScores.append(score);
        scoreSum += score;
        testScoreCtr++;
      }
      LOG_INFO(
        "Received score of: " << score << " for test run #" << QString::number(i + 1) <<
        ", simulation #" << QString::number(j + 1));
      LOG_VARD(scoreSum);
    }

    const double avgScore = (double)scoreSum / (double)_numTestSimulations;
    LOG_VARD(avgScore);
    const double scoreVariance = abs(_expectedScores[i] - avgScore);
    LOG_VARD(scoreVariance);

    std::shared_ptr<const PertyTestRunResult> testRunResult(
      new PertyTestRunResult(
         referenceMapInputPath, outputPath, i + 1, simulationScores, avgScore, _expectedScores[i],
          scoreVariance, _allowedScoreVariance, _failOnBetterScore, _dynamicVariables,
          _dynamicVariableStartValue, _dynamicVariableIncrement, dynamicVariableValue));
    testRunResults.append(testRunResult);
    const QString testRunResultStr = testRunResult->toString();

    if (_numTestRuns > 1)
    {
      LOG_INFO(testRunResultStr);
    }
    // write after each test result iteration vs. after they're all finished in case an error occurs
    // during a test, in which we'd lose output for all previous tests
    if (resultsFile.open(QFile::Append | QFile::Text))
    {
      QTextStream out(&resultsFile);
      out << testRunResultStr;
      resultsFile.close();
    }
    else
    {
      LOG_ERROR("Unable to write to results file.");
    }

    //this code effectively does nothing if there are no dynamic variables present
    dynamicVariableValue += _dynamicVariableIncrement;
  }
  _testScores.clear();

  if (!_dynamicVariables.empty())
  {
    _writePlotFile(outputPath, testRunResults);
  }

  return testRunResults;
}

void PertyTestRunner::_writePlotFile(const QString& outputPath,
                                     const QList<std::shared_ptr<const PertyTestRunResult>>& testRunResults)
{
  QFile plotFile(outputPath + "/results-plot.dat");
  if (plotFile.exists())
  {
    plotFile.remove();
  }

  QString outStr = "";
  double dynamicVariableValue = _dynamicVariableStartValue;
  for (QList<std::shared_ptr<const PertyTestRunResult>>::const_iterator it = testRunResults.begin();
       it != testRunResults.end(); ++it)
  {
    std::shared_ptr<const PertyTestRunResult> result = *it;
    outStr += QString::number(dynamicVariableValue) + " " + QString::number(result->getScore()) + "\n";
    dynamicVariableValue += _dynamicVariableIncrement;
  }

  if (plotFile.open(QFile::Append | QFile::Text))
  {
    QTextStream out(&plotFile);
    out << outStr;
    plotFile.close();
  }
  else
  {
    LOG_ERROR("Unable to write to plot file.");
  }
}

}
