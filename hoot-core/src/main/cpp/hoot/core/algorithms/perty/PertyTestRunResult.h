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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef PERTY_TEST_RUN_RESULT_H
#define PERTY_TEST_RUN_RESULT_H

// Standard
#include <math.h>

namespace hoot
{

/**
 * @brief The PertyTestRunResult class represents the outcome of a PERTY test run.
 */
class PertyTestRunResult
{

public:

  PertyTestRunResult(const QString& referenceInput, const QString& outputDir, const int testRunNum,
                     const QList<double> simulationScores, const double score,
                     const double expectedScore, const double scoreVariance,
                     const double allowedScoreVariance, bool failOnBetterScore,
                     const QStringList& dynamicVariables, const double dynamicVariableStartingValue,
                     const double dynamicVariableIncrement, const double dynamicVariableValue);

  /**
    @brief testPassed determines whether the test run passed.
   */
  bool testPassed() const;

  /**
    @brief getTestRunNum returns the test run number.
    */
  int getTestRunNum() const { return _testRunNum; }
  /**
    @brief getReferenceInput returns the test run input.
    */
  QString getReferenceInput() const { return _referenceInput; }
  /**
    @brief getExpectedScore returns the expected score.
    */
  double getExpectedScore() const { return _expectedScore; }
  /**
    @brief getScoreVariance returns the score variance.
    */
  double getScoreVariance() const { return _scoreVariance; }
  /**
    @brief getAllowedScoreVariance returns the allowed score variance.
    */
  double getAllowedScoreVariance() const { return _allowedScoreVariance; }
  /**
    @brief getFailOnBetterScore returns whether the test run should fail if its score is better that
    its expected score.
    */
  bool getFailOnBetterScore() const { return _failOnBetterScore; }
  /**
    @brief getDynamicVariables returns the dynamic variables.
    */
  QStringList getDynamicVariables() const { return _dynamicVariables; }
  /**
    @brief getDynamicVariableStartingValue returns the dynamic variable starting value.
    */
  double getDynamicVariableStartingValue() const { return _dynamicVariableStartingValue; }
  /**
    @brief getDynamicVariableIncrement returns the dynamic variable increment.
    */
  double getDynamicVariableIncrement() const { return _dynamicVariableIncrement; }
  /**
    @brief getDynamicVariableValue returns the dynamic variable value.
    */
  double getDynamicVariableValue() const { return _dynamicVariableValue; }
  /**
    @brief getScore returns the test run score.
    */
  double getScore() const { return _score; }
  /**
    @brief getSimulationScores returns the simulation scores for the test run.
    */
  QList<double> getSimulationScores() const { return _simulationScores; }
  /**
    @brief getOutputDir returns the output directory.
    */
  QString getOutputDir() const { return _outputDir; }

  /**
    @brief toString returns a string representation of the object.
    */
  QString toString() const;

private:

  const QString _referenceInput;
  const QString _outputDir;
  const int _testRunNum;
  const double _score;
  const QList<double> _simulationScores;
  const double _expectedScore;
  const double _scoreVariance;
  const double _allowedScoreVariance;
  const bool _failOnBetterScore;
  const QStringList _dynamicVariables;
  const double _dynamicVariableStartingValue;
  const double _dynamicVariableIncrement;
  const double _dynamicVariableValue;
};

}

#endif // PERTY_TEST_RUN_RESULT_H
