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

#ifndef __ABSTRACTREGRESSIONTESTFITNESSFUNCTION_H__
#define __ABSTRACTREGRESSIONTESTFITNESSFUNCTION_H__

// Hoot
#include <hoot/core/conflate/optimization/AbstractTestFitnessFunction.h>

// Qt


namespace hoot
{

class AbstractRegressionTest;

/**
 * @brief The AbstractRegressionTestFitnessFunction class Fitness function which optimizes against
 * Hootenanny regression release test data.
 *
 * This class uses the error log setting to allow for more easily viewing the results of the
 * optimization without having to view conflation log clutter...there may be a better way to
 * handle this.
 *
 * At this time, this will only work against the network versions of the regression tests but
 * could easily be made to work against the unifying versions as well with some test refactoring.
 */
class AbstractRegressionTestFitnessFunction : public AbstractTestFitnessFunction
{

public:

  AbstractRegressionTestFitnessFunction(const QString& dir, const QString& configFile,
                                        const QString& testDirExtension);

  /**
   * @see Tgs::FitnessFunction
   */
  virtual double f(const Tgs::ConstStatePtr& s);

  /**
   * @see AbstractTestFitnessFunction
   */
  virtual void initTest(Settings& testSettings);
  /**
   * @see AbstractTestFitnessFunction
   */
  virtual void afterTestRun();

  /**
   * @brief bestScoresPerTestToString Pretty printed string for the best score for each test in the
   * test suite
   * @return a scores string
   */
  QString bestScoresPerTestToString() const;

protected:

  QMap<QString, double> _testsToBestScores;
  QString _configFile;

private:

  void _createConfig(const QString& testName, Settings& testSettings);
  void _checkForBetterScoreFromTest(AbstractRegressionTest* regressionTest);
};

}

#endif // __ABSTRACTREGRESSIONTESTFITNESSFUNCTION_H__
