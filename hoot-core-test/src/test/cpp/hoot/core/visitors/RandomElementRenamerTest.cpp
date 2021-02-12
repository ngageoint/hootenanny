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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/RandomElementRenamer.h>

// tbs
#include <tbs/stats/SampleStats.h>

using namespace std;

namespace hoot
{

class RandomElementRenamerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RandomElementRenamerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RandomElementRenamerTest()
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    n1->getTags()["name"] = "strange test";
    n1->getTags()["name:ru"] = QString::fromUtf8("странное испытание");
    n1->getTags()["alt_name"] = "strange;test";
    map->addNode(n1);

    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), 500, 500, 10));
    n2->getTags()["name"] = "little town";
    n2->getTags()["name:he"] = QString::fromUtf8("העיר קטנה");
    n2->getTags()["alt_name"] = "small town;littleville";
    map->addNode(n2);

    boost::minstd_rand rng;
    rng.seed(1);
    RandomElementRenamer v;
    v.setRng(rng);
    v.setChangeProbability(0.3);
    v.setProbability(0.5);
    map->visitRw(v);

    stringstream ss1;
    ss1 << n1->getTags().getNames();

    CPPUNIT_ASSERT_EQUAL(string("[4]{straneg, test, strange test, странное испытание}"), ss1.str());

    stringstream ss2;
    ss2 << n2->getTags().getNames();
    CPPUNIT_ASSERT_EQUAL(string("[4]{nmsal towl, littleville, little town, העיר קטנה}"), ss2.str());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RandomElementRenamerTest, "quick");

}
