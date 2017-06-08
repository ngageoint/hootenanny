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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/rnd/conflate/opt/ConflateSettingsOptimizer.h>
#include <hoot/rnd/conflate/network/opt/CaseTestFitnessFunction.h>

namespace hoot
{

/*
 * only the case tests can be run from this unit tests; regression optimization testing is handled
 * in the nightly tests
 */
class ConflateSettingsOptimizerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ConflateSettingsOptimizerTest);
  CPPUNIT_TEST(runSimpleCaseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  //we're not verifying the output with this test...just checking that it doesn't error out and that
  //it generates output
  void runSimpleCaseTest()
  {
     DisableLog dl(Log::Fatal);

     QDir().mkpath("test-output/algorithms");
     const QString output = "test-output/algorithms/ConflateSettingsOptimizerTest-states-out";
     QFile outputFile(output);
     if (outputFile.exists() && !outputFile.remove())
     {
       throw HootException("Error removing output file: " + output);
     }

     ConflateSettingsOptimizer().runOptimization(
       boost::shared_ptr<AbstractTestFitnessFunction>(
         new CaseTestFitnessFunction(
           "test-files/cases/hoot-rnd/network/conflicts",
           /*this config adds in a few other tests necessary for the case tests to run correctly;
           it does not specify the matcher, however, and assumes it to be the conflicts matcher,
           which was the default when this test was written*/
           "test-files/cases/hoot-rnd/network/Config.conf")),
       2,
       "test-files/cmd/slow/OptimizeConflateSettingsCmd/testSettings.json",
       output);


     CPPUNIT_ASSERT(outputFile.exists());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateSettingsOptimizerTest, "slow");

}
