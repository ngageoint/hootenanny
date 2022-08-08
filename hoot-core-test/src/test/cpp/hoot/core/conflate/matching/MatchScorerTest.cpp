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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchScorer.h>

namespace hoot
{

/*
 * This is testing error handling cases only right now, as ScoreMatchesCmdTest handles the rest.
 */
class MatchScorerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MatchScorerTest);
  CPPUNIT_TEST(runOptimizeWithOutputTest);
  CPPUNIT_TEST(runUnequalInputsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MatchScorerTest() : HootTestFixture()
  {
  }

  void runOptimizeWithOutputTest()
  {
    MatchScorer uut;
    uut.setOptimizeThresholds(true);

    QString exceptionMsg;
    try
    {
      // Inputs being populated and the actual value of the output don't matter here...only that the
      // output was populated with anything.
      uut.score(QStringList(), QStringList(), "blah");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS("Output path not valid when threshold optimization is enabled.", exceptionMsg);
  }

  void runUnequalInputsTest()
  {
    MatchScorer uut;
    QStringList ref1Inputs;
    ref1Inputs.append("something");
    ref1Inputs.append("something else");
    QStringList ref2Inputs;
    ref2Inputs.append("another thing");

    QString exceptionMsg;
    try
    {
      // Value of the populated inputs being populated and the actual value of the output don't
      // matter here...only that the inputs aren't of the same size.
      uut.score(ref1Inputs, ref2Inputs, "blah");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "The number of REF1 inputs: 2 does not equal the number of REF2 inputs: 1.", exceptionMsg);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchScorerTest, "quick");

}

