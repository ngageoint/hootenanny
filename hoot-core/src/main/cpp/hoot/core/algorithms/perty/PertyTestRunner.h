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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PERTY_TEST_RUNNER_H
#define PERTY_TEST_RUNNER_H

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>

// Qt
#include <QString>
#include <QStringList>

namespace hoot
{

class PertyTestRunResult;
class PertyMatchScorer;

/**
 * Runs a PERTY test to generate a PERTY score with an option to vary a single PERTY input variable
   (or multiple vars given the same value) across a number of test runs.  The scores are averaged
   across simulation runs for each test runs and each compared to an expected score.
 */
class PertyTestRunner : public Configurable
{

public:

  PertyTestRunner();
  virtual ~PertyTestRunner() = default;

  /**
    * Runs a PERTY test

      @param referenceMapInputPath Path to the input OSM map file to run PERTY against
      @param outputPath Output path for file and results
    */
  QList<std::shared_ptr<const PertyTestRunResult>> runTest(const QString& referenceMapInputPath,
                                                           const QString& outputPath);

  virtual void setConfiguration(const Settings& conf) { _settings = conf; }

  /**
    Sets the number of PERTY test runs
   */
  void setNumTestRuns(int numRuns)
  {
    if (numRuns < 1)
    {
      throw HootException("Invalid number of test runs: " + QString::number(numRuns));
    }
    _numTestRuns = numRuns;
  }

  /**
    Sets the number of PERTY test simulations per test run
   */
  void setNumTestSimulations(int numSimulations)
  {
    if (numSimulations < 1)
    {
      throw HootException("Invalid number of test simulations: " + QString::number(numSimulations));
    }
    _numTestSimulations = numSimulations;
  }

  /**
     Sets the PERTY input variables to receive changing values per test run
   */
  void setDynamicVariables(const QStringList& dynamicVariables)
  {
    _dynamicVariables.clear();
    foreach (QString var, dynamicVariables)
    {
      //this isn't the best check, since not all perty.* vars are numeric but will do for now
      if (var.trimmed() != "")
      {
        if (!var.startsWith("perty."))
        {
          throw HootException("Only PERTY variables may be manipulated during a PERTY test (config options = perty.*");
        }
        _dynamicVariables.append(var);
      }
    }
  }

  /**
    Sets a starting value for the dynamic variables specified in the PERTY dynamic variables list
   */
  void setDynamicVariableStartValue(double startValue)
  {
    _dynamicVariableStartValue = startValue;
  }

  /**
    Sets a per test run postive increment for the dynamic variables specified in the PERTY dynamic
    variables list
   */
  void setDynamicVariableIncrement(double increment)
  {
    _dynamicVariableIncrement = increment;
  }

  /**
    Sets a list of expected scores for each test run
   */
  void setExpectedScores(const QStringList& scores)
  {
    if (scores.size() < 1)
    {
      throw HootException("Invalid number of expected scores: " + scores.size());
    }
    QList<double> expectedScores;
    for (int i = 0; i < scores.size(); i++)
    {
      bool ok;
      expectedScores.append(scores[i].toDouble(&ok)) ;
      if (!ok)
      {
        throw HootException("Error parsing expected score value: " + scores[i]);
      }
    }
    _expectedScores = expectedScores;
  }

  /**
    Sets the maximum allowed amount a test run score may vary from an expected score while allowing
    the test run to pass
   */
  void setAllowedScoreVariance(double scoreVariance)
  {
    if (scoreVariance > 1.0 || scoreVariance < 0.0)
    {
      throw HootException("Invalid allowed score variance: " + QString::number(scoreVariance));
    }
    _allowedScoreVariance = scoreVariance;
  }

  /**
    Sets a setting that determines whether the test runner marks a score as failing if its test
    run score is better than its expected score
   */
  void setFailOnBetterScore(bool failOnBetterScore) { _failOnBetterScore = failOnBetterScore; }

  /**
    Sets a setting that determines whether stats are generated for each output map
   */
  void setGenerateMapStats(bool generateMapStats) { _generateMapStats = generateMapStats; }

private:

  int _numTestRuns;
  int _numTestSimulations;
  QStringList _dynamicVariables;
  double _dynamicVariableStartValue;
  double _dynamicVariableIncrement;

  QList<double> _expectedScores;
  double _allowedScoreVariance;
  bool _failOnBetterScore;

  bool _generateMapStats;

  Settings _settings;

  std::shared_ptr<PertyMatchScorer> _matchScorer;

  void _writeStatsForOutputFiles(const QString& inputMapPath, const QString& sep);
  void _writePlotFile(const QString& outputPath,
                      const QList<std::shared_ptr<const PertyTestRunResult>>& testRunResults);

  //for testing purposes only
  friend class PertyTestRunnerTest;
  QList<double> _testScores;
  bool _returnTestScores;

};

}

#endif // PERTY_TEST_RUNNER_H
