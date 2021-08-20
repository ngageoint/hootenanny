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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/NodeMatcher.h>
#include <hoot/core/util/Settings.h>

// Standard
#include <math.h>

using namespace std;

namespace hoot
{

class NodeMatcherTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NodeMatcherTest);
  CPPUNIT_TEST(runSimpleAnglesTest);
  CPPUNIT_TEST(runAnglesTest);
  CPPUNIT_TEST(runAnglesMismatchTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runSimpleAnglesTest()
  {
    NodeMatcher uut;

    double epsilon = 0.0001;

    vector<double> theta1;
    theta1.push_back(0.0);
    theta1.push_back(M_PI);
    vector<double> theta2;
    theta2.push_back(0.0 + epsilon);
    theta2.push_back(M_PI - epsilon);

    vector<bool> exclude(theta1.size(), false);
    double score = uut._calculateAngleScore(theta1, theta2, exclude, 0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(score, 1.0, 0.0001);
  }

  void runAnglesTest()
  {
    NodeMatcher uut;

    double epsilon = 0.0001;

    vector<double> theta1;
    theta1.push_back(0.0);
    theta1.push_back(M_PI);
    theta1.push_back(M_PI + M_PI / 4.0);
    vector<double> theta2;
    theta2.push_back(0.0 + epsilon);
    theta2.push_back(M_PI - M_PI / 4.0);
    theta2.push_back(M_PI + epsilon);

    vector<int> result;
    result.resize(3);

    vector<bool> exclude(theta1.size(), false);
    double score = uut._calculateAngleScore(theta1, theta2, exclude, 0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25004, score, 0.0001);
  }

  void runAnglesMismatchTest()
  {
    NodeMatcher uut;

    double epsilon = 0.0001;

    vector<double> theta1;
    theta1.push_back(0.0);
    theta1.push_back(M_PI);
    vector<double> theta2;
    theta2.push_back(0.0 + epsilon);
    theta2.push_back(M_PI - M_PI / 4.0);
    theta2.push_back(M_PI + epsilon);

    vector<int> result;
    result.resize(3);

    vector<bool> exclude(theta1.size(), false);
    double score = uut._calculateAngleScore(theta1, theta2, exclude, 0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, score, 0.0001);
  }

};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::NodeMatcherTest, "quick");

