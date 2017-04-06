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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/perty/PertyRemoveTagVisitor.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>

using namespace std;

namespace hoot
{

class PertyRemoveTagVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PertyRemoveTagVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runExemptTagKeysTest);
  CPPUNIT_TEST(runSubstituteValuesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMap::resetCounters();

    boost::shared_ptr<OsmMap> map(new OsmMap());
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
    PertyRemoveTagVisitor v;
    v.setRng(rng);
    v.setProbability(0.5);
    map->visitRw(v);

    stringstream ss1;
    ss1 << n1->getTags().keys();
    CPPUNIT_ASSERT_EQUAL(string("[2]{name:ru, alt_name}"), ss1.str());

    stringstream ss2;
    ss2 << n2->getTags().keys();
    CPPUNIT_ASSERT_EQUAL(string("[1]{name}"), ss2.str());
  }

  void runExemptTagKeysTest()
  {
    OsmMap::resetCounters();

    boost::shared_ptr<OsmMap> map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    n1->getTags()[MetadataTags::Ref1()] = MetadataTags::Ref1();
    n1->getTags()[MetadataTags::Ref2()] = MetadataTags::Ref2();
    map->addNode(n1);

    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), 500, 500, 10));
    n2->getTags()[MetadataTags::Ref1()] = MetadataTags::Ref1();
    n2->getTags()[MetadataTags::Ref2()] = MetadataTags::Ref2();
    map->addNode(n2);

    boost::minstd_rand rng;
    rng.seed(1);
    PertyRemoveTagVisitor v;
    v.setRng(rng);
    v.setProbability(1.0);
    QStringList exemptTagKeys;
    exemptTagKeys.append(MetadataTags::Ref1());
    v.setExemptTagKeys(exemptTagKeys);
    map->visitRw(v);

    CPPUNIT_ASSERT_EQUAL(1, n1->getTags().size());
    stringstream ss1;
    ss1 << n1->getTags().keys();
    CPPUNIT_ASSERT_EQUAL(string("[1]{" + MetadataTags::Ref1().toStdString() + "}"), ss1.str());

    CPPUNIT_ASSERT_EQUAL(1, n2->getTags().size());
    stringstream ss2;
    ss2 << n2->getTags().keys();
    CPPUNIT_ASSERT_EQUAL(string("[1]{" + MetadataTags::Ref1().toStdString() + "}"), ss2.str());
  }

  void runSubstituteValuesTest()
  {
    OsmMap::resetCounters();

    boost::shared_ptr<OsmMap> map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    n1->getTags()[MetadataTags::Ref1()] = MetadataTags::Ref1();
    n1->getTags()[MetadataTags::Ref2()] = MetadataTags::Ref2();
    map->addNode(n1);

    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), 500, 500, 10));
    n2->getTags()["highway"] = "blah";
    n2->getTags()[MetadataTags::Ref2()] = MetadataTags::Ref2();
    map->addNode(n2);

    boost::minstd_rand rng;
    rng.seed(1);
    PertyRemoveTagVisitor v;
    v.setRng(rng);
    v.setProbability(1.0);
    v.setExemptTagKeys(QStringList());
    QStringList replacementTagKeys;
    replacementTagKeys.append("highway");
    v.setReplacementTagKeys(replacementTagKeys);
    QStringList replacementTagValues;
    replacementTagValues.append("road");
    v.setReplacementTagValues(replacementTagValues);
    map->visitRw(v);

    CPPUNIT_ASSERT_EQUAL(0, n1->getTags().size());

    CPPUNIT_ASSERT_EQUAL(1, n2->getTags().size());
    stringstream ss2;
    ss2 << n2->getTags().keys() << n2->getTags().values();
    CPPUNIT_ASSERT_EQUAL(string("[1]{highway}[1]{road}"), ss2.str());
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyRemoveTagVisitorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyRemoveTagVisitorTest, "quick");

}
