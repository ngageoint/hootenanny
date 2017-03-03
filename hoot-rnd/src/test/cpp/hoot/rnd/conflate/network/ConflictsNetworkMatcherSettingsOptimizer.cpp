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

  //Things that could be better:
  //  - show the iteration number w/o having to temporarily add a cout statement to
  //    SimulatedAnnealing
  //  - modify fitness function to give variable failure based on the number of reviews
  class CaseFitnessFunction : public Tgs::FitnessFunction
  {
  public:

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
      settings.set("conflate.match.highway.classifier", "hoot::HighwayExpertClassifier");
      settings.set("way.subline.matcher", "hoot::MaximalSublineMatcher");

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
        //This message will actually show if, by chance, the first selected random state
        //is successful.  However, that state won't be included in what's returned from sa...
        //so this logging this success message is a little misleading in that situation.
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
        //VariableDescription::Real, 8.8, 8.8)); //default
        //VariableDescription::Real, 0.0, 10.0)); //min/max
        VariableDescription::Real, 5.0, 9.7));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsPartialHandicapKey(),
        //VariableDescription::Real, 0.2, 0.2)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.0, 0.46));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubHandicapKey(),
        //VariableDescription::Real, 1.7, 1.7)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.0, 0.56));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsWeightInfluenceKey(),
        //VariableDescription::Real, 0.0, 0.0)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.34, 1.49));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsOutboundWeightingKey(),
        //VariableDescription::Real, 0.25, 0.25)); //default
        //VariableDescription::Real, 0.0, 2.0)); //min/max
        VariableDescription::Real, 0.0, 0.51));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubThroughWeightingKey(),
        //VariableDescription::Real, 0.5, 0.5)); //default
        //VariableDescription::Real, 0.0, 10.0)); //min/max
        VariableDescription::Real, 6.5, 10.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMaxStubLengthKey(),
        //VariableDescription::Real, 20.0, 20.0)); //default
        //VariableDescription::Real, 1.0, 100.0));  //min/max??
        VariableDescription::Real, 36.0, 100.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMatchThresholdKey(),
        //VariableDescription::Real, 0.15, 0.15)); //default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.01, 0.39));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkMissThresholdKey(),
        //VariableDescription::Real, 0.85, 0.85)); //default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.72, 0.99));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkReviewThresholdKey(),
        //VariableDescription::Real, 0.5, 0.5)); //default
        //VariableDescription::Real, 0.01, 0.99));  //min/max
        VariableDescription::Real, 0.8, 0.99));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getWayMergerMinSplitSizeKey(),
        VariableDescription::Real, 5.0, 5.0)); //default
        //VariableDescription::Real, 0.01, 100.0));  //min/max??
        //VariableDescription::Real, 0.5, 30.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getWayMatcherMaxAngleKey(),
        VariableDescription::Real, 60.0, 60.0)); //default
        //VariableDescription::Real, 0.01, 90.0));  //min/max
        //VariableDescription::Real, 20.0, 75.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getWayMatcherHeadingDeltaKey(),
        VariableDescription::Real, 5.0, 5.0)); //default
        //VariableDescription::Real, 0.01, 100.0));  //min/max??
        //VariableDescription::Real, 0.5, 50.0));

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
