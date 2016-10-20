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

class ConflictsNetworkMatcherTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ConflictsNetworkMatcherTest);
  // used for parameter tuning and isn't a true test.
  //CPPUNIT_TEST(optimizeTest);
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
    virtual double f(const ConstStatePtr& s) const
    {
      Settings settings;
      foreach (QString k, s->getAllValues().keys())
      {
        settings.set(k, s->get(k));
      }

      TempFileName temp;
      LOG_VARW(temp.getFileName());
      settings.storeJson(temp.getFileName());

      ConflateCaseTestSuite suite("test-files/cases/hoot-rnd/network/conflicts/");

      int failures = 0;

      for (int i = 0; i < suite.getChildTestCount(); ++i)
      {
        ConflateCaseTest* test = dynamic_cast<ConflateCaseTest*>(suite.getChildTestAt(i));

        // this one fails due to review scores and we don't really care.
        if (QString::fromStdString(test->getName()).contains("highway-009"))
        {
          continue;
        }

        test->addConfig(temp.getFileName());
        CppUnit::TestResult result;
        SimpleListener listener;
        result.addListener(&listener);
        test->run(&result);

        if (listener.isFailure())
        {
          LOG_WARN("Failure: " << test->getName());
          failures++;
        }
      }

      LOG_VARW(failures);
      LOG_VARW(suite.getChildTestCount());

      return (double)failures / (double)suite.getChildTestCount();
    }
  };

  void optimizeTest()
  {
    StateDescriptionPtr desc(new StateDescription());

    // good for highway-008
    // getNetworkConflictsPartialHandicapKey - 0.5
    // getNetworkConflictsStubHandicapKey - 0.5
    // getNetworkConflictsStubThroughWeightingKey - 1.0


    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsPartialHandicapKey(),
        VariableDescription::Real, 0.5, 0.5));
        //VariableDescription::Real, 0.0, 1.5));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubHandicapKey(),
        // value of 0.5 here gives good results in test highway-008
        //VariableDescription::Real, 0.6, 0.6));
        VariableDescription::Real, .86, .86));
        //VariableDescription::Real, 0.5, 1.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsAggressionKey(),
        VariableDescription::Real, 4.4, 4.4));
        //VariableDescription::Real, 3.0, 7.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsWeightInfluenceKey(),
        VariableDescription::Real, 0.0, 0.0));
        //VariableDescription::Real, 0.0, 2.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsOutboundWeightingKey(),
        VariableDescription::Real, 0.0, 0.0));
        //VariableDescription::Real, 0.0, 2.0));
    desc->addVariable(
      new VariableDescription(ConfigOptions::getNetworkConflictsStubThroughWeightingKey(),
        // value of 5 here gives good results in test highway-008
        // value of 0, 0.5 here gives good results in test highway-011/010
        VariableDescription::Real, 0.59, 0.59));
        //VariableDescription::Real, 0.5, 1.0));

    shared_ptr<FitnessFunction> ff(new CaseFitnessFunction());
    SimulatedAnnealing sa(desc, ff);
//#error modify fitness function to give variable failure based on the number of reviews
    sa.setPickFromBestScores(true);
    sa.iterate(000);

    foreach (ConstStatePtr state, sa.getBestStates())
    {
      LOG_VARW(state);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflictsNetworkMatcherTest, "quick");

}
