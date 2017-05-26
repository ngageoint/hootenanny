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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

#include "../TestUtils.h"

namespace hoot
{

class OverwriteTagMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OverwriteTagMergerTest);
  CPPUNIT_TEST(overwriteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void overwriteTest()
  {
    //Log::getInstance().setLevel(Log::Info);
    OverwriteTagMerger uut;

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
      compareTags(expected, merged);
    }

    {
      OverwriteTagMerger uut1(true);

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

      Tags merged = uut1.mergeTags(t1, t2, ElementType::Way);
      compareTags(expected, merged);
    }
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

      Tags expected;
      expected["highway"] = "primary_link";
      expected["all_weather"] = "yes";
      expected["surface"] = "paved";
      expected["lanes"] = "2";
      expected["oneway"] = "yes";

      Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
      compareTags(expected, merged);
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
        HOOT_STR_EQUALS(it.key() << "=" << t1[it.key()], it.key() << "=" << t2[it.key()]);
      }
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OverwriteTagMergerTest, "quick");

}
