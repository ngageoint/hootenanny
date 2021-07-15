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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/schema/TranslatedTagDifferencer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class TranslateTagDifferencerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TranslateTagDifferencerTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runTestMulti);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    TranslatedTagDifferencer uut;
    Settings s;
    s.set(ConfigOptions::getTranslatedTagDifferencerScriptKey(),
          "translations/HootTest.js");
    uut.setConfiguration(s);

    OsmMapPtr map = std::make_shared<OsmMap>();
    WayPtr w1 = std::make_shared<Way>(Status::Unknown1, -1, 0);
    w1->getTags()["name"] = "foo";
    w1->getTags()["highway"] = "road";
    w1->getTags()["sidewalk"] = "both";

    WayPtr w2 = std::make_shared<Way>(Status::Unknown1, -1, 0);
    w2->getTags()["name"] = "foo";
    w2->getTags()["highway"] = "road";
    w2->getTags()["sidewalk"] = "left";

    // should be a small diff b/c of the UFI
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 / 7.0, uut.diff(map, w1, w2), 1e-5);

    // ignore the UFI and make sure there is no diff.
    s.set(ConfigOptions::getTranslatedTagDifferencerIgnoreListKey(), "UFI");
    uut.setConfiguration(s);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, uut.diff(map, w1, w2), 1e-9);

    // see what happens if we make the surface types different
    w1->getTags()["highway"] = "secondary";
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 / 6.0, uut.diff(map, w1, w2), 1e-5);

    // now make the surface types the same again
    w2->getTags()["highway"] = "secondary";
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, uut.diff(map, w1, w2), 1e-5);
  }

  void runTestMulti()
  {
    TranslatedTagDifferencer uut;
    Settings s;
    s.set(ConfigOptions::getTranslatedTagDifferencerScriptKey(),
          "translations/HootTest.js");
    // ignore the UFI
    s.set(ConfigOptions::getTranslatedTagDifferencerIgnoreListKey(), "UFI");
    uut.setConfiguration(s);

    OsmMapPtr map = std::make_shared<OsmMap>();
    WayPtr w1 = std::make_shared<Way>(Status::Unknown1, -1, 0);
    w1->getTags()["name"] = "foo";
    w1->getTags()["highway"] = "road";

    WayPtr w2 = std::make_shared<Way>(Status::Unknown1, -1, 0);
    w2->getTags()["name"] = "foo";
    w2->getTags()["highway"] = "road";
    w2->getTags()["bridge"] = "yes";

    // creates one record in w1 and two in w2.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0 / 11.0, uut.diff(map, w1, w2), 1e-5);

    WayPtr w3 = std::make_shared<Way>(Status::Unknown1, -1, 0);
    w3->getTags()["name"] = "foo";
    w3->getTags()["highway"] = "road";
    w3->getTags()["bridge"] = "yes";

    WayPtr w4 = std::make_shared<Way>(Status::Unknown1, -1, 0);
    w4->getTags()["name"] = "bar";
    w4->getTags()["highway"] = "road";
    w4->getTags()["bridge"] = "yes";

    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 / 11.0, uut.diff(map, w3, w4), 1e-5);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TranslateTagDifferencerTest, "quick");

}

