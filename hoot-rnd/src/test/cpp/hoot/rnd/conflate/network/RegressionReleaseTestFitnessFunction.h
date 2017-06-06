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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __REGRESSIONRELEASETESTFITNESSFUNCTION_H__
#define __REGRESSIONRELEASETESTFITNESSFUNCTION_H__

// Hoot
#include "AbstractTestFitnessFunction.h"

// Qt
#include <QMap>

namespace hoot
{

/**
 * Fitness function which optimizes against Hootenanny regression release test data.
 *
 * At this time, this will only work against the network versions of the regression tests but
 * could easily be made to work against the unifying versions as well with some test refactoring.
 */
class RegressionReleaseTestFitnessFunction : public AbstractTestFitnessFunction
{

public:

    RegressionReleaseTestFitnessFunction(QString dir, QString configFile);

    virtual double f(const Tgs::ConstStatePtr& s);

    virtual void initTest();

    virtual void afterTestRun();

    QString highestOverallScoresToString() const;

private:

    QMap<QString, int> _highestOverallScores;
    QString _configFile;

    void _createConfig(const QString testName);
    void _updateTestWithCurrentScore(AbstractTest* test);
    void _updateCurrentScoreFromTest(const int score, const QString testName);
};

}

#endif // __REGRESSIONRELEASETESTFITNESSFUNCTION_H__
