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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteWriter.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/visitors/ImplicitPoiTypeTagger.h>

namespace hoot
{

/*
 * The tests in this class will fail with any translator other than the current default,
 * ToEnglishDictionaryTranslator.
 */
class ImplicitPoiTypeTaggerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitPoiTypeTaggerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runDuplicateTagKeyTest);
  CPPUNIT_TEST(runLessSpecificImplicitTagTest);
  CPPUNIT_TEST(runMoreSpecificImplicitTagTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ImplicitPoiTypeTaggerTest()
    : HootTestFixture("test-files/visitors/ImplicitPoiTypeTaggerTest/",
                      "test-output/visitors/ImplicitPoiTypeTaggerTest/")
  {
    setResetType(ResetAll);
  }

  void runBasicTest()
  {
    //regenerate the db file
    const QString databaseOutFile =
      _outputPath + "ImplicitPoiTypeTaggerTest-runBasicTest-rules.sqlite";
    ImplicitTagRulesSqliteWriter writer;
    writer.open(databaseOutFile);
    writer.write(_inputPath + "runBasicTest-ruleWordParts");
    writer.close();

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

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmJsonReader().loadFromString(testJsonStr, map);

    // the JSON parser doesn't handle exotic characters
    map->getNode(-5)->getTags()["alt_name"] = QString::fromUtf8("Şiḩḩī");

    ImplicitPoiTypeTagger uut(databaseOutFile);
    uut.setAddTopTagOnly(false);
    uut.setAllowWordsInvolvedInMultipleRules(false);
    uut.setAllowTaggingSpecificFeatures(true);
    uut.setMatchEndOfNameSingleTokenFirst(true);
    uut.setTranslateNamesToEnglish(true);
    uut._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    map->visitRw(uut);

    HOOT_STR_EQUALS("amenity = pub\n"
                    "name = Alshy Burgers\n",
                    map->getNode(-1)->getTags());
    HOOT_STR_EQUALS("amenity = clinic\n"
                    "name = Alshy Clinic\n"
                    "hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: alshy; tags added: amenity = clinic\n",
                    map->getNode(-2)->getTags());
    HOOT_STR_EQUALS("amenity = place_of_worship\n"
                    "name = masjid\n"
                    "religion = muslim\n"
                    "poi = yes\n"
                    "hoot:implicitTags:tagsAdded = Added 2 implicitly derived tag(s) based on: mosque; tags added: amenity = place_of_worship, religion = muslim\n",
                    map->getNode(-3)->getTags());
    HOOT_STR_EQUALS("amenity = place_of_worship\n"
                    "name = masjid\n"
                    "religion = muslim\n"
                    "poi = yes\n"
                    "hoot:implicitTags:tagsAdded = Added 2 implicitly derived tag(s) based on: mosque; tags added: amenity = place_of_worship, religion = muslim\n",
                    map->getNode(-3)->getTags());
    HOOT_STR_EQUALS("amenity = hospital\n"
                    "name = alwhdt Mustashfa\n"
                    "place = locality\n"
                    "hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: Mustashfa; tags added: amenity = hospital\n",
                    map->getNode(-4)->getTags());
    HOOT_STR_EQUALS("amenity = clinic\n"
                    "name = Sihhi\n"
                    "alt_name = Şiḩḩī\n"
                    "hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: sihhi; tags added: amenity = clinic\n",
                    map->getNode(-5)->getTags());
    HOOT_STR_EQUALS("amenity = clinic\n"
                    "name = Sihhi\n"
                    "alt_name = Clinic\n"
                    "hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: Sihhi; tags added: amenity = clinic\n",
                    map->getNode(-6)->getTags());
    HOOT_STR_EQUALS("amenity = place_of_worship\n"
                    "name = masjid mosque\n"
                    "religion = muslim\n"
                    "hoot:implicitTags:tagsAdded = Added 2 implicitly derived tag(s) based on: mosque; tags added: amenity = place_of_worship, religion = muslim\n",
                    map->getNode(-7)->getTags());
    HOOT_STR_EQUALS("amenity = hospital\n"
                    "name = Mustashfa alwhdt\n"
                    "hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: Mustashfa alwhdt; tags added: amenity = hospital\n",
                    map->getNode(-8)->getTags());
    HOOT_STR_EQUALS("amenity = clinic\n"
                    "name = alwhdt\n"
                    "hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: alwhdt; tags added: amenity = clinic\n",
                    map->getNode(-9)->getTags());

  }

  void runDuplicateTagKeyTest()
  {
    //regenerate the db file
    const QString databaseOutFile =
      _outputPath + "ImplicitPoiTypeTaggerTest-runDuplicateTagKeyTest-rules.sqlite";
    ImplicitTagRulesSqliteWriter writer;
    writer.open(databaseOutFile);
    writer.write(_inputPath + "runDuplicateTagKeyTest-ruleWordParts");
    writer.close();

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node = std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0);
    //Even though this node has school in the name and could likely be a school, since it already
    //is tagged at a bank, we don't want to risk introducing a false positive tag...so don't add
    //amenity=school to it.
    node->getTags()["name"] = "school";
    node->getTags()["amenity"] = "bank";
    map->addNode(node);

    ImplicitPoiTypeTagger uut(databaseOutFile);
    uut.setAddTopTagOnly(false);
    uut.setAllowWordsInvolvedInMultipleRules(false);
    uut.setAllowTaggingSpecificFeatures(true);
    uut.setMatchEndOfNameSingleTokenFirst(true);
    uut.setTranslateNamesToEnglish(true);
    uut._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    map->visitRw(uut);

    CPPUNIT_ASSERT_EQUAL(2, map->getNode(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL(
      QString("school").toStdString(), map->getNode(1)->getTags()["name"].toStdString());
    CPPUNIT_ASSERT_EQUAL(
      QString("bank").toStdString(), map->getNode(1)->getTags()["amenity"].toStdString());
    CPPUNIT_ASSERT(!map->getNode(1)->getTags().contains("hoot:implicitTags:tagsAdded"));
  }

  void runLessSpecificImplicitTagTest()
  {
    //regenerate the db file
    const QString databaseOutFile =
      _outputPath + "ImplicitPoiTypeTaggerTest-runLessSpecificImplicitTagTest-rules.sqlite";
    ImplicitTagRulesSqliteWriter writer;
    writer.open(databaseOutFile);
    writer.write(_inputPath + "runLessSpecificImplicitTagTest-ruleWordParts");
    writer.close();

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node = std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0);
    //This node has a more specific amenity tag than the one in the rules file, so the node should
    //keep the amenity tag it starts with.
    node->getTags()["name"] = "hall";
    node->getTags()["amenity"] = "public_hall";
    map->addNode(node);

    ImplicitPoiTypeTagger uut(databaseOutFile);
    uut.setAddTopTagOnly(false);
    uut.setAllowWordsInvolvedInMultipleRules(false);
    uut.setAllowTaggingSpecificFeatures(true);
    uut.setMatchEndOfNameSingleTokenFirst(true);
    uut.setTranslateNamesToEnglish(true);
    uut._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    map->visitRw(uut);

    CPPUNIT_ASSERT_EQUAL(2, map->getNode(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL(
      QString("hall").toStdString(), map->getNode(1)->getTags()["name"].toStdString());
    CPPUNIT_ASSERT_EQUAL(
      QString("public_hall").toStdString(), map->getNode(1)->getTags()["amenity"].toStdString());
    CPPUNIT_ASSERT(!map->getNode(1)->getTags().contains("hoot:implicitTags:tagsAdded"));
  }

  void runMoreSpecificImplicitTagTest()
  {
    //regenerate the db file
    const QString databaseOutFile =
      _outputPath + "ImplicitPoiTypeTaggerTest-runMoreSpecificImplicitTagTest-rules.sqlite";
    ImplicitTagRulesSqliteWriter writer;
    writer.open(databaseOutFile);
    writer.write(_inputPath + "runMoreSpecificImplicitTagTest-ruleWordParts");
    writer.close();

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node = std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0);
    //The amenity tag referenced in the rules file is more specific than the one this node has, so
    //the node's amenity tag should be replaced by the one in the rules file.
    node->getTags()["name"] = "hall";
    node->getTags()["amenity"] = "hall";
    map->addNode(node);

    ImplicitPoiTypeTagger uut(databaseOutFile);
    uut.setAddTopTagOnly(false);
    uut.setAllowWordsInvolvedInMultipleRules(false);
    uut.setAllowTaggingSpecificFeatures(true);
    uut.setMatchEndOfNameSingleTokenFirst(true);
    uut.setTranslateNamesToEnglish(true);
    uut._translator = std::make_shared<ToEnglishDictionaryTranslator>();
    map->visitRw(uut);
    LOG_VART(map->getNode(1)->getTags());

    CPPUNIT_ASSERT_EQUAL(3, map->getNode(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL(
      QString("hall").toStdString(), map->getNode(1)->getTags()["name"].toStdString());
    CPPUNIT_ASSERT_EQUAL(
      QString("public_hall").toStdString(), map->getNode(1)->getTags()["amenity"].toStdString());
    CPPUNIT_ASSERT(map->getNode(1)->getTags().contains("hoot:implicitTags:tagsAdded"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitPoiTypeTaggerTest, "quick");

}
