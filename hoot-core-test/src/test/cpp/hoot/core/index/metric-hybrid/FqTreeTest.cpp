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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/index/metric-hybrid/FqTree.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/visitors/GetTagValuesVisitor.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/System/Time.h>

#include "../../TestUtils.h"

namespace hoot
{
using namespace hybrid;

class FqDummyData
{
public:
  FqDummyData() {}
  FqDummyData(const FqDummyData& dd) : _str(dd._str) {}
  FqDummyData(const char* s) : _str(s) {}
  FqDummyData(const QString& s) : _str(s) {}

  const QString& getMetricElement() const { return _str; }

  QString _str;
};

std::ostream& operator<<(std::ostream & o, const FqDummyData& dd)
{
  o << dd.getMetricElement();
  return o;
}

class FqTreeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(FqTreeTest);
  CPPUNIT_TEST(runBuildIndexTest);
  CPPUNIT_TEST(runQueryIndexTest);
  CPPUNIT_TEST(runRandomQueryTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBuildIndexTest()
  {
    FqTree<FqDummyData, int, LevenshteinDistance> uut;

    vector<FqDummyData> names;
    vector<int> data;

    names.push_back("apple");
    names.push_back("car");
    names.push_back("cat");
    names.push_back("banana");
    names.push_back("bandana");

    Tgs::Random::instance()->seed(0);

    for (size_t i = 0; i < names.size(); ++i)
    {
      data.push_back(i);
    }

    uut.setDepth(3);
    uut.buildIndex(names, data);
    LOG_VART(uut.toString());
    CPPUNIT_ASSERT_EQUAL(string("FqNode: \n"
                                "  d: 0\n"
                                "  FqNode: \n"
                                "    d: 5\n"
                                "    FqNode: \n"
                                "      d: 5\n"
                                "      Leaf: \n"
                                "        banana : 3\n"
                                "  d: 1\n"
                                "  FqNode: \n"
                                "    d: 6\n"
                                "    FqNode: \n"
                                "      d: 6\n"
                                "      Leaf: \n"
                                "        bandana : 4\n"
                                "  d: 5\n"
                                "  FqNode: \n"
                                "    d: 0\n"
                                "    FqNode: \n"
                                "      d: 1\n"
                                "      Leaf: \n"
                                "        car : 1\n"
                                "    d: 1\n"
                                "    FqNode: \n"
                                "      d: 0\n"
                                "      Leaf: \n"
                                "        cat : 2\n"
                                "    d: 5\n"
                                "    FqNode: \n"
                                "      d: 5\n"
                                "      Leaf: \n"
                                "        apple : 0\n"), uut.toString().toStdString());

    uut.setDepth(-1);
    uut.setBucketSize(10);
    uut.buildIndex(names, data);
    LOG_VART(uut.toString());
    CPPUNIT_ASSERT_EQUAL(string("Leaf: \n"
                                "  apple : 0\n"
                                "  car : 1\n"
                                "  cat : 2\n"
                                "  banana : 3\n"
                                "  bandana : 4\n"), uut.toString().toStdString());

    uut.setDepth(-1);
    uut.setBucketSize(2);
    uut.buildIndex(names, data);
    LOG_VART(uut.toString());
    CPPUNIT_ASSERT_EQUAL(string("FqNode: \n"
                                "  d: 0\n"
                                "  Leaf: \n"
                                "    apple : 0\n"
                                "  d: 5\n"
                                "  FqNode: \n"
                                "    d: 0\n"
                                "    Leaf: \n"
                                "      car : 1\n"
                                "    d: 1\n"
                                "    Leaf: \n"
                                "      cat : 2\n"
                                "    d: 5\n"
                                "    Leaf: \n"
                                "      banana : 3\n"
                                "  d: 6\n"
                                "  Leaf: \n"
                                "    bandana : 4\n"), uut.toString().toStdString());
  }

  void runQueryIndexTest()
  {
    FqTree<FqDummyData, int, LevenshteinDistance> uut;

    vector<FqDummyData> names;
    vector<int> data;

    names.push_back("apple");
    names.push_back("car");
    names.push_back("cat");
    names.push_back("banana");
    names.push_back("bandana");

    for (size_t i = 0; i < names.size(); ++i)
    {
      data.push_back(i);
    }

    uut.setDepth(3);
    uut.buildIndex(names, data);
    LOG_TRACE(uut.toString());
    HOOT_STR_EQUALS("[2]{1, 2}", uut.find("car", 2));
    HOOT_STR_EQUALS("[4]{1, 2, 3, 4}", uut.find("band", 4));
    HOOT_STR_EQUALS("[2]{3, 4}", uut.find("bananas", 2));
  }

  set<int> bruteForce(const FqDummyData& needle, const vector<FqDummyData>& haystack, int D)
  {
    set<int> result;
    for (size_t i = 0; i < haystack.size(); i++)
    {
      if ((int)LevenshteinDistance::distance(needle.getMetricElement(),
        haystack[i].getMetricElement()) <= D)
      {
        result.insert((int)i);
      }
    }

    return result;
  }

  static int count;

  class TestDistance
  {
  public:

    TestDistance()
    {
    }

    int operator() (const QString& s1, const QString& s2) const
    {
      FqTreeTest::count++;
      LOG_TRACE("compare: " << s1 << " " << s2);
      return LevenshteinDistance::distance(s1, s2);
    }
  };

  void runRandomQueryTest()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmPbfReader(true).read("test-files/index/hybrid/TinyGeoNamesOrg.osm.pbf", map);

    set<QString> bag;
    GetTagValuesVisitor v1("name", bag, true);
    GetTagValuesVisitor v2("alt_name", bag, true);
    map->visitRo(v1);
    map->visitRo(v2);

    vector<FqDummyData> names;
    vector<int> data;

    int i = 0;
    for (set<QString>::iterator it = bag.begin(); it != bag.end() && names.size() < 300; ++it)
    {
      names.push_back(*it);
      data.push_back(i++);
    }

    for (int depth = 5; depth <= 10; depth+= 5)
    {
      FqTree<FqDummyData, int, TestDistance> uut;
      uut.setDepth(depth);
      //uut.setBucketSize(2);
      LOG_DEBUG("Building index.");
      uut.buildIndex(names, data);
      LOG_DEBUG("Built index.");

      LOG_DEBUG("Depth: " << depth);
      int testCount = 50;
      FqTreeTest::count = 0;
      double bruteForceTime = 0.0;
      double indexTime = 0.0;
      for (int i = 0; i < testCount; i++)
      {
        QString n = names[Tgs::Random::instance()->generateInt(names.size())].getMetricElement();
        int D = max(1, (int)(n.size() * .2));
        double start = Tgs::Time::getTime();
        set<int> base = bruteForce(n, names, D);
        bruteForceTime += Tgs::Time::getTime() - start;
        start = Tgs::Time::getTime();
        set<int> r = uut.find(n, D);
        indexTime += Tgs::Time::getTime() - start;
        HOOT_STR_EQUALS(base, r);
      }

      LOG_DEBUG("Mean searches: " << FqTreeTest::count / testCount);
      LOG_DEBUG("Brute force mean time: " << bruteForceTime / (double)testCount);
      LOG_DEBUG("index mean time: " << indexTime / (double)testCount);
    }
  }
};

int FqTreeTest::count = 0;

CPPUNIT_TEST_SUITE_REGISTRATION(FqTreeTest);

}
