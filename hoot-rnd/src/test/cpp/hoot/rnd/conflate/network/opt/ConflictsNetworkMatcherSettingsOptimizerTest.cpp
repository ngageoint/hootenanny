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
#include <hoot/rnd/conflate/network/opt/ConflictsNetworkMatcherSettingsOptimizer.h>
#include <hoot/rnd/conflate/network/opt/CaseTestFitnessFunction.h>
#include <hoot/rnd/conflate/network/opt/ReleaseTestFitnessFunction.h>
#include <hoot/rnd/conflate/network/opt/PertyTestFitnessFunction.h>

namespace hoot
{

/*
 * "runSimpleCaseTest" is a brief unit test of the optimizer for case tests only.
 *
 * The "optimizeAgainst*" tests are used for network conflation parameter tuning only and aren't
 * actually unit tests, so leave them disabled by default.  Most of the time you want to run those
 * at the error log level to reduce log clutter.  Also when running them, temporarily uncomment
 * cout lines in SimulatedAnnealing::iterate for better logging feedback.
 *
 * TODO: come up with a better way to control logging inside SimulatedAnnealing than uncommenting
 * cout lines
 */
class ConflictsNetworkMatcherSettingsOptimizerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ConflictsNetworkMatcherSettingsOptimizerTest);
  //CPPUNIT_TEST(runSimpleCaseTest);
  //leave these disabled in source control
  //CPPUNIT_TEST(optimizeAgainstCaseDataTest);
  //CPPUNIT_TEST(optimizeAgainstRegressionReleaseDataTest);
  CPPUNIT_TEST(optimizeAgainstRegressionPertyDataTest);
  CPPUNIT_TEST_SUITE_END();

public:

  //TODO: modify fitness function to give variable failure based on the number of reviews (#1092)

  //we're not checking the output with this test...just that it doesn't error out
  void runSimpleCaseTest()
  {
     DisableLog dl(Log::Fatal);

     boost::shared_ptr<AbstractTestFitnessFunction> fitnessFunction(
       new CaseTestFitnessFunction(
         "test-files/cases/hoot-rnd/network/conflicts",
         "test-files/cases/hoot-rnd/network/Config.conf"));
     ConflictsNetworkMatcherSettingsOptimizer().runOptimization(fitnessFunction, 2);
  }

  void optimizeAgainstCaseDataTest()
  {
    boost::shared_ptr<AbstractTestFitnessFunction> fitnessFunction(
      new CaseTestFitnessFunction(
        "test-files/cases/hoot-rnd/network/conflicts",
        "test-files/cases/hoot-rnd/network/Config.conf"));
    ConflictsNetworkMatcherSettingsOptimizer().runOptimization(fitnessFunction, 2);
  }

  void optimizeAgainstRegressionReleaseDataTest()
  {
    boost::shared_ptr<AbstractTestFitnessFunction> fitnessFunction(
      new ReleaseTestFitnessFunction(
        //fill this in with the full path to the test dir
        "/fouo/hoot-tests/network-tests.child/release_test.child",
        "test-files/cases/hoot-rnd/network/Config.conf"));
    ConflictsNetworkMatcherSettingsOptimizer().runOptimization(fitnessFunction, 2);
  }

  void optimizeAgainstRegressionPertyDataTest()
  {
    boost::shared_ptr<AbstractTestFitnessFunction> fitnessFunction(
      new PertyTestFitnessFunction(
        //fill this in with the full path to the test dir
        "/fouo/hoot-tests/network-tests.child/perty-tests.child",
        "test-files/cases/hoot-rnd/network/Config.conf"));
    ConflictsNetworkMatcherSettingsOptimizer().runOptimization(fitnessFunction, 2);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflictsNetworkMatcherSettingsOptimizerTest, "slow");

}
