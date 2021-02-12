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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class OverwriteTagMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OverwriteTagMergerTest);
  CPPUNIT_TEST(overwriteTest);
  CPPUNIT_TEST(overwriteSwapTest);
  CPPUNIT_TEST(overwriteExcludeTest);
  CPPUNIT_TEST(accumulateValuesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void overwriteTest()
  {
    // overwrite t2

    {
      OverwriteTagMerger uut;

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
      t2["name:he"] = QString::fromUtf8("מידלנד המהיר");
      t2["ref"] = "24";
      t2["oneway"] = "true";
      t2["bridge"] = "yes";
      t2["uuid"] = "bar";

      Tags expected;
      expected["highway"] = "trunk";
      expected["name"] = "Midland Expressway";
      expected["name:en"] = "Midland Expressway";
      expected["ref"] = "US 24";
      expected["oneway"] = "yes";
      expected["lanes"] = "2";
      expected["alt_name"] = "24;Midland Expy";
      expected["name:he"] = QString::fromUtf8("מידלנד המהיר");
      expected["bridge"] = "yes";
      expected["uuid"] = "foo;bar";

      Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
      CPPUNIT_ASSERT_EQUAL(expected, merged);
    }

    {
      OverwriteTagMerger uut;

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

      Tags expected;
      expected["highway"] = "primary_link";
      expected["all_weather"] = "yes";
      expected["surface"] = "paved";
      expected["lanes"] = "2";
      expected["oneway"] = "yes";

      Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
      CPPUNIT_ASSERT_EQUAL(expected, merged);
    }
  }

  void overwriteSwapTest()
  {
    // overwrite t1

    OverwriteTagMerger uut(true);

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
    t2["name:he"] = QString::fromUtf8("מידלנד המהיר");
    t2["ref"] = "24";
    t2["oneway"] = "true";
    t2["bridge"] = "yes";
    t2["uuid"] = "bar";

    Tags expected;
    expected["highway"] = "secondary";
    expected["name"] = "Midland Expy";
    expected["name:en"] = "Midland Expressway";
    expected["ref"] = "24";
    expected["oneway"] = "true";
    expected["lanes"] = "2";
    expected["alt_name"] = "US 24";
    expected["name:he"] = QString::fromUtf8("מידלנד המהיר");
    expected["bridge"] = "yes";
    expected["uuid"] = "bar;foo";

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void overwriteExcludeTest()
  {
    // overwrite t2; exclude some tags from being overwritten

    OverwriteTagMerger uut;
    QStringList excludeKeys;
    // tag w/ inheritance
    excludeKeys.append("highway");
    // name tag
    excludeKeys.append("name");
    // text tag
    excludeKeys.append("uuid");
    // tag in ref, not in sec
    excludeKeys.append("lanes");
    // tag in sec, not in ref
    excludeKeys.append("bridge");
    uut.setOverwriteExcludeTagKeys(excludeKeys);

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
    t2["name:he"] = QString::fromUtf8("מידלנד המהיר");
    t2["ref"] = "24";
    t2["oneway"] = "true";
    t2["bridge"] = "yes";
    t2["uuid"] = "bar";

    Tags expected;
    expected["highway"] = "secondary";
    expected["name"] = "Midland Expy";
    expected["name:en"] = "Midland Expressway";
    expected["ref"] = "US 24";
    expected["oneway"] = "yes";
    expected["lanes"] = "2";
    expected["alt_name"] = "24";
    expected["name:he"] = QString::fromUtf8("מידלנד המהיר");
    expected["bridge"] = "yes";
    expected["uuid"] = "bar";

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void accumulateValuesTest()
  {
    // overwrite t1; force some tag to accumulate their values on successive merges

    OverwriteTag1Merger uut;
    QStringList accumulateKeys;
    accumulateKeys.append("b_id");
    accumulateKeys.append("b_id_2");
    uut.setAccumulateValuesTagKeys(accumulateKeys);

    Tags t1;;
    t1["name"] = "Test1";
    t1["a_id"] = "1";

    Tags t2;
    t2["name"] = "Test2";
    t2["b_id"] = "1";

    Tags expected;
    expected["name"] = "Test2";
    expected["alt_name"] = "Test1";
    expected["a_id"] = "1";
    expected["b_id"] = "1";

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    t1 = expected;
    expected.clear();
    t2.clear();

    t2["name"] = "Test3";
    t2["b_id"] = "2";
    t2["b_id_2"] = "1";

    expected["name"] = "Test3";
    expected["alt_name"] = "Test1;Test2";
    expected["a_id"] = "1";
    expected["b_id"] = "1;2";
    expected["b_id_2"] = "1";

    merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);

    t1 = expected;
    expected.clear();
    t2.clear();

    t2["name"] = "Test4";
    t2["b_id_2"] = "2";

    expected["name"] = "Test4";
    expected["alt_name"] = "Test1;Test2;Test3";
    expected["a_id"] = "1";
    expected["b_id"] = "1;2";
    expected["b_id_2"] = "1;2";

    merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OverwriteTagMergerTest, "quick");

}
