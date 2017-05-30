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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/ml/LogisticRegression.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

using namespace std;

class LogisticRegressionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(LogisticRegressionTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    // this test came from training a Logistic model in Weka and displaying the predictions. I then
    // use the weka generated coefficients and intercept to verify against several samples.
    LogisticRegression uut;
    uut.setIntercept(-166.5986);

    LogisticRegression::Coeff coeff;
    coeff["AttributeScore"] = 121.9717;
    coeff["DistanceScore"] = -2.9825;
    coeff["LengthScore"] = -8.254;
    coeff["ParallelScore"] = 35.6726;
    coeff["PortionScore"] = 18.4649;
    coeff["ProbabilityScore"] = 20.8245;
    uut.setCoefficients(coeff);

    map<QString, double> sample;
    sample["AttributeScore"] = 1;
    sample["DistanceScore"] = 0.399088463352104;
    sample["LengthScore"] = 0.667299903312345;
    sample["ParallelScore"] = 0.920388982337191;
    sample["PortionScore"] = 0.725104961168539;
    sample["ProbabilityScore"] = 0.0612775870280161;
    sample["ZipperScore"] = 0.25;

    double p = uut.evaluate(sample);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.021, p, 0.001);

    sample["AttributeScore"] = 1;
    sample["DistanceScore"] = 0.588656899563679;
    sample["LengthScore"] = 0.509870795790218;
    sample["ParallelScore"] = 0.986994857478687;
    sample["PortionScore"] = 0.735982417844526;
    sample["ProbabilityScore"] = 0.074058902963291;
    sample["ZipperScore"] = 0.25;

    p = uut.evaluate(sample);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.438, p, 0.001);

    sample["AttributeScore"] = 1;
    sample["DistanceScore"] = 0.879991475505835;
    sample["LengthScore"] = 0.759791901714845;
    sample["ParallelScore"] = 0.991592300475001;
    sample["PortionScore"] = 0.838350883783242;
    sample["ProbabilityScore"] = 0.16574723033824;
    sample["ZipperScore"] = 0.25;

    p = uut.evaluate(sample);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.686, p, 0.001);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LogisticRegressionTest, "quick");

