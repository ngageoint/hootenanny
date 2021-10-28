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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/schema/OsmSchema.h>

const double epsilon = 1e-6;

using namespace geos::geom;
using namespace std;

namespace hoot
{

class OsmSchemaTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmSchemaTest);
  CPPUNIT_TEST(averageTest);
  CPPUNIT_TEST(categoryTest);
  CPPUNIT_TEST(loadTest);
  CPPUNIT_TEST(commonAncestorTest);
  CPPUNIT_TEST(distanceTest);
  CPPUNIT_TEST(getChildTagsTest);
  CPPUNIT_TEST(getSimilarTagsTest);
  CPPUNIT_TEST(getTagTest);
  CPPUNIT_TEST(isAncestorTest);
  CPPUNIT_TEST(isMetaDataTest);
  CPPUNIT_TEST(religionTest);
  CPPUNIT_TEST(isGenericTest);
  CPPUNIT_TEST(isGenericKvpTest);
  CPPUNIT_TEST(hasTypeTest);
  CPPUNIT_TEST(hasMoreThanOneTypeTest);
  CPPUNIT_TEST(getFirstTypeTest);
  CPPUNIT_TEST(explicitTypeMismatchTest);
  CPPUNIT_TEST(mostSpecificTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Test calculating the average between two tags with weights.
   */
  void averageTest()
  {
    OsmSchema uut;
    uut.createTestingGraph();

    double score;
    QString avg;
    avg = uut.average("highway=primary", "highway=residential", score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.64, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(avg.toStdString(), std::string("highway=secondary"));

    avg = uut.average("highway=Primary", "Highway=residential", score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.64, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(avg.toStdString(), std::string("highway=secondary"));

    avg = uut.average("highway=road", "highway=secondary", score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(avg.toStdString(), std::string("highway=secondary"));

    avg = uut.average("highway=primary", 1, "highway=service", 1, score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.512, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(std::string("highway=secondary"), avg.toStdString());

    avg = uut.average("highway=primary", 1, "highway=service", 2, score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.512, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(avg.toStdString(), std::string("highway=residential"));

    avg = uut.average("highway=primary", 1, "highway=service", 5, score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4096, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(avg.toStdString(), std::string("highway=service"));

    avg = uut.average("highway=primary", 5, "highway=service", 1, score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4096, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(avg.toStdString(), std::string("highway=primary"));

    avg = uut.average("amenity=bar", 5, "amenity=cafe", 1, score);
    HOOT_STR_EQUALS("amenity=bar", avg.toStdString());

    // when averaging a non-specific tag w/ a tag we know about always return the tag we know.
    avg = uut.average("leisure=badvalue", 5, "leisure=track", 1, score);
    HOOT_STR_EQUALS("leisure=track", avg.toStdString());

    avg = uut.average("leisure=track", 5, "leisure=badvalue", 1, score);
    HOOT_STR_EQUALS("leisure=track", avg.toStdString());

    // when averaging two non-specific tags return the first tag.
    avg = uut.average("leisure=badvalue1", 5, "leisure=badvalue2", 1, score);
    HOOT_STR_EQUALS("leisure=badvalue1", avg.toStdString());
  }

  void categoryTest()
  {
    OsmSchema uut;
    uut.createTestingGraph();

    Tags tags;
    tags["poi"] = "yes";
    HOOT_STR_EQUALS(1, uut.hasCategory(tags, "poi"));
    HOOT_STR_EQUALS(0, uut.hasCategory(tags, "transportation"));

    tags.clear();
    tags["highway"] = "primary";
    HOOT_STR_EQUALS(0, uut.hasCategory(tags, "poi"));
    HOOT_STR_EQUALS(1, uut.hasCategory(tags, "transportation"));

    vector<SchemaVertex> tvs = uut.getTagByCategory(OsmSchemaCategory::transportation());
    vector<QString> names;
    for (size_t i = 0; i < tvs.size(); i++)
    {
      names.push_back(tvs[i].getName());
    }
    HOOT_STR_EQUALS("[5]{highway=road, highway=primary, highway=secondary, highway=residential, highway=service}",
                    names);
  }

  void commonAncestorTest()
  {
    OsmSchema uut;
    uut.createTestingGraph();

    const SchemaVertex& v1 = uut.getFirstCommonAncestor("highway=primary", "highway=secondary");
    CPPUNIT_ASSERT_EQUAL(string("road"), v1.getValue().toStdString());
    const SchemaVertex& v2 = uut.getFirstCommonAncestor("highway=primary", "highway=primary");
    CPPUNIT_ASSERT_EQUAL(string("primary"), v2.getValue().toStdString());
    const SchemaVertex& v3 = uut.getFirstCommonAncestor("highway=road", "highway=primary");
    CPPUNIT_ASSERT_EQUAL(string("road"), v3.getValue().toStdString());
    const SchemaVertex& v4 = uut.getFirstCommonAncestor("highway=primary", "highway=road");
    CPPUNIT_ASSERT_EQUAL(string("road"), v4.getValue().toStdString());
  }

  void getChildTagsTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    vector<SchemaVertex> gravel = uut.getChildTagsAsVertices("surface=gravel");

    CPPUNIT_ASSERT_EQUAL(2, (int)gravel.size());
  }

  void getSimilarTagsTest()
  {
    OsmSchema uut;
    uut.createTestingGraph();

    HOOT_STR_EQUALS("[3]{highway=road, highway=primary, highway=secondary}",
      _tagsToNames(uut.getSimilarTagsAsVertices("highway=primary", 0.8)));
    HOOT_STR_EQUALS("[4]{highway=road, highway=primary, highway=secondary, highway=residential}",
      _tagsToNames(uut.getSimilarTagsAsVertices("highway=primary", 0.5)));
    HOOT_STR_EQUALS("[1]{highway=road}",
      _tagsToNames(uut.getSimilarTagsAsVertices("highway=road", 0.1)));
  }

  /**
   * Test rudimentary loading of the schema file.
   */
  void loadTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    QFile fp("tmp/schema.dot");
    fp.open(QFile::WriteOnly);
    fp.write(uut.toGraphvizString().toUtf8());
    fp.close();

//    _dumpAsCsv(uut, "surface=unknown");
//    _dumpAsCsv(uut, "highway");

    double d;
    d = uut.score("highway=trunk", "highway=motorway");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, d, 0.001);
    d = uut.score("highway=path", "highway=motorway");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.210, d, 0.001);
    d = uut.score("highway=path", "highway=road");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, d, 0.001);

    QString avg;
    double score;
    avg = uut.average("highway=path", "highway=motorway", score);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.410, score, 0.001);
    CPPUNIT_ASSERT_EQUAL(std::string("highway=tertiary"), avg.toStdString());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7, uut.score("surface=cobblestone", "surface=asphalt"), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.score("surface=cobblestone:flattened",
                                                "surface=cobblestone"), 0.001);
    avg = uut.average("surface=cobblestone:flattened", "surface=asphalt", score);
    CPPUNIT_ASSERT_EQUAL(std::string("surface=paved"), avg.toStdString());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(.2, score, 0.001);
    avg = uut.average("surface=cobblestone:flattened", "surface=earth", score);
    CPPUNIT_ASSERT_EQUAL(avg.toStdString(), std::string("surface=unknown"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(.1, score, 0.001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.score("parking=surface", "amenity=parking"), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, uut.score("parking=surface", "parking=covered"), 0.001);

    // check wildcard mismatchScore for seamark:type.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, uut.score("seamark:type=foo", "seamark:type=bar"), 0.001);
    // Check to see if mismatchScore works within amenity types. This doesn't work now, but it'd
    // be good to implement this in the near future.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.score("amenity=auditorium", "amenity=embassy"), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.score("amenity=restaurant", "amenity=restaurant"), 0.001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8,
      uut.score("amenity=exhibition_hall", "amenity=convention_centre"), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,
      uut.score("amenity=conference_centre", "amenity=convention_centre"), 0.001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
      uut.scoreOneWay("poi=yes", "amenity=restaurant"), 0.001);

    Tags t;
    t["public_transport"] = "platform";
    t["bus"] = "yes";
    HOOT_STR_EQUALS("[2]{name: public_transport=platform\n"
      "key: public_transport\n"
      "value: platform\n"
      "influence: 1\n"
      "childWeight: -1\n"
      "mismatchScore: -1\n"
      "valueType: 2\n"
      "aliases: [0]{}\n"
      "geometries: 23\n"
      "categories: [1]{poi}\n"
      ", name: bus_platform\n"
      "key: \n"
      "value: \n"
      "influence: 1\n"
      "childWeight: -1\n"
      "mismatchScore: -1\n"
      "valueType: 2\n"
      "aliases: [0]{}\n"
      "geometries: 23\n"
      "categories: [1]{poi}\n"
      "tags: [2]{[2]{public_transport=platform, bus=yes}, [2]{public_transport=platform, highway=bus_stop}}\n"
      "}", toString(uut.getSchemaVertices(t)));

    HOOT_STR_EQUALS("[1]{name: bus_platform\n"
      "key: \n"
      "value: \n"
      "influence: 1\n"
      "childWeight: -1\n"
      "mismatchScore: -1\n"
      "valueType: 2\n"
      "aliases: [0]{}\n"
      "geometries: 23\n"
      "categories: [1]{poi}\n"
      "tags: [2]{[2]{public_transport=platform, bus=yes}, [2]{public_transport=platform, highway=bus_stop}}\n"
      "}", toString(uut.getUniqueSchemaVertices(t)));

    HOOT_STR_EQUALS(1, uut.score("highway=bus_stop", "bus_platform"));

    avg = uut.average("highway=track", "highway=secondary", score);
    HOOT_STR_EQUALS("highway=unclassified", avg.toStdString());

    HOOT_STR_EQUALS(0.3, uut.score("highway=residential", "highway=construction"));

    // expecting the first tag as a result if there is only one hop between the two.
    avg = uut.average("highway=track", "highway=unclassified", score);
    HOOT_STR_EQUALS("highway=track", avg.toStdString());
  }

  /**
   * Test calculating distance between two tags
   */
  void distanceTest()
  {
    OsmSchema uut;
    uut.createTestingGraph();

    double d = uut.score("highway=primary", "highway=secondary");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, d, 0.001);
    d = uut.score("highway=primary", "highway=bad_value");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, d, 0.001);
    d = uut.score("Highway=primary", "highway=Bad_value");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, d, 0.001);
    d = uut.score("highway=secondary", "highway=residential");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.64, d, 0.001);
    d = uut.score("HIGHWAY=Secondary", "hiGHway=ResidenTial");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.64, d, 0.001);
    d = uut.score("highway=primary", "highway=residential");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.512, d, 0.001);
    d = uut.score("highway=primary", "highway=service");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4096, d, 0.001);
    d = uut.score("highway=primary", "highway=road");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, d, 0.001);
  }

  void getTagTest()
  {
    OsmSchema uut;
    uut.createTestingGraph();

    HOOT_STR_EQUALS("abstract_name", uut.getTagVertex("abstract_name").getName());
    HOOT_STR_EQUALS("abstract_name", uut.getTagVertex("abstract_name=foo").getName());
    HOOT_STR_EQUALS("name", uut.getTagVertex("name").getName());
    HOOT_STR_EQUALS("name", uut.getTagVertex("name=bar").getName());
    HOOT_STR_EQUALS("", uut.getTagVertex("poi=foo").getName());
    HOOT_STR_EQUALS("poi=yes", uut.getTagVertex("poi=yes").getName());
    HOOT_STR_EQUALS("leisure=*", uut.getTagVertex("leisure=foo").getName());
  }

  void isAncestorTest()
  {
    OsmSchema uut;
    uut.createTestingGraph();

    CPPUNIT_ASSERT_EQUAL(false, uut.isAncestor("highway=primary", "highway=secondary"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isAncestor("highway=primary", "highway=road"));

    CPPUNIT_ASSERT_EQUAL(true, OsmSchema::getInstance().isAncestor("highway=secondary",
      "highway=road"));
  }

  void isMetaDataTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    CPPUNIT_ASSERT_EQUAL(false, uut.isMetaData("name", "foo"));
    CPPUNIT_ASSERT_EQUAL(false, uut.isMetaData("foo", "bar"));
    CPPUNIT_ASSERT_EQUAL(false, uut.isMetaData("highway", "service"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("security", "U"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("security:blah", "dunno"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("source", "foo"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("source", ""));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("source:bar", "baz"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("created_by", "baz"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("source", "foo"));
    CPPUNIT_ASSERT_EQUAL(true, uut.isMetaData("source", ""));
  }

  void religionTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    double d;
    // These should have a high score. The exact value isn't important.
    d = uut.score("building=mosque", "amenity=place_of_worship");
    CPPUNIT_ASSERT(d >= 0.8);
    // These shouldn't have a high score. The exact score isn't important.
    d = uut.score("building=mosque", "amenity=church");
    CPPUNIT_ASSERT(d <= 0.3);
    // These should have a high score. The exact value isn't important.
    d = uut.score("building=abbey", "amenity=church");
    CPPUNIT_ASSERT(d >= 0.8);
  }

  void isGenericTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    Tags tags;
    tags["poi"] = "yes";
    CPPUNIT_ASSERT(uut.isGeneric(tags));

    tags["amenity"] = "ambulance_station";
    CPPUNIT_ASSERT(!uut.isGeneric(tags));

    // don't account for non-standard tags, so this will come back as generic...that's ok
    tags["amenity"] = "yes";
    CPPUNIT_ASSERT(!uut.isGeneric(tags));
  }

  void isGenericKvpTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    CPPUNIT_ASSERT(uut.isGenericKvp("poi=yes"));
    CPPUNIT_ASSERT(!uut.isGenericKvp("amenity=ambulance_station"));
  }

  void hasTypeTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    Tags tags;
    tags["poi"] = "yes";
    CPPUNIT_ASSERT(uut.hasType(tags));

    tags.clear();
    tags["amenity"] = "ambulance_station";
    CPPUNIT_ASSERT(uut.hasType(tags));

    tags.clear();
    CPPUNIT_ASSERT(!uut.hasType(tags));
  }

  void hasMoreThanOneTypeTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    Tags tags;
    tags["poi"] = "yes";
    CPPUNIT_ASSERT(!uut.hasMoreThanOneType(tags));

    tags["amenity"] = "ambulance_station";
    CPPUNIT_ASSERT(uut.hasMoreThanOneType(tags));

    tags.clear();
    CPPUNIT_ASSERT(!uut.hasMoreThanOneType(tags));
  }

  void getFirstTypeTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    Tags tags;
    tags["poi"] = "yes";
    HOOT_STR_EQUALS("poi=yes", uut.getFirstType(tags, true));
    HOOT_STR_EQUALS("", uut.getFirstType(tags, false));

    tags["amenity"] = "ambulance_station";
    HOOT_STR_EQUALS("amenity=ambulance_station", uut.getFirstType(tags, true));
    HOOT_STR_EQUALS("amenity=ambulance_station", uut.getFirstType(tags, false));
  }

  void explicitTypeMismatchTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();

    Tags tags1;
    tags1["amenity"] = "ambulance_station";
    Tags tags2;
    tags2["amenity"] = "ambulance_station";
    CPPUNIT_ASSERT(!uut.explicitTypeMismatch(tags1, tags2, 0.8));

    tags1.clear();
    tags1["amenity"] = "ambulance_station";
    tags2.clear();
    tags2["amenity"] = "animal_boarding";
    CPPUNIT_ASSERT(uut.explicitTypeMismatch(tags1, tags2, 0.8));

    tags1.clear();
    tags1["amenity"] = "ambulance_station";
    tags2.clear();
    tags2["landuse"] = "military";
    CPPUNIT_ASSERT(uut.explicitTypeMismatch(tags1, tags2, 0.8));

    tags1.clear();
    tags1["poi"] = "yes";
    tags2.clear();
    tags2["poi"] = "yes";
    CPPUNIT_ASSERT(!uut.explicitTypeMismatch(tags1, tags2, 0.8));

    tags1.clear();
    tags1["poi"] = "yes";
    tags2.clear();
    tags2["amenity"] = "ambulance_station";
    CPPUNIT_ASSERT(!uut.explicitTypeMismatch(tags1, tags2, 0.8));
  }

  void mostSpecificTypeTest()
  {
    OsmSchema& uut = OsmSchema::getInstance();
    Tags tags;

    tags.clear();
    tags["building"] = "yes";
    HOOT_STR_EQUALS("building=yes", uut.mostSpecificType(tags));

    tags.clear();
    tags["building"] = "yes";
    tags["building"] = "fort";
    HOOT_STR_EQUALS("building=fort", uut.mostSpecificType(tags));

    tags.clear();
    tags["building"] = "yes";
    tags["name"] = "fort";
    HOOT_STR_EQUALS("building=yes", uut.mostSpecificType(tags));

    // Even though surface=asphalt isn't a child of highway-secondary, surface=asphalt is identified
    // in the schema as a combo tag, which means it needs to be used in combination with another
    // tag to clearly delinate a type. Non-type tags are considered more specific than type tags.
    tags.clear();
    tags["highway"] = "secondary";
    tags["surface"] = "asphalt";
    HOOT_STR_EQUALS("highway=secondary", uut.mostSpecificType(tags));
  }

