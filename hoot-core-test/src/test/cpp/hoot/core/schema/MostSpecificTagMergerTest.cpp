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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/MostSpecificTagMerger.h>

namespace hoot
{

/**
 * These tests are fairly sensitive to schema changes. This test is essentially the same as
 * PreserveTypesTagMergerTest but ensures TypesTagMerger::ALT_TYPES_TAG_KEY doesn't get populated.
 */
class MostSpecificTagMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MostSpecificTagMergerTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(overlappingKeysTest);
  CPPUNIT_TEST(overwrite1Test);
  CPPUNIT_TEST(overwrite1OverlappingKeysTest);
  CPPUNIT_TEST(skipTagsTest);
  CPPUNIT_TEST(ancestorTest);
  CPPUNIT_TEST(ancestorGenericTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    // All the type tags should be preserved, the second name should go to alt_names, and
    // building=yes shouldn't be duplicated

    Tags t1;
    t1[MetadataTags::Building()] = "yes";
    t1["name"] = "Building 1";
    t1["shop"] = "supermarket";

    Tags t2;
    t2[MetadataTags::Building()] = "yes";
    t2["name"] = "Building 2";
    t2["amenity"] = "restaurant";

    Tags expected;
    expected[MetadataTags::Building()] = "yes";
    expected["name"] = "Building 1";
    expected["alt_name"] = "Building 2";
    expected["shop"] = "supermarket";
    expected["amenity"] = "restaurant";

    MostSpecificTagMerger uut;
    uut.setOverwrite1(false);
    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void overlappingKeysTest()
  {
    // The second name should go into alt_name.

    Tags t1;
    t1["name"] = "Building 1";
    t1[MetadataTags::Building()] = "cathedral";

    Tags t2;
    t2["name"] = "Building 2";
    t2[MetadataTags::Building()] = "chapel";

    Tags expected;
    expected["name"] = "Building 1";
    expected["alt_name"] = "Building 2";
    expected[MetadataTags::Building()] = "cathedral";

    MostSpecificTagMerger uut;
    uut.setOverwrite1(false);

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    // After this merge, we should see names for the second two buildings in alt_name.

    Tags t3;
    t3["name"] = "Building 3";
    t3[MetadataTags::Building()] = "church";

    Tags expected2;
    expected2["name"] = "Building 1";
    expected2["alt_name"] = "Building 2;Building 3";
    expected2[MetadataTags::Building()] = "cathedral";

    merged = uut.mergeTags(merged, t3, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected2, merged);
  }

  void overwrite1Test()
  {
    // The first name should go in alt_name, and all types should be preserved.

    Tags t1;
    t1[MetadataTags::Building()] = "yes";
    t1["name"] = "Building 1";
    t1["shop"] = "supermarket";

    Tags t2;
    t2[MetadataTags::Building()] = "yes";
    t2["name"] = "Building 2";
    t2["amenity"] = "restaurant";

    Tags expected;
    expected[MetadataTags::Building()] = "yes";
    expected["name"] = "Building 2";
    expected["alt_name"] = "Building 1";
    expected["shop"] = "supermarket";
    expected["amenity"] = "restaurant";

    MostSpecificTagMerger uut;
    uut.setOverwrite1(true);
    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void overwrite1OverlappingKeysTest()
  {
    // The first name should go into alt_name, and only the second building tag should be preserved.

    Tags t1;
    t1["name"] = "Building 1";
    t1[MetadataTags::Building()] = "cathedral";

    Tags t2;;
    t2["name"] = "Building 2";
    t2[MetadataTags::Building()] = "chapel";

    Tags expected;
    expected["name"] = "Building 2";
    expected["alt_name"] = "Building 1";
    expected[MetadataTags::Building()] = "chapel";

    MostSpecificTagMerger uut;
    uut.setOverwrite1(true);

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    Tags t3;
    t3["name"] = "Building 3";
    t3[MetadataTags::Building()] = "church";

    Tags expected2;;
    expected2["name"] = "Building 3";
    expected2["alt_name"] = "Building 1;Building 2";
    expected2[MetadataTags::Building()] = "church";

    merged = uut.mergeTags(merged, t3, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected2, merged);
  }

  void skipTagsTest()
  {
    // The amenity tag should be witheld from the output.

    Tags t1;
    t1[MetadataTags::Building()] = "yes";
    t1["name"] = "Building 1";
    t1["shop"] = "supermarket";

    Tags t2;
    t2[MetadataTags::Building()] = "yes";
    t2["name"] = "Building 2";
    t2["amenity"] = "restaurant";

    Tags expected;
    expected[MetadataTags::Building()] = "yes";
    expected["name"] = "Building 1";
    expected["alt_name"] = "Building 2";
    expected["shop"] = "supermarket";

    QSet<QString> keys;
    keys.insert("amenity");
    MostSpecificTagMerger uut(keys);
    uut.setOverwrite1(false);

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void ancestorTest()
  {
    MostSpecificTagMerger uut;
    Tags expected;
    Tags merged;

    // amenity=church_hall is a more specific instance of amenity=hall, so only the
    // amenity=church_hall type tag should be kept. This should work when merging in either
    // direction.

    Tags t1;
    t1["amenity"] = "hall";
    t1["name"] = "Hall 1";

    Tags t2;
    t2["amenity"] = "church_hall";
    t2["name"] = "Hall 2";

    expected["amenity"] = "church_hall";
    expected["name"] = "Hall 1";
    expected["alt_name"] = "Hall 2";

    uut.setOverwrite1(false);
    merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    expected.clear();
    expected["amenity"] = "church_hall";
    expected["name"] = "Hall 2";
    expected["alt_name"] = "Hall 1";

    uut.setOverwrite1(true);
    merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void ancestorGenericTest()
  {
    MostSpecificTagMerger uut;
    Tags expected;
    Tags merged;

    // building=mosque is a more specific instance of building=yes, so only the building=mosque type
    // tag should be kept. This should work when merging in either direction. Note that this
    // ancestral relationship between generic (building=yes) and non-generic features
    // (building=mosque) doesn't currently work as part of the schema and was added as a custom
    // behavior to PreserveTypesTagMerger. We may want to look into why it doesn't work by default
    // when calling OsmSchema::isAncestor at some point.

    Tags t1;
    t1[MetadataTags::Building()] = "yes";
    t1["name"] = "Building 1";

    Tags t2;
    t2[MetadataTags::Building()] = "mosque";
    t2["name"] = "Building 2";

    expected[MetadataTags::Building()] = "mosque";
    expected["name"] = "Building 1";
    expected["alt_name"] = "Building 2";

    uut.setOverwrite1(false);
    merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    expected.clear();
    expected[MetadataTags::Building()] = "mosque";
    expected["name"] = "Building 2";
    expected["alt_name"] = "Building 1";

    uut.setOverwrite1(true);
    merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MostSpecificTagMergerTest, "quick");

}
