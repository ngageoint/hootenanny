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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/visitors/DeriveImplicitTagsVisitor.h>


namespace hoot
{

class DeriveImplicitTagsVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DeriveImplicitTagsVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  void runBasicTest()
  {
    QString testJsonStr = QString::fromUtf8(
      "{                                      \n"
      " 'elements': [                         \n"
      " { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'Alshy Burgers' } },\n"
      " { 'type': 'node', 'id': -2, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Alshy Clinic' } },\n"
      " { 'type': 'node', 'id': -3, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'poi': 'yes', 'name': 'masjid' } },\n"
      " { 'type': 'node', 'id': -4, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'place': 'locality', 'name': 'Mustashfa alwhdt' } },\n"
      " { 'type': 'node', 'id': -5, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Sihhi' } },\n"
      " { 'type': 'node', 'id': -6, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Sihhi', 'alt_name': 'Clinic' } }\n"
      "]                                      \n"
      "}                                      \n");

    OsmMapPtr map = OsmJsonReader().loadFromString(testJsonStr);
    // the JSON parser doesn't handle exotic characters.
    map->getNode(-5)->getTags()["alt_name"] = QString::fromUtf8("Şiḩḩī");

    DeriveImplicitTagsVisitor uut;

    map->visitRw(uut);

//    LOG_VAR(TestUtils::toQuotedString(map->getNode(-1)->getTags().toString()));
//    LOG_VAR(TestUtils::toQuotedString(map->getNode(-2)->getTags().toString()));
//    LOG_VAR(TestUtils::toQuotedString(map->getNode(-3)->getTags().toString()));
//    LOG_VAR(TestUtils::toQuotedString(map->getNode(-4)->getTags().toString()));
//    LOG_VAR(TestUtils::toQuotedString(map->getNode(-5)->getTags().toString()));
//    LOG_VAR(TestUtils::toQuotedString(map->getNode(-6)->getTags().toString()));

    HOOT_STR_EQUALS("name = Alshy Burgers\n"
                    "amenity = pub\n",
                    map->getNode(-1)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: alshy\n"
                    "name = Alshy Clinic\n"
                    "amenity = clinic\n",
                    map->getNode(-2)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: masjid\n"
                    "poi = yes\n"
                    "religion = muslim\n"
                    "name = masjid\n"
                    "amenity = place_of_worship\n",
                    map->getNode(-3)->getTags());
    HOOT_STR_EQUALS("place = locality\n"
                    "note = Found multiple possible matches for implicit tags: alwhdt, mustashfa\n"
                    "name = Mustashfa alwhdt\n",
                    map->getNode(-4)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: sihhi, şiḩḩī\n"
                    "alt_name = Şiḩḩī\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-5)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: sihhi\n"
                    "alt_name = Clinic\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-6)->getTags());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DeriveImplicitTagsVisitorTest, "quick");

}
