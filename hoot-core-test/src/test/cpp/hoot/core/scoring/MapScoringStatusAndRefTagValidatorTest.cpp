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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/scoring/MapScoringStatusAndRefTagValidator.h>
using namespace hoot;

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

// Standard
#include <stdio.h>

namespace hoot
{

class MapScoringStatusAndRefTagValidatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MapScoringStatusAndRefTagValidatorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /*
   * This test makes sure data loaded with an Unknown1 status doesn't have any REF2 tags in it and
   * that data loaded with an Unknown2 status doesn't have any REF1 tags in it.
   */
  void runTest()
  {
    OsmMap::resetCounters();
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map1(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref1-tags-only.osm", map1);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref2-tags-only.osm", map1);
    CPPUNIT_ASSERT(MapScoringStatusAndRefTagValidator::allTagsAreValid(map1));

    boost::shared_ptr<OsmMap> map2(new OsmMap());
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref1-tags-only.osm", map2);
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref2-tags-only.osm", map2);
    CPPUNIT_ASSERT(!MapScoringStatusAndRefTagValidator::allTagsAreValid(map2));

    boost::shared_ptr<OsmMap> map3(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref1-tags-only.osm", map3);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref1-tags-only.osm", map3);
    CPPUNIT_ASSERT(!MapScoringStatusAndRefTagValidator::allTagsAreValid(map3));

    boost::shared_ptr<OsmMap> map4(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref2-tags-only.osm", map4);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(
      "test-files/scoring/MapScoringStatusAndRefTagValidatorTest/has-ref2-tags-only.osm", map4);
    CPPUNIT_ASSERT(!MapScoringStatusAndRefTagValidator::allTagsAreValid(map4));
  }

};

}

CPPUNIT_TEST_SUITE_REGISTRATION(MapScoringStatusAndRefTagValidatorTest);




