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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>

#include <hoot/rnd/conflate/matching/ScoreMatchesDiff.h>

namespace hoot
{

/**
 * ScoreMatchesCmdTest is testing the basic diff functionality, so only testing some error handling
 * here.
 */
class ScoreMatchesDiffTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ScoreMatchesDiffTest);
  CPPUNIT_TEST(unsupportedInputFormatTest);
  // Validity of this test needs more investigation. See comments in ScoreMatchesDiff.
  //CPPUNIT_TEST(unequalManualMatchesTest);
  CPPUNIT_TEST(emptyDiffTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ScoreMatchesDiffTest() :
  HootTestFixture(
    "test-files/rnd/conflate/matching/ScoreMatchesDiffTest/",
    "test-output/rnd/conflate/matching/ScoreMatchesDiffTest/")
  {
  }

  void unsupportedInputFormatTest()
  {
    QString exceptionMsg("");
    ScoreMatchesDiff uut;

    try
    {
      uut.calculateDiff("input1.txt", "input2.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unsupported input format"));

    try
    {
      uut.calculateDiff("input1.osm", "input2.txt");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unsupported input format"));
  }

//  void unequalManualMatchesTest()
//  {
//    QString exceptionMsg("");
//    try
//    {
//      ScoreMatchesDiff().calculateDiff(
//        _inputPath + "ScoreMatchesDiffTest-unequalManualMatchesTest-1.osm",
//        _inputPath + "ScoreMatchesDiffTest-unequalManualMatchesTest-2.osm");
//    }
//    catch (const HootException& e)
//    {
//      exceptionMsg = e.what();
//    }
//    CPPUNIT_ASSERT(
//      exceptionMsg.startsWith("The two input datasets have a different number of manual matches"));
//  }

  void emptyDiffTest()
  {
    DisableLog dl;  // for the no diff warning
    // fails b/c no diff has been calc'd yet
    CPPUNIT_ASSERT(!ScoreMatchesDiff().printDiff("output"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ScoreMatchesDiffTest, "glacial");

}
