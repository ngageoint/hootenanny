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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <iostream>

// TBS
#include <tbs/stats/TDistribution.h>

#include <boost/math/special_functions/digamma.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

using namespace cv;

namespace tbs
{
using namespace std;

class TDistributionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TDistributionTest);
  CPPUNIT_TEST(runTest1D);
  CPPUNIT_TEST(runTest2D);
  CPPUNIT_TEST_SUITE_END();

public:

// The older version of GCC in CentOS 6 gives a bunch of false warnings.
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION < 40800
# pragma GCC diagnostic ignored "-Wuninitialized"
#endif

  void runTest2D()
  {
    int count = 1000;
    Mat m = Mat::zeros(count, 2, CV_64F);

    boost::minstd_rand rng;
    boost::normal_distribution<> ndx;
    boost::normal_distribution<> ndy;
    boost::uniform_real<> uni(0.0, 1.0);

    boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> varx(rng, ndx);
    boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> vary(rng, ndy);

    uni(rng);

    for (int i = 0; i < count; i++)
    {
      double x = varx();
      double y = vary();
      m.at<double>(i, 0) = x;
      m.at<double>(i, 1) = y * 2;
    }

    m.at<double>(0, 0) = 100.0;

    TDistribution td(m);

    // these were not manually verified but they "look" right and should let us know if something
    // changes.
    Mat p = Mat::zeros(1, 2, CV_64F);
    p.at<double>(0, 0) = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1195, td.getLikelihood(p), 0.0001);

    p.at<double>(0, 0) = 1.0;
    p.at<double>(0, 1) = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0386, td.getLikelihood(p), 0.0001);

    p.at<double>(0, 0) = 0.0;
    p.at<double>(0, 1) = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0878, td.getLikelihood(p), 0.0001);

    p.at<double>(0, 0) = 1.0;
    p.at<double>(0, 1) = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0458, td.getLikelihood(p), 0.0001);
  }

  void runTest1D()
  {
    int count = 1000;
    Mat m = Mat::zeros(count, 1, CV_64F);

    boost::minstd_rand rng;
    boost::normal_distribution<> ndx;
    boost::normal_distribution<> ndy;
    boost::uniform_real<> uni(0.0, 1.0);

    boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> varx(rng, ndx);
    boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> vary(rng, ndy);

    uni(rng);

    for (int i = 0; i < count; i++)
    {
      double x = varx();
      m.at<double>(i, 0) = x;
    }

    TDistribution td(m);

    // these values were manually verified and are approximately correct. The randomness of the data

    Mat p = Mat::zeros(1, 1, CV_64F);
    p.at<double>(0, 0) = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.41, td.getLikelihood(p), 0.01);

    p.at<double>(0, 0) = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.24, td.getLikelihood(p), 0.01);

    p.at<double>(0, 0) = 2.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, td.getLikelihood(p), 0.01);

    p.at<double>(0, 0) = 3.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, td.getLikelihood(p), 0.01);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TDistributionTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TDistributionTest, "quick");

}
