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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/scoring/TextTable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

using namespace std;

namespace hoot
{

class TagComparatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagComparatorTest);
  CPPUNIT_TEST(averageTest);
  CPPUNIT_TEST(averageCaseSensitiveTest);
  CPPUNIT_TEST(averageCaseInsensitiveTest);
  CPPUNIT_TEST(buildingTest);
  CPPUNIT_TEST(compareTest);
  CPPUNIT_TEST(compareNamesTest);
  CPPUNIT_TEST(compareEnumTest);
  CPPUNIT_TEST(generalizeTest);
  CPPUNIT_TEST(generalizeCaseSensitiveTest);
  CPPUNIT_TEST(generalizeCaseInsensitiveTest);
  CPPUNIT_TEST(railwayBusStopTest);
  CPPUNIT_TEST(realWorldTest);
  CPPUNIT_TEST(nonNameTagsExactlyMatchTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void averageTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
        Tags t1;
        t1["highway"] = "trunk";
        t1["name"] = "Midland Expressway";
        t1["name:en"] = "Midland Expressway";
        t1["ref"] = "US 24";
        t1["oneway"] = "yes";
        t1["lanes"] = "2";
        t1["uuid"] = "foo";

        Tags t2;
        t2["highway"] = "secondary";
        t2["name"] = "Midland Expy";
        t2["name:en"] = "Midland Expressway";
        t2["name:he"] = "מידלנד המהיר";
        t2["ref"] = "24";
        t2["oneway"] = "true";
        t2["bridge"] = "yes";
        t2["uuid"] = "bar";

        Tags expected;
        expected["highway"] = "primary";
        expected["name"] = "Midland Expressway";
        expected["name:en"] = "Midland Expressway";
        expected["ref"] = "US 24";
        expected["oneway"] = "yes";
        expected["lanes"] = "2";
        expected["alt_name"] = "24;Midland Expy";
        expected["name:he"] = "מידלנד המהיר";
        expected["bridge"] = "yes";
        expected["uuid"] = "foo;bar";

        Tags avg;
        uut.averageTags(t1, t2, avg);
        compareTags(expected, avg);
    }

    {
        Tags t1;
        t1["highway"] = "trunk";
        t1["name"] = "Midland Expressway";
        t1["name:en"] = "Midland Expressway";
        t1["ref"] = "US 24";
        t1["oneway"] = "yes";
        t1["lanes"] = "2";

        Tags t2;
        t2["highway"] = "primary";
        t2["name"] = "Midland Expy";
        t2["name:en"] = "Midland Expy";
        t2["ref"] = "24";
        t2["oneway"] = "true";

        Tags expected;
        expected["highway"] = "trunk";
        expected["name"] = "Midland Expressway";
        expected["name:en"] = "Midland Expressway";
        expected["ref"] = "US 24";
        expected["oneway"] = "yes";
        expected["lanes"] = "2";
        expected["alt_name"] = "24;Midland Expy";

        Tags avg;
        uut.averageTags(t1, t2, avg);
        compareTags(expected, avg);
    }

    //    {
    //      Tags t1;
    //      t1["highway"] = "path";
    //      t1["foot"] = "designated";
    //      t1["name"] = "Intemann Trail";
    //      t1["official_name"] = "Paul Intemann Memorial Nature Trail";

    //      Tags t2;
    //      t2["highway"] = "footway";
    //      t2["name"] = "Intemann";

    //      Tags expected;
    //      expected["highway"] = "path";
    //      expected["foot"] = "designated";
    //      expected["name"] = "Intemann Trail";
    //      expected["official_name"] = "Paul Intemann Memorial Nature Trail";
    //      expected["alt_name"] = "Intemann";

    //      Tags avg;
    //      uut.averageTags(t1, t2, avg);
    //      //LOG_WARN("average: \n" << avg.toString());
    //      compareTags(expected, avg);
    //    }

    {
        Tags t1;
        t1["highway"] = "road";
        t1["surface"] = "paved";

        Tags t2;
        t2["highway"] = "primary";
        t2["surface"] = "asphalt";

        Tags expected;
        expected["highway"] = "primary";
        expected["surface"] = "asphalt";

        Tags avg;
        uut.averageTags(t1, t2, avg);
        //LOG_WARN("average: \n" << avg.toString());
        compareTags(expected, avg);
    }

    {
        Tags t1;
        t1["note"] = "b;a;c;c";

        Tags t2;
        t2["note"] = "a;b;d";

        Tags expected;
        expected["note"] = "b;a;c;d";

        Tags avg;
        uut.averageTags(t1, t2, avg);
        //LOG_WARN("average: \n" << avg.toString());
        compareTags(expected, avg);
    }
  }

  void averageCaseSensitiveTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["highway"] = "trunk";
      t1["name"] = "Midland Expressway";
      t1["name:en"] = "Midland Expressway";
      t1["ref"] = "US 24";
      t1["oneway"] = "yes";
      t1["lanes"] = "2";
      t1["uuid"] = "foo";

      Tags t2;
      t2["highway"] = "secondary";
      t2["name"] = "Midland Expy";
      t2["name:en"] = "MIDLAND EXPRESSWAY";
      t2["name:he"] = "מידלנד המהיר";
      t2["ref"] = "24";
      t2["oneway"] = "true";
      t2["bridge"] = "yes";
      t2["uuid"] = "bar";

      //With case sensitivity on, the two name:en tags are considered different from one another.
      //So, the all caps name goes into alt_name.
      Tags expected;
      expected["highway"] = "primary";
      expected["name"] = "Midland Expressway";
      expected["name:en"] = "Midland Expressway";
      expected["ref"] = "US 24";
      expected["oneway"] = "yes";
      expected["lanes"] = "2";
      expected["alt_name"] = "24;MIDLAND EXPRESSWAY;Midland Expy";
      expected["name:he"] = "מידלנד המהיר";
      expected["bridge"] = "yes";
      expected["uuid"] = "foo;bar";

      Tags avg;
      uut.averageTags(t1, t2, avg);
      compareTags(expected, avg);
    }
  }

  void averageCaseInsensitiveTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["highway"] = "trunk";
      t1["name"] = "Midland Expressway";
      t1["name:en"] = "Midland Expressway";
      t1["ref"] = "US 24";
      t1["oneway"] = "yes";
      t1["lanes"] = "2";
      t1["uuid"] = "foo";

      Tags t2;
      t2["highway"] = "secondary";
      t2["name"] = "Midland Expy";
      t2["name:en"] = "MIDLAND EXPRESSWAY";
      t2["name:he"] = "מידלנד המהיר";
      t2["ref"] = "24";
      t2["oneway"] = "true";
      t2["bridge"] = "yes";
      t2["uuid"] = "bar";

      //With case sensitivity off, the two name:en tags are considered the same.  So, the camel
      //cased one is chosen and the other does not go into alt_name.
      Tags expected;
      expected["highway"] = "primary";
      expected["name"] = "Midland Expressway";
      expected["name:en"] = "Midland Expressway";
      expected["ref"] = "US 24";
      expected["oneway"] = "yes";
      expected["lanes"] = "2";
      expected["alt_name"] = "24;Midland Expy";
      expected["name:he"] = "מידלנד המהיר";
      expected["bridge"] = "yes";
      expected["uuid"] = "foo;bar";

      Tags avg;
      uut.averageTags(t1, t2, avg, false, false);
      compareTags(expected, avg);
     }
  }

  void buildingTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["building"] = "yes";
      t1["addr:housenumber"] = "123";

      Tags t2;
      t2["building"] = "yes";
      t2["addr:housenumber"] = "125";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6667, uut.compareTags(t1, t2), 0.001);
      t2["addr:housenumber"] = "123";
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["building"] = "yes";
      t1["addr:housenumber"] = "125";
      t1["addr:city"] = "Denver";

      Tags t2;
      t2["building"] = "yes";
      t2["addr:housenumber"] = "125";
      t2["addr:city"] = "Denvir";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.833, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["building"] = "commercial";

      Tags t2;
      t2["building"] = "residential";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, uut.compareTags(t1, t2), 0.001);
    }
  }

  void compareTags(const Tags& t1, const Tags& t2)
  {
    if (t1.size() != t2.size())
    {
      LOG_WARN("t1: " << t1.toString());
      LOG_WARN("t2: " << t2.toString());
      CPPUNIT_ASSERT_EQUAL(t1.size(), t2.size());
    }

    for (Tags::const_iterator it = t1.begin(); it != t1.end(); ++it)
    {
      if (t1[it.key()] != t2[it.key()])
      {
        LOG_WARN("t1: " << t1.toString());
        LOG_WARN("t2: " << t2.toString());
        CPPUNIT_ASSERT_EQUAL(t1[it.key()].toStdString(), t2[it.key()].toStdString());
      }
    }
  }

  /**
   * Test calculating distance between two tags
   */
  void compareTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["highway"] = "trunk";
      t1["name"] = "Midland Expressway";
      t1["name:en"] = "Midland Expressway";
      t1["ref"] = "US 24";
      t1["oneway"] = "yes";
      t1["lanes"] = "2";

      Tags t2;
      t2["highway"] = "primary";
      t2["name"] = "Midland Expy";
      t2["name:en"] = "Midland Expy";
      t2["ref"] = "24";
      t2["oneway"] = "true";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5333, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["highway"] = "secondary";

      Tags t2;
      t2["highway"] = "road";
      t2["surface"] = "unpaved";
      t2["all_weather"] = "fair";
      t2["width"] = "4";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["highway"] = "residential";
      t1["name"] = "Oak Place";
      t1["name:en"] = "Oak Place";
      t1["alt_name"] = "My Street;Dog Bark Lane";
      t1["oneway"] = "false";
      t1["lanes"] = "2";

      Tags t2;
      t2["highway"] = "road";
      t2["name"] = "Oak Pl";
      t2["name:en"] = "Oak Pl";
      t2["name:de"] = "Oak Platz";
      // This should show up as hebrew characters. This is intentional. Please be sure you're using
      // UTF-8 text encoding before you modify this file.
      t2["name:he"] = "אלון המקום";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7222, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["all_weather"] = "yes";
      t1["bridge"] = "";
      t1["highway"] = "secondary";
      t1["lanes"] = "2";
      t1["surface"] = "paved";
      t1["width"] = "5.5";

      Tags t2;
      t2["highway"] = "road";
      t2["all_weather"] = "fair";
      t2["surface"] = "unpaved";
      t2["width"] = "3.5";

      //cout << uut.compareTags(t1, t2) << endl;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.048, uut.compareTags(t1, t2), 0.001);
    }
  }

  /**
   * Test calculating distance between two tags
   */
  void compareEnumTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["highway"] = "trunk";
      t1["oneway"] = "yes";
      t1["surface"] = "paved";

      Tags t2;
      t2["highway"] = "primary";
      t2["oneway"] = "true";
      t2["surface"] = "asphalt";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["highway"] = "residential";
      t1["oneway"] = "false";
      t1["surface"] = "asphalt";

      Tags t2;
      t2["highway"] = "road";
      t2["oneway"] = "true";
      t2["surface"] = "grass";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["highway"] = "primary";
      t1["oneway"] = "1";
      t1["surface"] = "asphalt";

      Tags t2;
      t2["highway"] = "residential";
      t2["oneway"] = "true";
      t2["surface"] = "dirt";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.051, uut.compareTags(t1, t2), 0.001);
    }

  }

  /**
   * Test calculating distance between two tags
   */
  void compareNamesTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["name"] = "Midland Expressway";
      t1["name:en"] = "Midland Expressway";
      t1["ref"] = "US 24";

      Tags t2;
      t2["name"] = "Midland Expy";
      t2["name:en"] = "Midland Expy";
      t2["ref"] = "24";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.667, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["name"] = "Oak Place";
      t1["name:en"] = "Oak Place";
      t1["alt_name"] = "My Street;Dog Bark Lane";

      Tags t2;
      t2["name"] = "Oak Pl";
      t2["name:en"] = "Oak Pl";
      t2["name:de"] = "Oak Platz";
      // This should show up as hebrew characters. This is intentional. Please be sure you're using
      // UTF-8 text encoding before you modify this file.
      t2["name:he"] = "אלון המקום";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.722, uut.compareTags(t1, t2), 0.001);
    }
  }

  void generalizeTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
        Tags t1;
        t1["name"] = "foo";
        t1["lala"] = "1;2";
        t1["building"] = "yes";
        t1["uid"] = "123";

        Tags t2;
        t2["name"] = "bar";
        t2["lala"] = "2;1";
        t2["building"] = "yes";
        t2["uid"] = "456";

        Tags expected;
        expected["name"] = "foo";
        expected["alt_name"] = "bar";
        expected["lala"] = "1;2";
        expected["uid"] = "123;456";
        expected["building"] = "yes";

        Tags gen = uut.generalize(t1, t2);
        compareTags(expected, gen);
    }

    {
        Tags t1;
        t1["highway"] = "trunk";
        t1["name"] = "Midland Expressway";
        t1["name:en"] = "Midland Expressway";
        t1["ref"] = "US 24";
        t1["oneway"] = "yes";
        t1["lanes"] = "2";

        Tags t2;
        t2["highway"] = "secondary";
        t2["name"] = "Midland Expy";
        t2["name:en"] = "Midland Expressway";
        t2["name:he"] = "מידלנד המהיר";
        t2["ref"] = "24";
        t2["oneway"] = "true";
        t2["bridge"] = "yes";

        Tags expected;
        expected["highway"] = "road";
        expected["name"] = "Midland Expressway";
        expected["name:en"] = "Midland Expressway";
        expected["ref"] = "US 24";
        expected["oneway"] = "yes";
        expected["lanes"] = "2";
        expected["alt_name"] = "24;Midland Expy";
        expected["name:he"] = "מידלנד המהיר";
        expected["bridge"] = "yes";

        Tags gen = uut.generalize(t1, t2);
        compareTags(expected, gen);
    }

    {
        Tags t1;
        t1["highway"] = "road";
        t1["surface"] = "paved";

        Tags t2;
        t2["highway"] = "primary";
        t2["surface"] = "asphalt";

        Tags expected;
        expected["highway"] = "road";
        expected["surface"] = "paved";

        Tags gen = uut.generalize(t1, t2);
        compareTags(expected, gen);
    }

    {
        Tags t1;
        t1["building"] = "residential";
        t1[MetadataTags::Ref1()] = "foo";

        Tags t2;
        t2["building"] = "no";
        t2[MetadataTags::Ref1()] = "bar";

        Tags expected;
        expected[MetadataTags::Ref1()] = "foo;bar";

        Tags gen = uut.generalize(t1, t2);
        compareTags(expected, gen);
    }

    {
        Tags t1;
        t1["name"] = "foo";
        t1["alt_name"] = "baz";
        t1["name:en"] = "enfoo";
        t1["note"] = "foo";

        Tags t2;
        t2["name"] = "bar";
        t2["alt_name"] = "qux;quux";
        t2["name:en"] = "enbar";
        t2["note"] = "bar";

        Tags expected;
        // building is dropped due to conflict
        expected["name"] = "foo";
        expected["alt_name"] = "bar;baz;enbar;quux;qux";
        expected["name:en"] = "enfoo";
        expected["note"] = "foo;bar";

        Tags gen = uut.generalize(t1, t2);
        compareTags(expected, gen);
    }
  }

  void generalizeCaseSensitiveTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["name"] = "foo";
      t1["lala"] = "1;2";
      t1["building"] = "yes";
      t1["uid"] = "123";

      Tags t2;
      t2["name"] = "FOO";
      t2["lala"] = "2;1";
      t2["building"] = "yes";
      t2["uid"] = "456";

      //With case sensitivity on, the two name tags are considered different from one another.
      //So, the all caps name goes into alt_name.
      Tags expected;
      expected["name"] = "foo";
      expected["alt_name"] = "FOO";
      expected["lala"] = "1;2";
      expected["uid"] = "123;456";
      expected["building"] = "yes";

      Tags gen = uut.generalize(t1, t2);
      compareTags(expected, gen);
    }
  }

  void generalizeCaseInsensitiveTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["name"] = "foo";
      t1["lala"] = "1;2";
      t1["building"] = "yes";
      t1["uid"] = "123";

      Tags t2;
      t2["name"] = "FOO";
      t2["lala"] = "2;1";
      t2["building"] = "yes";
      t2["uid"] = "456";

      //With case sensitivity off, the two name tags are considered different from each other.  So,
      //the lower cased one is chosen and the other does not go into alt_name.
      Tags expected;
      expected["name"] = "foo";
      expected["lala"] = "1;2";
      expected["uid"] = "123;456";
      expected["building"] = "yes";

      Tags gen = uut.generalize(t1, t2, false, false);
      compareTags(expected, gen);
    }
  }

  /**
   * Test rudimentary loading of the schema file.
   */
  void railwayBusStopTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    vector<Tags> railways;
    vector<Tags> buses;

    Tags t;

    t.clear();
    t["highway"] = "bus_stop";
    buses.push_back(t);

    t.clear();
    t["amenity"] = "bus_station";
    buses.push_back(t);

    t.clear();
    t["railway"] = "halt";
    railways.push_back(t);

    t.clear();
    t["railway"] = "station";
    railways.push_back(t);

    t.clear();
    t["station"] = "light_rail";
    railways.push_back(t);

    t.clear();
    t["railway"] = "subway_entrance";
    railways.push_back(t);

    TextTable::Data data;

    for (size_t i = 0; i < buses.size(); i++)
    {
      for (size_t j = 0; j < railways.size(); j++)
      {
        Tags t1 = buses[i];
        Tags t2 = railways[j];
        QString s1 = toString(t1).trimmed();
        QString s2 = toString(t2).trimmed();
        data[s1][s2] = uut.compareTags(t1, t2);
        data[s2][s1] = uut.compareTags(t2, t1);
        //LOG_VAR(uut.compareTags(buses[i], railways[i]));
      }
    }

    for (size_t i = 0; i < buses.size(); i++)
    {
      for (size_t j = 0; j < buses.size(); j++)
      {
        Tags t1 = buses[i];
        Tags t2 = buses[j];
        QString s1 = toString(t1).trimmed();
        QString s2 = toString(t2).trimmed();
        data[s1][s2] = uut.compareTags(t1, t2);
        //LOG_VAR(uut.compareTags(buses[i], railways[i]));
      }
    }

    for (size_t i = 0; i < railways.size(); i++)
    {
      for (size_t j = 0; j < railways.size(); j++)
      {
        Tags t1 = railways[i];
        Tags t2 = railways[j];
        QString s1 = toString(t1).trimmed();
        QString s2 = toString(t2).trimmed();
        data[s1][s2] = uut.compareTags(t1, t2);
        //LOG_VAR(uut.compareTags(buses[i], railways[i]));
      }
    }

    TextTable table(data);
    LOG_VARD(table.toWikiString());
  }

  /**
   * Test calculating distance between two tags
   */
  void realWorldTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["highway"] = "primary_link";
      t1["all_weather"] = "yes";
      t1["bridge"] = "";
      t1["surface"] = "paved";
      t1["tunnel"] = "";

      Tags t2;
      t2["bridge"] = "";
      t2["highway"] = "motorway_link";
      t2["lanes"] = "2";
      t2["oneway"] = "yes";
      t2["tunnel"] = "";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.288, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["name"] = "Mayjen Sutoyo";
      t1["highway"] = "primary";
      t1["all_weather"] = "yes";
      t1["bridge"] = "";
      t1["surface"] = "paved";
      t1["tunnel"] = "";

      Tags t2;
      t2["bridge"] = "";
      t2["highway"] = "motorway_link";
      t2["lanes"] = "2";
      t2["oneway"] = "yes";
      t2["tunnel"] = "";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.32, uut.compareTags(t1, t2), 0.001);
    }

    {
      Tags t1;
      t1["bridge"] = "";
      t1["highway"] = "primary";
      t1["name"] = "Otto Iskandar Dinata";
      t1["oneway"] = "yes";
      t1["tunnel"] = "";

      Tags t2;
      t2["all_weather"] = "yes";
      t2["alt_name"] = "OTTO ISKANDARDINATA";
      t2["highway"] = "secondary";
      t2["lanes"] = "4";
      t2["name"] = "OTTO ISKANDARDINATA (OLLSLA)";
      t2["oneway"] = "yes";
      t2["surface"] = "paved";
      t2["width"] = "10";

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.76, uut.compareTags(t1, t2), 0.001);
    }
  }

  void nonNameTagsExactlyMatchTest()
  {
    TagComparator& uut = TagComparator::getInstance();

    {
      Tags t1;
      t1["highway"] = "primary_link";
      t1["all_weather"] = "yes";
      t1["bridge"] = "";
      t1["surface"] = "paved";
      t1["tunnel"] = "";

      Tags t2;
      t2["highway"] = "primary_link";
      t2["all_weather"] = "yes";
      t2["bridge"] = "";
      t2["surface"] = "paved";
      t2["tunnel"] = "";
      t2[MetadataTags::HootId()] = "1";

      CPPUNIT_ASSERT(uut.nonNameTagsExactlyMatch(t1, t2));
    }

    {
      Tags t1;
      t1["highway"] = "primary_link";
      t1["all_weather"] = "yes";
      t1["bridge"] = "";
      t1["surface"] = "paved";
      t1["tunnel"] = "";

      Tags t2;
      t2["highway"] = "primary_link";
      t2["tunnel"] = "";
      t2["all_weather"] = "yes";
      t2["surface"] = "paved";
      t2[MetadataTags::HootId()] = "1";
      t2["bridge"] = "";

      CPPUNIT_ASSERT(uut.nonNameTagsExactlyMatch(t1, t2));
    }

    {
      Tags t1;
      t1["highway"] = "primary_link";
      t1["all_weather"] = "yes";
      t1["bridge"] = "";
      t1["surface"] = "paved";
      t1["tunnel"] = "";

      Tags t2;
      t2["HIGHWAY"] = "primary_link";
      t2["all_weather"] = "YES";
      t2["bridge"] = "";
      t2["surface"] = "paved";
      t2["tunnel"] = "";
      t2[MetadataTags::HootId()] = "1";

      CPPUNIT_ASSERT(uut.nonNameTagsExactlyMatch(t1, t2, false));
    }

    {
      Tags t1;
      t1["highway"] = "primary_link";
      t1["all_weather"] = "YES";
      t1["bridge"] = "";
      t1["SURFACE"] = "PAVED";
      t1["tunnel"] = "";

      Tags t2;
      t2["highway"] = "PRIMARY_LINK";
      t2["tunnel"] = "";
      t2["all_weather"] = "yes";
      t2["surface"] = "paved";
      t2[MetadataTags::HootId()] = "1";
      t2["BRIDGE"] = "";

      CPPUNIT_ASSERT(uut.nonNameTagsExactlyMatch(t1, t2, false));
    }

    {
      Tags t1;
      t1["highway"] = "primary_link";
      t1["all_weather"] = "yes";
      t1["bridge"] = "";
      t1["surface"] = "paved";
      t1["tunnel"] = "";

      Tags t2;
      t2["HIGHWAY"] = "primary_link";
      t2["all_weather"] = "YES";
      t2["bridge"] = "";
      t2["surface"] = "paved";
      t2["tunnel"] = "";
      t2[MetadataTags::HootId()] = "1";

      CPPUNIT_ASSERT(!uut.nonNameTagsExactlyMatch(t1, t2));
    }

    {
      Tags t1;
      t1["key1"] = "bar";
      t1["key2"] = "foo";

      Tags t2;
      t2["key1"] = "foo";
      t2["key2"] = "bar";

      CPPUNIT_ASSERT(!uut.nonNameTagsExactlyMatch(t1, t2));
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagComparatorTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagComparatorTest, "current");

}
