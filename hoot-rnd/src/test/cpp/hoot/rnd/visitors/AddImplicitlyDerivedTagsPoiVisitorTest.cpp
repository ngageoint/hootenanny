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
  CPPUNIT_TEST(runDuplicateTagKeyTest);
  CPPUNIT_TEST(runLessSpecificImplicitTagTest);
  CPPUNIT_TEST(runMoreSpecificImplicitTagTest);
  CPPUNIT_TEST(runWordIgnoreTest);
  CPPUNIT_TEST(runTagIgnoreTest);
  CPPUNIT_TEST(runRuleIgnoreTest);
  //TODO: top tag test
  CPPUNIT_TEST_SUITE_END();

public:

  //TODO: use these throughout test
  static QString inDir() { return "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest"; }
  static QString outDir() { return "test-output/visitors/AddImplicitlyDerivedTagsPoiVisitorTest"; }

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  void runBasicTest()
  {
    QDir().mkpath(outDir());

    //TODO: don't regen this database every time
    //use this to regenerate the db file
    const QString databaseOutFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runBasicTest-rules.sqlite";
    ImplicitTagRulesSqliteRecordWriter writer;
    writer.open(databaseOutFile);
    writer.write(
      "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest/runBasicTest-ruleWordParts");
    writer.close();
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

    //const QString databaseInFile =
      //"test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest-runBasicTest-rules.sqlite";
    AddImplicitlyDerivedTagsPoiVisitor uut(/*databaseInFile*/databaseOutFile);
    Settings conf;
    conf.set("poi.implicit.tag.rules.rule.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.file", "");
    conf.set("poi.implicit.tag.rules.word.ignore.file", "");
    conf.set("poi.implicit.tag.rules.custom.rule.file", "");
    uut.setConfiguration(conf);
    map->visitRw(uut);

    HOOT_STR_EQUALS("name = Alshy Burgers\n"
                    "amenity = pub\n",
                    map->getNode(-1)->getTags());
    HOOT_STR_EQUALS("hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: alshy; tags added: amenity = clinic\n"
                    "name = Alshy Clinic\n"
                    "amenity = clinic\n",
                    map->getNode(-2)->getTags());
    HOOT_STR_EQUALS("hoot:implicitTags:tagsAdded = Added 2 implicitly derived tag(s) based on: masjid; tags added: religion = muslim, amenity = place_of_worship\n"
                    "religion = muslim\n"
                    "name = masjid\n"
                    "amenity = place_of_worship\n",
                    map->getNode(-3)->getTags());
    HOOT_STR_EQUALS("place = locality\n"
                    "hoot:implicitTags:multipleRules = No implicit tags added due to finding multiple possible matches for implicit tags: alwhdt, Mustashfa\n"
                    "name = alwhdt Mustashfa\n",
                    map->getNode(-4)->getTags());
    HOOT_STR_EQUALS("hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: Sihhi, Şiḩḩī; tags added: amenity = clinic\n"
                    "alt_name = Şiḩḩī\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-5)->getTags());
    HOOT_STR_EQUALS("hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: Sihhi; tags added: amenity = clinic\n"
                    "alt_name = Clinic\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-6)->getTags());
    HOOT_STR_EQUALS("hoot:implicitTags:tagsAdded = Added 2 implicitly derived tag(s) based on: masjid, mosque; tags added: religion = muslim, amenity = place_of_worship\n"
                    "religion = muslim\n"
                    "name = masjid mosque\n"
                    "amenity = place_of_worship\n",
                    map->getNode(-7)->getTags());
    HOOT_STR_EQUALS("hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: Mustashfa alwhdt; tags added: amenity = hospital\n"
                    "name = Mustashfa alwhdt\n"
                    "amenity = hospital\n",
                    map->getNode(-8)->getTags());
    HOOT_STR_EQUALS("hoot:implicitTags:tagsAdded = Added 1 implicitly derived tag(s) based on: alwhdt; tags added: amenity = clinic\n"
                    "name = alwhdt\n"
                    "amenity = clinic\n",
                    map->getNode(-9)->getTags());
  }

  void runDuplicateTagKeyTest()
  {
    QDir().mkpath(outDir());

    //TODO: don't regen this database every time
    //use this to regenerate the db file
    const QString databaseOutFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runDuplicateTagKeyTest-rules.sqlite";
    ImplicitTagRulesSqliteRecordWriter writer;
    writer.open(databaseOutFile);
    writer.write(
      "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest/runDuplicateTagKeyTest-ruleWordParts");
    writer.close();
  //    return;

    OsmMapPtr map(new OsmMap());
    NodePtr node(new Node(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0));
    //Even though this node has school in the name and could likely be a school, since it already
    //is tagged at a bank, we don't want to risk introducing a false positive tag...so don't add
    //amenity=school to it.
    node->getTags()["name"] = "school";
    node->getTags()["amenity"] = "bank";
    map->addNode(node);

    //const QString databaseInFile =
      //"test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest-runBasicTest-rules.sqlite";
    AddImplicitlyDerivedTagsPoiVisitor uut(/*databaseInFile*/databaseOutFile);
    Settings conf;
    conf.set("poi.implicit.tag.rules.rule.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.file", "");
    conf.set("poi.implicit.tag.rules.word.ignore.file", "");
    conf.set("poi.implicit.tag.rules.custom.rule.file", "");
    uut.setConfiguration(conf);
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
    QDir().mkpath(outDir());

    //TODO: don't regen this database every time
    //use this to regenerate the db file
    const QString databaseOutFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runLessSpecificImplicitTagTest-rules.sqlite";
    ImplicitTagRulesSqliteRecordWriter writer;
    writer.open(databaseOutFile);
    writer.write(
      "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest/runLessSpecificImplicitTagTest-ruleWordParts");
    writer.close();
  //    return;

    OsmMapPtr map(new OsmMap());
    NodePtr node(new Node(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0));
    //This node has a more specific amenity tag than the one in the rules file, so the node should
    //keep the amenity tag it starts with.
    node->getTags()["name"] = "hall";
    node->getTags()["amenity"] = "public_hall";
    map->addNode(node);

    //const QString databaseInFile =
      //"test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest-runLessSpecificImplicitTagTest-rules.sqlite";
    AddImplicitlyDerivedTagsPoiVisitor uut(/*databaseInFile*/databaseOutFile);
    Settings conf;
    conf.set("poi.implicit.tag.rules.rule.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.file", "");
    conf.set("poi.implicit.tag.rules.word.ignore.file", "");
    conf.set("poi.implicit.tag.rules.custom.rule.file", "");
    uut.setConfiguration(conf);
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
    QDir().mkpath(outDir());

    //TODO: don't regen this database every time
    //use this to regenerate the db file
    const QString databaseOutFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runMoreSpecificImplicitTagTest-rules.sqlite";
    ImplicitTagRulesSqliteRecordWriter writer;
    writer.open(databaseOutFile);
    writer.write(
      "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest/runMoreSpecificImplicitTagTest-ruleWordParts");
    writer.close();
  //    return;

    OsmMapPtr map(new OsmMap());
    NodePtr node(new Node(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0));
    //The amenity tag referenced in the rules file is more specific than the one this node has, so
    //the node's amenity tag should be replaced by the one in the rules file.
    node->getTags()["name"] = "hall";
    node->getTags()["amenity"] = "hall";
    map->addNode(node);

    //const QString databaseInFile =
      //"test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest-runMoreSpecificImplicitTagTest-rules.sqlite";
    AddImplicitlyDerivedTagsPoiVisitor uut(/*databaseInFile*/databaseOutFile);
    Settings conf;
    conf.set("poi.implicit.tag.rules.rule.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.file", "");
    conf.set("poi.implicit.tag.rules.word.ignore.file", "");
    conf.set("poi.implicit.tag.rules.custom.rule.file", "");
    uut.setConfiguration(conf);
    map->visitRw(uut);
    LOG_VART(map->getNode(1)->getTags());

    CPPUNIT_ASSERT_EQUAL(3, map->getNode(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL(
      QString("hall").toStdString(), map->getNode(1)->getTags()["name"].toStdString());
    CPPUNIT_ASSERT_EQUAL(
      QString("public_hall").toStdString(), map->getNode(1)->getTags()["amenity"].toStdString());
    CPPUNIT_ASSERT(map->getNode(1)->getTags().contains("hoot:implicitTags:tagsAdded"));
  }

  void runWordIgnoreTest()
  {
    QDir().mkpath(outDir());

    //TODO: don't regen this database every time
    //use this to regenerate the db file
    const QString databaseOutFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runWordIgnoreTest-rules.sqlite";
    ImplicitTagRulesSqliteRecordWriter writer;
    writer.open(databaseOutFile);
    writer.write(
      "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest/runWordIgnoreTest-ruleWordParts");
    writer.close();
  //    return;

    OsmMapPtr map(new OsmMap());
    NodePtr node(new Node(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0));
    node->getTags()["name"] = "hall";
    map->addNode(node);

    //const QString databaseInFile =
      //"test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest-runWordIgnoreTest-rules.sqlite";
    AddImplicitlyDerivedTagsPoiVisitor uut(/*databaseInFile*/databaseOutFile);
    Settings conf;
    conf.set("poi.implicit.tag.rules.rule.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.file", "");
    conf.set("poi.implicit.tag.rules.word.ignore.file", inDir() + "/wordIgnoreList");
    conf.set("poi.implicit.tag.rules.custom.rule.file", "");
    uut.setConfiguration(conf);
    map->visitRw(uut);
    LOG_VART(map->getNode(1)->getTags());

    CPPUNIT_ASSERT_EQUAL(1, map->getNode(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL(
      QString("hall").toStdString(), map->getNode(1)->getTags()["name"].toStdString());
  }

  void runTagIgnoreTest()
  {
    QDir().mkpath(outDir());

    //TODO: don't regen this database every time
    //use this to regenerate the db file
    const QString databaseOutFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runTagIgnoreTest-rules.sqlite";
    ImplicitTagRulesSqliteRecordWriter writer;
    writer.open(databaseOutFile);
    writer.write(
      "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest/runTagIgnoreTest-ruleWordParts");
    writer.close();
  //    return;

    OsmMapPtr map(new OsmMap());
    NodePtr node(new Node(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0));
    node->getTags()["name"] = "hall";
    map->addNode(node);

    //const QString databaseInFile =
      //"test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest-runTagIgnoreTest-rules.sqlite";
    AddImplicitlyDerivedTagsPoiVisitor uut(/*databaseInFile*/databaseOutFile);
    Settings conf;
    conf.set("poi.implicit.tag.rules.rule.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.ignore.file", inDir() + "/tagIgnoreList");
    conf.set("poi.implicit.tag.rules.tag.file", "");
    conf.set("poi.implicit.tag.rules.word.ignore.file", "");
    conf.set("poi.implicit.tag.rules.custom.rule.file", "");
    uut.setConfiguration(conf);
    map->visitRw(uut);
    LOG_VART(map->getNode(1)->getTags());

    CPPUNIT_ASSERT_EQUAL(1, map->getNode(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL(
      QString("hall").toStdString(), map->getNode(1)->getTags()["name"].toStdString());
  }

  void runRuleIgnoreTest()
  {
    QDir().mkpath(outDir());

    //TODO: don't regen this database every time
    //use this to regenerate the db file
    const QString databaseOutFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runRuleIgnoreTest-rules.sqlite";
    ImplicitTagRulesSqliteRecordWriter writer;
    writer.open(databaseOutFile);
    writer.write(
      "test-files/visitors/AddImplicitlyDerivedTagsPoiVisitorTest/runRuleIgnoreTest-ruleWordParts");
    writer.close();
  //    return;

    OsmMapPtr map(new OsmMap());
    NodePtr node(new Node(Status::Unknown1, 1, geos::geom::Coordinate(1, 1), 15.0));
    node->getTags()["name"] = "hall";
    map->addNode(node);

    //const QString databaseInFile =
      //"test-files/io/AddImplicitlyDerivedTagsPoiVisitorTest-runRuleIgnoreTest-rules.sqlite";
    AddImplicitlyDerivedTagsPoiVisitor uut(/*databaseInFile*/databaseOutFile);
    Settings conf;
    conf.set("poi.implicit.tag.rules.rule.ignore.file", inDir() + "/ruleIgnoreList");
    conf.set("poi.implicit.tag.rules.tag.ignore.file", "");
    conf.set("poi.implicit.tag.rules.tag.file", "");
    conf.set("poi.implicit.tag.rules.word.ignore.file", "");
    conf.set("poi.implicit.tag.rules.custom.rule.file", "");
    uut.setConfiguration(conf);
    map->visitRw(uut);
    LOG_VART(map->getNode(1)->getTags());

    CPPUNIT_ASSERT_EQUAL(1, map->getNode(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL(
      QString("hall").toStdString(), map->getNode(1)->getTags()["name"].toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddImplicitlyDerivedTagsPoiVisitorTest, "quick");

}
