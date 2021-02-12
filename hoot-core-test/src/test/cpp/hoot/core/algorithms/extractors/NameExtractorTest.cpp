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
 * @copyright Copyright (C) 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/extractors/NameExtractor.h>
#include <hoot/core/elements/Node.h>

namespace hoot
{

class NameExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NameExtractorTest);
  CPPUNIT_TEST(runMeanLevenshteinTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * "Sheraton Sana'a Hotel" and "Sheraton hotel" should provide a perfect match. (we ignore the
   * extra "Sana'a" bit)
   */
  void runMeanLevenshteinTest()
  {
    NodePtr n1(new Node(Status::Invalid, -1, 0, 0, 0));
    n1->setTag("name", "Sheraton Sana'a Hotel");
    n1->setTag("alt_name", QString::fromUtf8("Funduq Shiratun;Funduq Shīrātūn;Hotel Sheraton San`a';Hotel Sheraton San‘ā’;fndq shyratwn;فندق شيراتون"));

    NodePtr n2(new Node(Status::Invalid, -2, 0, 0, 0));
    n2->setTag("name", QString::fromUtf8("شيراتون"));
    n2->setTag("name:ar", QString::fromUtf8("شيراتون"));
    n2->setTag("name:en", "Sheraton hotel");
    n2->setTag("alt_name", QString::fromUtf8("فندق شيراتون"));

    NameExtractor uut(StringDistancePtr(new MeanWordSetDistance(
                      StringDistancePtr(new LevenshteinDistance(1.5)))));

    HOOT_STR_EQUALS(1, uut.extract(n1, n2));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NameExtractorTest, "quick");

}
