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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/conflate/CumulativeConflator.h>

namespace hoot
{

class CumulativeConflatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(CumulativeConflatorTest);
  //CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  CumulativeConflatorTest()
    : HootTestFixture("test-files/rnd/conflate/CumulativeConflatorTest/",
                      "test-output/rnd/conflate/CumulativeConflatorTest/")
  {
    setResetType(ResetAll);
  }

  void basicTest()
  {
    Settings::getInstance().set(ConfigOptions::getWriterCleanReviewTagsKey(), false);
    Settings::getInstance().set(
      ConfigOptions::getTagMergerDefaultKey(), "hoot::ProvenanceAwareOverwriteTagMerger");

    QStringList inputs;
    inputs.append(_inputPath + "OakLabsRestaurants_RioSubsetCopy.osm");
    inputs.append(_inputPath + "Restaurants_RioSource2.osm");
    inputs.append(_inputPath + "Restaurants_RioSource3.osm");

    CumulativeConflator::conflate(inputs, _outputPath + "CumulativeConflatorTest.osm");

    HOOT_FILE_EQUALS(
      _inputPath + "CumulativeConflatorTest.osm",
      _outputPath + "CumulativeConflatorTest.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CumulativeConflatorTest, "glacial");

}
