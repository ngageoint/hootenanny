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
#ifndef PERTY_TEST_RUNNER_H
#define PERTY_TEST_RUNNER_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class PertyTestRunResult;
class PertyMatchScorer;

/**
 * @brief The PertyTestRunner class runs a PERTY test to generate a PERTY score with an option to
 * vary a single PERTY input variable (or multiple vars given the same value) across a number of
 * test runs. The scores are averaged across simulation runs for each test runs and each compared
 * to an expected score.
 */
class PertyTestRunner : public Configurable
{

public:

  PertyTestRunner();
  ~PertyTestRunner() = default;

  /**
    * @brief runTest runs a PERTY test.
      @param referenceMapInputPath Path to the input OSM map file to run PERTY against
      @param outputPath Output path for file and results
    */
  QList<std::shared_ptr<const PertyTestRunResult>> runTest(const QString& referenceMapInputPath,
                                                           const QString& outputPath);

  void setConfiguration(const Settings& conf) override { _settings = conf; }

  /**
    @brief setNumTestRuns sets the number of PERTY test runs.
   */
  void setNumTestRuns(int numRuns);
  /**
    @brief setNumTestSimulations sets the number of PERTY test simulations per test run.
   */
  void setNumTestSimulations(int numSimulations);
  /**
     @brief setDynamicVariables sets the PERTY input variables to receive changing values per test
     run.
   */
  void setDynamicVariables(const QStringList& dynamicVariables);
  /**
    @brief setDynamicVariableStartValue sets a starting value for the dynamic variables specified in
    the PERTY dynamic variables list
   */
  void setDynamicVariableStartValue(double startValue)
  {
    _dynamicVariableStartValue = startValue;
  }
  /**
    @brief setDynamicVariableIncrement sets a per test run postive increment for the dynamic
    variables specified in the PERTY dynamic variables list
   */
  void setDynamicVariableIncrement(double increment)
  {
    _dynamicVariableIncrement = increment;
  }
  /**
    @brief setExpectedScores sets a list of expected scores for each test run.
   */
  void setExpectedScores(const QStringList& scores);
  /**
    @brief setAllowedScoreVariance sets the maximum allowed amount a test run score may vary from an
    expected score while allowing the test run to pass.
   */
  void setAllowedScoreVariance(double scoreVariance);
  /**
    @brief setFailOnBetterScore sets a setting that determines whether the test runner marks a score
    as failing if its test run score is better than its expected score.
   */
  void setFailOnBetterScore(bool failOnBetterScore) { _failOnBetterScore = failOnBetterScore; }
  /**
    @brief setGenerateMapStats Sets a setting that determines whether stats are generated for each
    output map.
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

  void _writeStatsForOutputFiles(const QString& inputMapPath, const QString& sep) const;
  void _writePlotFile(const QString& outputPath,
                      const QList<std::shared_ptr<const PertyTestRunResult>>& testRunResults) const;

  //for testing purposes only
  friend class PertyTestRunnerTest;
  QList<double> _testScores;
  bool _returnTestScores;

};

}

#endif // PERTY_TEST_RUNNER_H
