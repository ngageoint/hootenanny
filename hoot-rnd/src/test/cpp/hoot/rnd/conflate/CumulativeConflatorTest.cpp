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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/conflate/CumulativeConflator.h>

// Qt
#include <QDir>

namespace hoot
{

class CumulativeConflatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CumulativeConflatorTest);
  CPPUNIT_TEST(basicTest);
  //TODO: more error handling tests
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    TestUtils::resetEnvironment();
    OsmMap::resetCounters();
    Settings::getInstance().set(ConfigOptions::getReviewTagsTreatAsMetadataKey(), false);
    Settings::getInstance().set(
      ConfigOptions::getTagMergerDefaultKey(), "hoot::ProvenanceAwareOverwriteTagMerger");

    QStringList inputs;
    inputs.append("test-files/conflate/CumulativeConflatorTest/OakLabsRestaurants_RioSubsetCopy.osm");
    inputs.append("test-files/conflate/CumulativeConflatorTest/Restaurants_RioSource2.osm");
    inputs.append("test-files/conflate/CumulativeConflatorTest/Restaurants_RioSource3.osm");

    QDir().mkpath("test-output/conflate/");
    CumulativeConflator::conflate(inputs, "test-output/conflate/CumulativeConflatorTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/CumulativeConflatorTest/CumulativeConflatorTest.osm",
      "test-output/conflate/CumulativeConflatorTest.osm");

    TestUtils::resetEnvironment();
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CumulativeConflatorTest, "slow");

}
