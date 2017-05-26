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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QBuffer>

// Standard Includes
#include <iostream>
#include <math.h>
using namespace std;

// TGS Includes
#include <tgs/TgsException.h>
#include <tgs/Interpolation/KernelEstimationInterpolator.h>
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/Statistics/Random.h>
#include <tgs/Statistics/Normal.h>

namespace Tgs
{

class KernelEstimationInterpolatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(KernelEstimationInterpolatorTest);
  CPPUNIT_TEST(optimizationTest);
  CPPUNIT_TEST(serializationTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST_SUITE_END();
public:

  boost::shared_ptr<KernelEstimationInterpolator> buildRandom()
  {
    boost::shared_ptr<KernelEstimationInterpolator> result(new KernelEstimationInterpolator());
    KernelEstimationInterpolator& uut = *result;

    boost::shared_ptr<DataFrame> dfPtr(new DataFrame());
    DataFrame& df = *dfPtr;
    vector<string> labels;
    labels.push_back("x");
    labels.push_back("y");
    labels.push_back("h");
    vector<int> types(3, DataFrame::Numerical);
    df.setFactorLabels(labels);
    df.setFactorTypes(types);

    // is there a cleaner way?
    vector<double> d(3);

    double n0 = Normal::normal(0, 0.5);
    for (size_t i = 0; i < 100; i++)
    {
      d[0] = Random::instance()->generateUniform() * 2 - 1;
      d[1] = Random::instance()->generateUniform() * 2 - 1;
      double e = sqrt(d[0] * d[0] + d[1] * d[1]);
      d[2] = Normal::normal(e, .5) / n0;
      df.addDataVector("", d);
    }

    uut.setData(dfPtr);
    vector<string> ind;
    ind.push_back("x");
    ind.push_back("y");
    uut.setIndependentColumns(ind);
    vector<string> dep;
    dep.push_back("h");
    uut.setDependentColumns(dep);

    return result;
  }

  void optimizationTest()
  {
    Tgs::Random::instance()->seed(0);

    boost::shared_ptr<KernelEstimationInterpolator> di = buildRandom();
    KernelEstimationInterpolator& uut = *di;
    uut.setStopDelta(0.0001);
    uut.setSigma(-1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.155, uut.getSigma(), 0.001);

//    // write out as a .pgm image
//    ofstream fs;
//    fs.open("output.pgm");

//    double max = Normal::normal(0, 0.5) / n0;

//    fs << "P2" << endl;
//    fs << "100 100" << endl;
//    fs << "300" << endl;
//    int maxC = 0;
//    vector<double> p(2);
//    for (size_t py = 0; py < 100; py++)
//    {
//      for (size_t px = 0; px < 100; px++)
//      {
//        double x = px / 50.0 - 1.0;
//        double y = py / 50.0 - 1.0;
//        p[0] = x;
//        p[1] = y;

//        double e = sqrt(x * x + y * y);
//        double v = Normal::normal(e, .5) / n0;

//        const vector<double>& r = uut.interpolate(p);
//        int c = (int)((v - r[0]) / max * 255 + 128);
//        c = (int)((r[0]) / max * 255);
//        cout << r[0] << " ";
//        maxC = std::max(c, maxC);
//        fs << c << " ";
//      }
//      fs << endl;
//    }
//    fs.close();
  }

  void serializationTest()
  {
    Tgs::Random::instance()->seed(0);

    boost::shared_ptr<KernelEstimationInterpolator> di = buildRandom();
    KernelEstimationInterpolator& uut = *di;
    uut.setStopDelta(0.0001);
    uut.setSigma(0.3);

    Tgs::Random::instance()->seed(0);
    double error = uut.estimateError();

    QBuffer buf;
    buf.open(QBuffer::WriteOnly);

    uut.writeInterpolator(buf);

    buf.close();
    buf.open(QBuffer::ReadOnly);

    KernelEstimationInterpolator copy;
    copy.readInterpolator(buf);
    Tgs::Random::instance()->seed(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(error, copy.estimateError(), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, copy.getSigma(), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0001, copy.getStopDelta(), 0.0001);
  }

  void simpleTest()
  {
    Tgs::Random::instance()->seed(0);
    KernelEstimationInterpolator uut(.1);

    boost::shared_ptr<DataFrame> dfPtr(new DataFrame());
    DataFrame& df = *dfPtr;
    vector<string> labels;
    labels.push_back("x");
    labels.push_back("y");
    labels.push_back("h");
    vector<int> types(3, DataFrame::Numerical);
    df.setFactorLabels(labels);
    df.setFactorTypes(types);

    // is there a cleaner way?
    vector<double> d(3);

    for (size_t i = 0; i < 500; i++)
    {
      d[0] = Random::instance()->generateUniform() * 2 - 1;
      d[1] = Random::instance()->generateUniform() * 2 - 1;
      d[2] = Normal::normal(d[0], .5);
      df.addDataVector("", d);
    }

    uut.setData(dfPtr);
    vector<string> ind;
    ind.push_back("x");
    //ind.push_back("y");
    uut.setIndependentColumns(ind);
    vector<string> dep;
    dep.push_back("h");
    uut.setDependentColumns(dep);

    uut.setSigma(0.002);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3247, uut.estimateError(), 0.0001);
    uut.setSigma(0.008);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0721615, uut.estimateError(), 0.00001);
  }

};

}


//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::KernelEstimationInterpolatorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::KernelEstimationInterpolatorTest, "quick");

