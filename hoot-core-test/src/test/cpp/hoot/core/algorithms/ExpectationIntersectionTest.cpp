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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/ExpectationIntersection.h>
#include <hoot/core/util/Log.h>

// OpenCV
// #include <opencv/cv.h>

// Boost
#include <boost/random/normal_distribution.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/variate_generator.hpp>

// Tgs
#include <tgs/Statistics/Random.h>

using namespace cv;
using namespace std;

namespace hoot
{


class ExpectationIntersectionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ExpectationIntersectionTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(run3SplitTest);
  CPPUNIT_TEST(runSmallSplitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    ExpectationIntersection uut;

    boost::minstd_rand rng;
    rng.seed(42);
    boost::normal_distribution<> ndx;
    boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> varx(rng, ndx);

    int set1 = 10;
    int set2 = 20;
    Mat m = Mat::zeros(set1 + set2, 2, CV_64F);

    for (int i = 0; i < set1; i++)
    {
      m.at<double>(i, 0) = varx() + 2;
      m.at<double>(i, 1) = varx();
    }

    for (int i = set1; i < set1 + set2; i++)
    {
      m.at<double>(i, 0) = varx();
      m.at<double>(i, 1) = varx() + 2;
    }

    Mat ranges = Mat::zeros(2, 2, CV_32S);
    ranges.at<int>(0, 0) = 0;
    ranges.at<int>(0, 1) = 20;
    ranges.at<int>(1, 0) = 9;
    ranges.at<int>(1, 1) = 29;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.174, uut.snapMatches(m, ranges)[0], 0.001);
  }

  void run3SplitTest()
  {
    ExpectationIntersection uut;

    boost::minstd_rand rng;
    rng.seed(42);
    boost::normal_distribution<> ndx;
    boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> varx(rng, ndx);

    int set1 = 10;
    int set2 = 20;
    int set3 = 50;
    Mat m = Mat::zeros(set1 + set2 + set3, 2, CV_64F);

    int c = 0;
    for (int i = 0; i < set1; i++)
    {
      m.at<double>(c, 0) = varx() + 2;
      m.at<double>(c++, 1) = varx();
    }

    for (int i = 0; i < set2; i++)
    {
      m.at<double>(c, 0) = varx();
      m.at<double>(c++, 1) = varx() + 2;
    }

    for (int i = 0; i < set3; i++)
    {
      m.at<double>(c, 0) = varx() + 2;
      m.at<double>(c++, 1) = varx();
    }

    Mat ranges = Mat::zeros(3, 2, CV_32S);
    ranges.at<int>(0, 0) = 0;
    ranges.at<int>(0, 1) = 20;
    ranges.at<int>(1, 0) = 9;
    ranges.at<int>(1, 1) = 40;
    ranges.at<int>(2, 0) = 25;
    ranges.at<int>(2, 1) = 79;
    vector<double> splits = uut.snapMatches(m, ranges);

    CPPUNIT_ASSERT_EQUAL(2, (int)splits.size());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.174, splits[0], 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(29.535, splits[1], 0.001);
  }

  /**
   * This is based on a bug that I found while running some other unit tests. The range values were
   * not being used properly to bound the split which caused an error.
   */
  void runSmallSplitTest()
  {
    ExpectationIntersection uut;

    Mat m = Mat::zeros(50, 2, CV_64F);

    m.at<double>(0, 0) = 0.16666666666666666667;
    m.at<double>(0, 1) = 0.785398163;

    for (int i = 1; i < 50; i++)
    {
      m.at<double>(i, 0) = 0.16666666666666666667;
      m.at<double>(i, 1) = 0;
    }

    Mat ranges = Mat::zeros(2, 2, CV_32S);
    ranges.at<int>(0, 0) = 0;
    ranges.at<int>(0, 1) = 17;
    ranges.at<int>(1, 0) = 1;
    ranges.at<int>(1, 1) = 49;
    vector<double> splits = uut.snapMatches(m, ranges);

    HOOT_STR_EQUALS("[1]{1}", splits);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ExpectationIntersectionTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ExpectationIntersectionTest, "quick");

}
