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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "PertyTestRunResult.h"

// TBS
#include <tbs/stats/SampleStats.h>

using namespace std;

namespace hoot
{

PertyTestRunResult::PertyTestRunResult(
  const QString& referenceInput, const QString& outputDir, const int testRunNum,
  const QList<double> simulationScores, const double score, const double expectedScore,
  const double scoreVariance, const double allowedScoreVariance, const bool failOnBetterScore,
  const QStringList& dynamicVariables, const double dynamicVariableStartingValue,
  const double dynamicVariableIncrement, const double dynamicVariableValue) :
_referenceInput(referenceInput),
_outputDir(outputDir),
_testRunNum(testRunNum),
_score(score),
_simulationScores(simulationScores),
_expectedScore(expectedScore),
_scoreVariance(scoreVariance),
_allowedScoreVariance(allowedScoreVariance),
_failOnBetterScore(failOnBetterScore),
_dynamicVariables(dynamicVariables),
_dynamicVariableStartingValue(dynamicVariableStartingValue),
_dynamicVariableIncrement(dynamicVariableIncrement),
_dynamicVariableValue(dynamicVariableValue)
{
}

bool PertyTestRunResult::testPassed() const
{
  const bool scoreWithinVariance = getScoreVariance() <= getAllowedScoreVariance();
  if (getFailOnBetterScore())
  {
    return scoreWithinVariance;
  }
  else
  {
    return scoreWithinVariance || (getScore() > getExpectedScore());
  }
}

QString PertyTestRunResult::toString() const
{
  QString str = "";
  str += "\n\nTest Run #" + QString::number(_testRunNum) + "\n\n";
  str += "Reference Input: " + _referenceInput + "\n";
  str += "Output Directory: " + _outputDir + "\n";
  if (!getDynamicVariables().empty())
  {
    str += "Dynamic Variable Names: " + getDynamicVariables().join(", ") + "\n";
    str += "Dynamic Variable Starting Value: " + QString::number(getDynamicVariableStartingValue()) + "\n";
    str += "Dynamic Variable Increment: " + QString::number(getDynamicVariableIncrement()) + "\n\n";
    str += "Dynamic Variable Value: " + QString::number(getDynamicVariableValue()) + "\n";
  }
  str += "Simulation Scores:\n";
  for (int i = 0; i < _simulationScores.size(); i++)
  {
    str += "     #" + QString::number(i + 1) + ": " + QString::number(_simulationScores[i]) + "\n";
  }
  vector<double> scores = _simulationScores.toVector().toStdVector();
  tbs::SampleStats stats(scores);
  str += "Test Run Score (averaged): " + QString::number(_score) + "\n";
  str += QString("Test Run CI90: %1, %2\n").arg(stats.calculateCi90Lower()).
      arg(stats.calculateCi90Upper());
  str += "Expected Score: " + QString::number(getExpectedScore()) + "\n";
  str += "Score Variance From Expected: " + QString::number(getScoreVariance()) + "\n";
  str += "Allowed Score Variance: " + QString::number(getAllowedScoreVariance()) + "\n";
  QString failOnBetterScoreStr = "false";
  if (getFailOnBetterScore())
  {
    failOnBetterScoreStr = "true";
  }
  str += "Fail On Better Score: " + failOnBetterScoreStr + "\n";
  QString testPassedStr = "false";
  if (testPassed())
  {
    testPassedStr = "true";
  }
  str += "Test Passed: " + testPassedStr + "\n";
  return str;
}

}
