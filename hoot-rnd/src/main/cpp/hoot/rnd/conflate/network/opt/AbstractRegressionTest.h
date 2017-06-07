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
#ifndef ABSTRACTREGRESSIONTEST_H
#define ABSTRACTREGRESSIONTEST_H

// Hoot
#include <hoot/core/test/AbstractTest.h>

// Qt
#include <QDir>
#include <QStringList>

namespace hoot
{

/**
 * At this time, this will only work against the network versions of the regression tests but
 * could easily be made to work against the unifying versions as well with some test refactoring.
 */
class AbstractRegressionTest : public AbstractTest
{

public:

  AbstractRegressionTest(QDir d, QStringList confs);

  virtual void runTest();

  double getMinPassingScore() const { return _minPassingScore; }
  void setMinPassingScore(double score) { _minPassingScore = score; }

  double getOverallScore() const { return _overallScore; }
  void setOverallScore(double score) { _overallScore = score; }

protected:

  virtual void _parseScore() = 0;

  double _minPassingScore;
  double _overallScore;

};

}

#endif // ABSTRACTREGRESSIONTEST_H
