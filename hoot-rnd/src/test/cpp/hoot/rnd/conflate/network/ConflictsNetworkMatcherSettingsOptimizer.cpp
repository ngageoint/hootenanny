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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
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

namespace hoot
{

// used for parameter tuning only and isn't a true test.
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

  class CaseFitnessFunction : public Tgs::FitnessFunction
  {
  public:

    //TODO: modify fitness function to give variable failure based on the number of reviews
    virtual double f(const ConstStatePtr& s) const
    {
      Settings settings;
      foreach (QString k, s->getAllValues().keys())
      {
        settings.set(k, s->get(k));
      }
      //possibly an easier way to do this would be to read these directly from
      //test-files/cases/hoot-rnd/network/Config.conf instead
      settings.set("match.creators", "hoot::NetworkMatchCreator");
      settings.set("merger.creators", "hoot::NetworkMergerCreator");
      settings.set("uuid.helper.repeatable", "true");
      settings.set("writer.include.debug", "true");
      settings.set("network.matcher", "hoot::ConflictsNetworkMatcher");
      settings.set("conflate.add.review.detail", "false");

      TempFileName temp;
      LOG_VARW(temp.getFileName());
      settings.storeJson(temp.getFileName());

      ConflateCaseTestSuite suite("test-files/cases/hoot-rnd/network/conflicts/");
      const int testCount = suite.getChildTestCount();
      int failures = 0;
      for (int i = 0; i < testCount; ++i)
      {
        ConflateCaseTest* test = dynamic_cast<ConflateCaseTest*>(suite.getChildTestAt(i));
        const QString testName = QString::fromStdString(test->getName());
        //LOG_WARN("Running " << testName << "...");
        test->addConfig(temp.getFileName());
        CppUnit::TestResult result;
        SimpleListener listener;
        result.addListener(&listener);
        test->run(&result);

        if (listener.isFailure())
        {
          LOG_WARN("Failure: " << testName);
          failures++;
        }
      }

      LOG_WARN(failures << "/" << testCount << " tests failed");
      if (failures == 0)
      {
        //TODO: This message will actually show if, by chance, the first selected random state
        //is successful.  However, that state won't be included in what's returned from sa.
        LOG_WARN("\n\n***BOOM GOES THE DYNAMITE!***\n");
      }

      return (double)failures / (double)testCount;
    }
  };

  void optimizeAgainstCaseDataTest()
  {
    StateDescriptionPtr desc(new StateDescription());
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsAggressionKey(),
        //VariableDescription::Real, 4.4, 4.4)); //default
        //VariableDescription::Real, 0.0, 10.0)); //min/max
        VariableDescription::Real, 9.59, 9.69));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsPartialHandicapKey(),
        //VariableDescription::Real, 0.2, 0.2)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.2, 0.2));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubHandicapKey(),
        //VariableDescription::Real, .86, .86)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 1.8, 1.9));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsWeightInfluenceKey(),
        //VariableDescription::Real, 0.0, 0.0)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.0, 0.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsOutboundWeightingKey(),
        //VariableDescription::Real, 0.0, 0.0)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.68, 0.89));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubThroughWeightingKey(),
        //VariableDescription::Real, 0.32, 0.32)); //default
        //VariableDescription::Real, 0.0, 10.0)); //min/max
        VariableDescription::Real, 8.0, 8.1));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMaxStubLengthKey(),
        //VariableDescription::Real, 20.0, 20.0)); //default
        //VariableDescription::Real, 1.0, 100.0));  //min/max??
        VariableDescription::Real, 35.0, 46.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMatchThresholdKey(),
        //VariableDescription::Real, 0.15, 0.15)); //default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.15, 0.15));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMissThresholdKey(),
        //VariableDescription::Real, 0.85, 0.85)); //default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.85, 0.85));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkReviewThresholdKey(),
        //VariableDescription::Real, 0.5, 0.5)); //default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.5, 0.5));

    shared_ptr<FitnessFunction> ff(new CaseFitnessFunction());
    SimulatedAnnealing sa(desc, ff);
    sa.setPickFromBestScores(true);
    sa.iterate(50);

    foreach (ConstStatePtr state, sa.getBestStates())
    {
      LOG_VARW(state);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflictsNetworkMatcherSettingsOptimizer, "current");

}
