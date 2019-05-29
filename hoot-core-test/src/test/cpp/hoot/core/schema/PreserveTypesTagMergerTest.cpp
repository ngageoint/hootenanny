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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/PreserveTypesTagMerger.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

/**
 * These tests are fairly sensitive to schema changes.
 */
class PreserveTypesTagMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PreserveTypesTagMergerTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(overlappingKeysTest);
  CPPUNIT_TEST(overwrite1Test);
  CPPUNIT_TEST(overwrite1OverlappingKeysTest);
  CPPUNIT_TEST(skipTagsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    // All the type tags should be preserved, the second name should go to alt_names, and
    // building=yes shouldn't be duplicated

    Tags t1;
    t1["building"] = "yes";
    t1["name"] = "Building 1";
    t1["shop"] = "supermarket";

    Tags t2;
    t2["building"] = "yes";
    t2["name"] = "Building 2";
    t2["amenity"] = "restaurant";

    Tags expected;
    expected["building"] = "yes";
    expected["name"] = "Building 1";
    expected["alt_name"] = "Building 2";
    expected["shop"] = "supermarket";
    expected["amenity"] = "restaurant";

    PreserveTypesTagMerger uut;
    uut.setOverwrite1(false);
    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void overlappingKeysTest()
  {
    // The second name should go into alt_name, and the second building tag should go into alt_types.

    Tags t1;
    t1["name"] = "Building 1";
    t1["building"] = "cathedral";

    Tags t2;
    t2["name"] = "Building 2";
    t2["building"] = "chapel";

    Tags expected;
    expected["name"] = "Building 1";
    expected["alt_name"] = "Building 2";
    expected["building"] = "cathedral";
    expected[PreserveTypesTagMerger::ALT_TYPES_TAG_KEY] = "building=chapel";

    PreserveTypesTagMerger uut;
    uut.setOverwrite1(false);

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    // After this merge, we should see names for the second two buildings in alt_name and their
    // building tags in alt_types.

    Tags t3;
    t3["name"] = "Building 3";
    t3["building"] = "church";

    Tags expected2;
    expected2["name"] = "Building 1";
    expected2["alt_name"] = "Building 2;Building 3";
    expected2["building"] = "cathedral";
    expected2[PreserveTypesTagMerger::ALT_TYPES_TAG_KEY] = "building=chapel;building=church";

    merged = uut.mergeTags(merged, t3, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected2, merged);
  }

  void overwrite1Test()
  {
    // The first name should go in alt_name, and all types should be preserved.

    Tags t1;
    t1["building"] = "yes";
    t1["name"] = "Building 1";
    t1["shop"] = "supermarket";

    Tags t2;
    t2["building"] = "yes";
    t2["name"] = "Building 2";
    t2["amenity"] = "restaurant";

    Tags expected;
    expected["building"] = "yes";
    expected["name"] = "Building 2";
    expected["alt_name"] = "Building 1";
    expected["shop"] = "supermarket";
    expected["amenity"] = "restaurant";

    PreserveTypesTagMerger uut;
    uut.setOverwrite1(true);
    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void overwrite1OverlappingKeysTest()
  {
    // The first name should go into alt_name, and only the second building tag should be preserved.

    Tags t1;
    t1["name"] = "Building 1";
    t1["building"] = "cathedral";

    Tags t2;;
    t2["name"] = "Building 2";
    t2["building"] = "chapel";

    Tags expected;
    expected["name"] = "Building 2";
    expected["alt_name"] = "Building 1";
    expected["building"] = "chapel";
    expected[PreserveTypesTagMerger::ALT_TYPES_TAG_KEY] = "building=cathedral";

    PreserveTypesTagMerger uut;
    uut.setOverwrite1(true);

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    Tags t3;
    t3["name"] = "Building 3";
    t3["building"] = "church";

    Tags expected2;;
    expected2["name"] = "Building 3";
    expected2["alt_name"] = "Building 1;Building 2";
    expected2["building"] = "church";
    expected2[PreserveTypesTagMerger::ALT_TYPES_TAG_KEY] = "building=cathedral;building=chapel";

    merged = uut.mergeTags(merged, t3, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected2, merged);
  }

  void skipTagsTest()
  {
    // The amenity tag should be witheld from the output.

    Tags t1;
    t1["building"] = "yes";
    t1["name"] = "Building 1";
    t1["shop"] = "supermarket";

    Tags t2;
    t2["building"] = "yes";
    t2["name"] = "Building 2";
    t2["amenity"] = "restaurant";

    Tags expected;
    expected["building"] = "yes";
    expected["name"] = "Building 1";
    expected["alt_name"] = "Building 2";
    expected["shop"] = "supermarket";

    std::set<QString> keys;
    keys.insert("amenity");
    PreserveTypesTagMerger uut(keys);
    uut.setOverwrite1(false);

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PreserveTypesTagMergerTest, "quick");

}
