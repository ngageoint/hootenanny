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
#ifndef RELEASETEST_H
#define RELEASETEST_H

// Hoot
#include <hoot/core/conflate/optimization/AbstractRegressionTest.h>

namespace hoot
{

/**
 * Represents a hoot regression release test to be run as part of a hoot tests suite.
 *
 * This class uses the error log setting to allow for more easily viewing the results of the
 * optimization without having to view conflation log clutter...there may be a better way to
 * handle this.
 *
 * At this time, this will only work against the network versions of the regression tests but
 * could easily be made to work against the unifying versions of those same tests as well with some
 * code and test refactoring.
 */
class ReleaseTest : public AbstractRegressionTest
{

public:

  /**
   * @brief PertyTest
   * @param d base dir containing the tests
   * @param confs configuration file paths to pass along to the tests
   */
  ReleaseTest(QDir d, QStringList confs);

protected:

  /*
   * Parse the score output of a regression test from a file
   */
  virtual void _parseScore();

};

}

#endif // RELEASETEST_H