private:

  QStringList _tagsToNames(const vector<SchemaVertex>& v)
  {
    QStringList l;
    for (size_t i = 0; i < v.size(); i++)
    {
      l << v[i].getName();
    }
    return l;
  }

  void _dumpAsCsv(OsmSchema& schema, QString tag)
  {
    vector<SchemaVertex> surfaces = schema.getChildTagsAsVertices(tag);
    QString csvDistance;
    QString csvAverage;

    for (size_t i = 0; i < surfaces.size(); i++)
    {
      csvDistance += ", " + surfaces[i].getName();
    }
    csvDistance += "\n";
    csvAverage = csvDistance;

    for (size_t i = 0; i < surfaces.size(); i++)
    {
      csvDistance += surfaces[i].getName();
      csvAverage += surfaces[i].getName();
      for (size_t j = 0; j < surfaces.size(); j++)
      {
        double d = schema.score(surfaces[i].getName(), surfaces[j].getName());
        csvDistance += QString(", %1").arg(d);
        double best;
        QString avg = schema.average(surfaces[i].getName(), surfaces[j].getName(), best);
        csvAverage += QString(", %1").arg(avg);
      }
      csvDistance += "\n";
      csvAverage += "\n";
    }

    QFile csvFp("test-output/" + tag + ".csv");
    csvFp.open(QFile::WriteOnly);
    csvFp.write(csvDistance.toUtf8());
    csvFp.close();

    QFile csvAverageFp("test-output/" + tag + "-average.csv");
    csvAverageFp.open(QFile::WriteOnly);
    csvAverageFp.write(csvAverage.toUtf8());
    csvAverageFp.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmSchemaTest, "slow");

}
