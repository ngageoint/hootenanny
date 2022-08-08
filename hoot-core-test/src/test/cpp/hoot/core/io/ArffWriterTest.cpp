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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/io/ArffWriter.h>
#include <hoot/core/scoring/MatchFeatureExtractor.h>

using namespace std;

namespace hoot
{

class ArffWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ArffWriterTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ArffWriterTest()
    : HootTestFixture("test-files/io/ArffWriterTest/",
                      "test-output/io/ArffWriterTest/")
  {
  }

  void runBasicTest()
  {
    vector<ArffWriter::Sample> samples;

    ArffWriter::Sample s;
    s["class"] = MatchType::Match;
    s["a"] = 1.1;
    s["b"] = 1.2;
    samples.push_back(s);
    s["class"] = MatchType::Miss;
    s["a"] = 2.1;
    s["b"] = 2.2;
    s["c"] = 2.3;
    samples.push_back(s);
    s.clear();
    s["class"] = MatchType::Review;
    s["b"] = 3.2;
    samples.push_back(s);

    ArffWriter uut(_outputPath + "runBasic.arff", true);
    uut.write(samples);

    // check for consistency with previous versions.
    HOOT_FILE_EQUALS(_inputPath + "runBasic.arff", _outputPath + "runBasic.arff");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ArffWriterTest, "quick");

}
