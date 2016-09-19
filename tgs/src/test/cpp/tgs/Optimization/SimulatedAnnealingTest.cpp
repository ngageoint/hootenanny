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

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard Includes
#include <math.h>

// TGS Includes
#include <tgs/Optimization/SimulatedAnnealing.h>

namespace Tgs
{
using namespace std;

class SimulatedAnnealingTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SimulatedAnnealingTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST_SUITE_END();
public:
  class TestFitnessFunction : public FitnessFunction
  {
    virtual double f(const ConstStatePtr& s) const
    {
      double w = s->get("w");
      double x = s->get("x");
      double y = s->get("y");
      double z = s->get("z");

      // w [0, 4] optimal = 3
      // x [0, 1] optimal ~ .852
      // y [0, 1] optimal ~ 5.85
      // z [0, 3] optimal ~ 1.54
      double v = sin(x * .4 + 1.23) * sin(y * .2 + .4) * sin(z * .5 + .8) *
        (w == 3 ? 1.0 : 0.5);

      double result = 5.0 / std::max(1e-9, v);
      //cout << "v: " << v << " score: " << 5.0 / max(1e-9, v) << endl;

      return result;
    }
  };

  void simpleTest()
  {
    StateDescriptionPtr sd(new StateDescription());
    sd->addVariable(new VariableDescription("w", VariableDescription::Integer, 0, 4));
    sd->addVariable(new VariableDescription("x", VariableDescription::Real, 0, 1));
    sd->addVariable(new VariableDescription("y", VariableDescription::Real, 0, 7));
    sd->addVariable(new VariableDescription("z", VariableDescription::Real, 0, 3));
    SimulatedAnnealing uut(sd, boost::shared_ptr<TestFitnessFunction>(new TestFitnessFunction()));
    CPPUNIT_ASSERT(uut.iterate(100) < 5.5);
    CPPUNIT_ASSERT_EQUAL(uut.getBestStates().size(), 1);
    ConstStatePtr state = *uut.getBestStates().begin();
    CPPUNIT_ASSERT_EQUAL(state->getInt("w"), 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(.852, state->get("x"), .1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.85, state->get("y"), .7);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.54, state->get("z"), .3);
//    foreach (ConstStatePtr state, uut.getBestStates())
//    {
//      std::cout << state->toString().toUtf8().data() << std::endl;
//    }
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::SimulatedAnnealingTest, "quick");
