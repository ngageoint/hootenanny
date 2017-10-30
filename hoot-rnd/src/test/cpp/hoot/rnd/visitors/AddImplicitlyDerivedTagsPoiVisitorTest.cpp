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
#include <hoot/rnd/visitors/AddImplicitlyDerivedTagsPoiVisitor.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteRecordWriter.h>

// Qt
#include <QDir>

namespace hoot
{

class AddImplicitlyDerivedTagsPoiVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(AddImplicitlyDerivedTagsPoiVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  static QString outDir() { return "test-output/io/AddImplicitlyDerivedTagsPoiVisitorTest"; }

  void runBasicTest()
  {
    QDir().mkpath(outDir());
    const QString databaseFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runBasicTest-rules.sqlite";
    //use this to regenerate the db file
//    ImplicitTagRulesSqliteRecordWriter writer;
//    writer.open(databaseFile);
//    writer.write("test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest/ruleWordParts");
//    writer.close();
//    return;

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
      "   'tags': { 'place': 'locality', 'name': 'alwhdt Mustashfa' } },\n"
      " { 'type': 'node', 'id': -5, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Sihhi' } },\n"
      " { 'type': 'node', 'id': -6, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Sihhi', 'alt_name': 'Clinic' } },\n"
      " { 'type': 'node', 'id': -7, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'masjid mosque' } },\n"
      " { 'type': 'node', 'id': -8, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Mustashfa alwhdt' } },\n"
      " { 'type': 'node', 'id': -9, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'alwhdt' } }\n"
      "]                                      \n"
      "}                                      \n");

    OsmMapPtr map = OsmJsonReader().loadFromString(testJsonStr);
    // the JSON parser doesn't handle exotic characters.
    map->getNode(-5)->getTags()["alt_name"] = QString::fromUtf8("Şiḩḩī");

    AddImplicitlyDerivedTagsPoiVisitor uut(databaseFile);

    map->visitRw(uut);

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
                    "note = Found multiple possible matches for implicit tags: alwhdt, Mustashfa\n"
                    "name = alwhdt Mustashfa\n",
                    map->getNode(-4)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: Sihhi, Şiḩḩī\n"
                    "alt_name = Şiḩḩī\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-5)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: Sihhi\n"
                    "alt_name = Clinic\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-6)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: masjid, mosque\n"
                    "religion = muslim\n"
                    "name = masjid mosque\n"
                    "amenity = place_of_worship\n",
                    map->getNode(-7)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: Mustashfa alwhdt\n"
                    "name = Mustashfa alwhdt\n"
                    "amenity = hospital\n",
                    map->getNode(-8)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: alwhdt\n"
                    "name = alwhdt\n"
                    "amenity = clinic\n",
                    map->getNode(-9)->getTags());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddImplicitlyDerivedTagsPoiVisitorTest, "quick");

}
