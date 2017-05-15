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

// Cpp Unit
#include <cppunit/TestListener.h>
#include <cppunit/TestResult.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/test/ConflateCaseTestSuite.h>
#include <hoot/rnd/conflate/network/ConflictsNetworkMatcher.h>

// Qt
#include <QTemporaryFile>

// Tgs
#include <tgs/Optimization/SimulatedAnnealing.h>

using namespace Tgs;

namespace hoot
{

/*
 * This is used for network conflation parameter tuning only and isn't actually a unit test.
 *
 * IMPORTANT: Most of the time you want to run this at the error log level to reduce log clutter.
 *
 * TODO: come up with a better way to control logging inside SimulatedAnnealing than cout
 */
class ConflictsNetworkMatcherSettingsOptimizer : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ConflictsNetworkMatcherSettingsOptimizer);
  //CPPUNIT_TEST(optimizeAgainstCaseDataTest);
  CPPUNIT_TEST_SUITE_END();

public:

  class SimpleListener : public CppUnit::TestListener
  {

  public:

    SimpleListener() : _failure(false) {}

    virtual void addFailure( const CppUnit::TestFailure & /*failure*/ ) { _failure = true; }
    bool isFailure() const { return _failure; }

  private:

    bool _failure;
  };

  class TempFileName
  {
  public:

    TempFileName()
    {
      do
      {
        int r = Random::instance()->generateInt();
        _name = QString("/tmp/HootConflictsTest-%1.conf").arg(r, 10, 10, QChar('0'));
      }
      while (QFile(_name).exists());
    }

    ~TempFileName()
    {
      if (QFile(_name).exists())
      {
        if (!QFile(_name).remove())
        {
          LOG_WARN("Failure removing: " << _name);
        }
      }
    }

    QString getFileName() const { return _name; }

  private:

    QString _name;
  };

  //TODO: modify fitness function to give variable failure based on the number of reviews
  class CaseFitnessFunction : public Tgs::FitnessFunction
  {
  public:

    virtual double f(const ConstStatePtr& s) const
    {
      LOG_DEBUG("Running fitness function...");

      Settings settings;
      foreach (QString k, s->getAllValues().keys())
      {
        settings.set(k, s->get(k));
      }
      //if you need to add any other temporary custom settings for this test that wouldn't
      //normally be used with the network conflation case tests, add those here
      //settings.set("", "");
      LOG_VART(settings);
      TempFileName temp;
      LOG_VARD(temp.getFileName());
      settings.storeJson(temp.getFileName());

      //this init will add the conflicts network case tests conf which is a subset of the overall
      //network cases tests conf
      ConflateCaseTestSuite suite("test-files/cases/hoot-rnd/network/conflicts/");
      const int testCount = suite.getChildTestCount();
      QStringList failedTests;
      for (int i = 0; i < testCount; ++i)
      {
        ConflateCaseTest* test = dynamic_cast<ConflateCaseTest*>(suite.getChildTestAt(i));
        const QString testName = QString::fromStdString(test->getName());
        //LOG_ERROR("Running " << testName << "...");
        //we still need to add the overall network cases tests conf
        test->addConfig("test-files/cases/hoot-rnd/network/Config.conf");
        //add our custom sa test option values
        test->addConfig(temp.getFileName());
        CppUnit::TestResult result;
        SimpleListener listener;
        result.addListener(&listener);
        test->run(&result);

        if (listener.isFailure())
        {
          //LOG_ERROR("Failure: " << testName);
          failedTests.append(testName);
        }
      }

      if (failedTests.size() == 0)
      {
        //This message will actually show if, by chance, the first selected random state
        //is successful.  However, that state is just a starting point for the actual simulated
        //annealing iterations.
        LOG_ERROR("\n\n***BOOM GOES THE DYNAMITE!***\n");
      }
      else
      {
        QString failureMsg =
          QString::number(failedTests.size()) + "/" + QString::number(testCount) +
          " tests failed:\n\n";
        for (int i = 0; i < failedTests.size(); i++)
        {
          failureMsg += "\t" + failedTests[i] + "\n";
        }
        LOG_ERROR(failureMsg);
      }

      return (double)failedTests.size() / (double)testCount;
    }
  };

  void optimizeAgainstCaseDataTest()
  {
    StateDescriptionPtr desc(new StateDescription());
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsAggressionKey(),
        //VariableDescription::Real, 4.4, 4.4)); //original default
        //VariableDescription::Real, 8.8, 8.8)); //current default
        //VariableDescription::Real, 0.0, 10.0)); //min/max
        VariableDescription::Real, 3.4, 5.4)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsPartialHandicapKey(),
        //VariableDescription::Real, 0.2, 0.2)); //original default
        //VariableDescription::Real, 0.2, 0.2)); //current default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.1, 0.3)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubHandicapKey(),
        //VariableDescription::Real, 0.86, 0.86)); //original default
        //VariableDescription::Real, 1.7, 1.7)); //current default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.76, 0.96)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsWeightInfluenceKey(),
        //VariableDescription::Real, 0.0, 0.0)); //original default
        //VariableDescription::Real, 0.0, 0.0)); //current default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.0, 0.1)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsOutboundWeightingKey(),
        //VariableDescription::Real, 0.0, 0.0)); //original default
        //VariableDescription::Real, 0.25, 0.25)); //current default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.0, 0.1)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubThroughWeightingKey(),
        //VariableDescription::Real, 0.59, 0.59)); //original default
        //VariableDescription::Real, 0.5, 0.5)); //current default
        //VariableDescription::Real, 0.0, 10.0)); //min/max
        VariableDescription::Real, 0.49, 0.69)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMaxStubLengthKey(),
        //VariableDescription::Real, 20.0, 20.0)); //original default
        //VariableDescription::Real, 20.0, 20.0)); //current default
        //VariableDescription::Real, 1.0, 100.0));  //min/max??
        VariableDescription::Real, 15.0, 25.0)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMatchThresholdKey(),
        //VariableDescription::Real, 0.15, 0.15)); //original default
        //VariableDescription::Real, 0.15, 0.15)); //current default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.05, 0.25)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMissThresholdKey(),
        //VariableDescription::Real, 0.85, 0.85)); //original default
        //VariableDescription::Real, 0.85, 0.85)); //current default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.75, 0.95)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkReviewThresholdKey(),
        //VariableDescription::Real, 0.5, 0.5)); //original default
        //VariableDescription::Real, 0.5, 0.5)); //current default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.4, 0.6)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getWayMergerMinSplitSizeKey(),
        //VariableDescription::Real, 5.0, 5.0)); //original default
        //VariableDescription::Real, 5.0, 5.0)); //current default
        //VariableDescription::Real, 0.01, 100.0));  //min/max??
        VariableDescription::Real, 4.0, 6.0)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getWayMatcherMaxAngleKey(),
        //VariableDescription::Real, 60.0, 60.0)); //original default
        //VariableDescription::Real, 60.0, 60.0)); //current default
        //VariableDescription::Real, 0.01, 90.0));  //min/max
        VariableDescription::Real, 50.0, 70.0)); //test values
    desc->addVariable(
      new VariableDescription(ConfigOptions::getWayMatcherHeadingDeltaKey(),
        //VariableDescription::Real, 5.0, 5.0)); //original default
        //VariableDescription::Real, 5.0, 5.0)); //new default
        //VariableDescription::Real, 0.01, 100.0));  //min/max??
        VariableDescription::Real, 4.0, 6.0)); //test values

    boost::shared_ptr<FitnessFunction> ff(new CaseFitnessFunction());
    SimulatedAnnealing sa(desc, ff);
    sa.setPickFromBestScores(true);
    const double bestScore = sa.iterate(100);
    LOG_ERROR("Best score: " << bestScore << " - (failures / num tests; lower is better)");
    if (bestScore == 0.0)
    {
      LOG_ERROR("***YOU FOUND A SOLUTION!***");
    }
    else
    {
      LOG_ERROR("No solution was found :-(");
    }
    LOG_ERROR("\nBest states:\n");
    foreach (ConstStatePtr state, sa.getBestStates())
    {
      LOG_VARE(state);
      LOG_ERROR("");
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflictsNetworkMatcherSettingsOptimizer, "glacial");

}
