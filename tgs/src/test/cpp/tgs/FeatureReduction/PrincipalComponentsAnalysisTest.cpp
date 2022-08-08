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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <float.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Newmat Includes
#include <newmat/newmat.h>

#include <tgs/StreamUtils.h>
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/FeatureReduction/PrincipalComponentsAnalysis.h>
#include <tgs/Statistics/Random.h>

#include "../PluginFactory.h"
#include "../FeatureSelection/GeneratedData.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{
  class DblVec : public std::vector<double>
  {
  public:
    DblVec() {}

    DblVec(double f1, double f2)
    {
      resize(2);
      operator[](0) = f1;
      operator[](1) = f2;
    }

    DblVec(double f1, double f2, double f3)
    {
      resize(3);
      operator[](0) = f1;
      operator[](1) = f2;
      operator[](2) = f3;
    }

    static bool isClose(const std::vector<double>& v1, const std::vector<double>& v2)
    {
      CPPUNIT_ASSERT_EQUAL(v1.size(), v2.size());
      for (unsigned int i = 0; i < v1.size(); i++)
      {
        if (fabs(v1[i] - v2[i]) > 1e-4)
        {
          return false;
        }
      }
      return true;
    }
  };

  const double PI = 3.14159265358979323846;

  class PrincipalComponentAnalysisTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(PrincipalComponentAnalysisTest);
    CPPUNIT_TEST(testReproject);
    CPPUNIT_TEST(sanityCheck);
    CPPUNIT_TEST(testRandom);
    //CPPUNIT_TEST(test20);
    CPPUNIT_TEST_SUITE_END();
  public:
    void populateSimpleDataFrame(DataFrame& df, double angle = 10.0)
    {
      // create a really simple data frame on a 10 deg angle
      double theta = angle / 360.0 * 2.0 * PI;
      df.addDataVector("a", DblVec(1 * cos(theta),   1 * sin(theta)));
      df.addDataVector("a", DblVec(2 * cos(theta),   2 * sin(theta)));
      df.addDataVector("b", DblVec(-1 * cos(theta), -1 * sin(theta)));
      df.addDataVector("b", DblVec(.5 * cos(theta), .5 * sin(theta)));
      std::vector<std::string> factorLabels;
      factorLabels.push_back("f1");
      factorLabels.push_back("f2");
      df.setFactorLabels(factorLabels);
    }

    /**
     * @param angle is in degrees
     */
    std::vector<double> rotate(std::vector<double>& p, double angle)
    {
      double theta = angle / 360.0 * 2.0 * PI;
      double x = p[0] * cos(theta) - p[1] * sin(theta);
      double y = p[0] * sin(theta) + p[1] * cos(theta);
      std::vector<double> result(2);
      result[0] = x;
      result[1] = y;
      return result;
    }

    void sanityCheck()
    {
      DataFrame df;
      populateSimpleDataFrame(df);

      PrincipalComponentsAnalysis uut;
      uut.compute(df);
      vector<vector<double>> result = uut.getComponents();

      ASSERT_DBL_EQL(0.984808, result[0][0]);
      ASSERT_DBL_EQL(0.173648, result[0][1]);
      ASSERT_DBL_EQL(-0.173648, result[1][0]);
      ASSERT_DBL_EQL(0.984808, result[1][1]);
    }

    void testRandom()
    {
      testRandom(10.0);
      testRandom(44.0);
      testRandom(45.0);
      testRandom(50.0);
      testRandom(60.0);
      testRandom(110.0);
      testRandom(160.0);
      testRandom(178.0);
    }

    /**
     * Specify angle in degrees
     */
    void testRandom(double angle)
    {
      DataFrame df;
      // convert to radians
      double theta = 2.0 * PI * angle / 360.0;
      while (df.getNumDataVectors() < 10000)
      {
        // create a random point inside a circle
        double x = 2.0 * Tgs::Random::instance()->generateUniform() - 1;
        double y = 2.0 * Tgs::Random::instance()->generateUniform() - 1;
        if (sqrt(x * x + y * y) > 1.0)
        {
          continue;
        }
        // make the circle wide
        x *= 5.0;

        double xp = x * cos(theta) - y * sin(theta);
        double yp = x * sin(theta) + y * cos(theta);

        df.addDataVector("a", DblVec(xp, yp));
      }
      std::vector<std::string> factorLabels;
      factorLabels.push_back("x");
      factorLabels.push_back("y");
      df.setFactorLabels(factorLabels);

      PrincipalComponentsAnalysis uut;
      uut.compute(df);
      vector<vector<double>> result = uut.getComponents();

      double x1 = result[0][0];
      double y1 = result[0][1];

      double angle1 = atan2(result[0][1], result[0][0]) / (2.0 * PI) * 360.0;
      double angle2 = atan2(result[1][1], result[1][0]) / (2.0 * PI) * 360.0;

      double angle2test = angle - 90;
      if (angle2test < 0)
      {
        angle2test += 180;
      }

      if (angle1 < 0)
      {
        angle1 += 180;
      }
      if (angle2 < 0)
      {
        angle2 += 180;
      }

      if (fabs(angle1 - angle) > 1.0 || fabs(angle2 - angle2test) > 1.0)
      {
        cout << "**** Error ****" << endl;
        cout << "  angle: " << angle << endl;
        cout << "  angle2test: " << angle2test << endl;
        cout << "  angle1: " << angle1 << endl;
        cout << "  angle2: " << angle2 << endl;
        cout << "  x1: " << x1 << endl;
        cout << "  y1: " << y1 << endl;
        cout << angle1 << endl;
        cout << angle2 << endl;
        cout << result << endl << endl;
      }

      //CPPUNIT_ASSERT_DOUBLES_EQUAL(theta, angle1, .5);
      //CPPUNIT_ASSERT_DOUBLES_EQUAL(theta + 90, angle2, .5);
    }

    // don't have a way to validate this at this point. :(
    void test20()
    {
      DataFrame df;
      generatedData20(df);
      DataFrame df2;
      int keepColumns[6] = { 0, 1, 2, 6, 7, 8 };
      std::vector<double> v(6);
      std::vector<std::string> labels(6);
      for (unsigned int r = 0; r < df.getNumDataVectors(); r++)
      {
        for (int i = 0; i < 6; i++)
        {
          v[i] = df.getDataVector(r)[keepColumns[i]];
          labels[i] = df.getFactorLabelFromIndex(keepColumns[i]);
        }
        df2.addDataVector(df.getTrainingLabel(r), v);
      }
      df2.setFactorLabels(labels);

      cout << df2;

      PrincipalComponentsAnalysis uut;
      uut.compute(df2);
      vector<vector<double>> result = uut.getComponents();

      cout << result;
    }

    void testReproject()
    {
      for (unsigned int i = 0; i < 40; i++)
      {
        double angle = Tgs::Random::instance()->generateInt(360);
        DataFrame df;
        populateSimpleDataFrame(df, angle);

        //cout << df;
        PrincipalComponentsAnalysis uut;
        uut.compute(df);
        //cout << uut.getComponents();

        DblVec p(Tgs::Random::instance()->generateUniform(), Tgs::Random::instance()->generateUniform());
        std::vector<double> result = uut.reprojectIntoPca(p);
        std::vector<double> rp = rotate(p, -(double)angle);
        std::vector<double> rpi = rotate(p, -(double)angle + 180);
        //cout << angle << ": " << p << " -> " << rp << endl;
        if (DblVec::isClose(rp, result) == false && DblVec::isClose(rpi, result) == false)
        {
          cout << "**** Error ****" << endl;
          cout << angle << "is: " << result << endl << "  should be: " << rp << " or " << rpi << endl;
          CPPUNIT_ASSERT(false);
        }
        result = uut.reprojectFromPca(rp);
        DblVec::isClose(p, result);
      }
    }
  };
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::PrincipalComponentAnalysisTest, PluginFactory::testName());
