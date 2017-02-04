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
#include <hoot/core/index/metric-hybrid/RTree.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/visitors/GetTagValuesVisitor.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/Statistics/Random.h>
#include <tgs/System/Time.h>

#include "../../TestUtils.h"

namespace hoot
{
using namespace hybrid;

class DummyData
{
public:
  DummyData() {}
  DummyData(const DummyData& dd) : _e(dd._e) {}
  DummyData(double x, double y) : _e(x, x, y, y) {}

  const Envelope& getEnvelope() const { return _e; }

private:
  Envelope _e;
};

std::ostream& operator<<(std::ostream & o, const DummyData& dd)
{
  o << dd.getEnvelope().toString();
  return o;
}

class RTreeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RTreeTest);
  CPPUNIT_TEST(runBuildIndexTest);
  CPPUNIT_TEST(runQueryIndexTest);
  CPPUNIT_TEST(runRandomQueryTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBuildIndexTest()
  {
    vector<DummyData> keys;
    vector<int> values;

    keys.push_back(DummyData(0, 0));
    keys.push_back(DummyData(1, 0));
    keys.push_back(DummyData(0, 1));
    keys.push_back(DummyData(1, 1));
    keys.push_back(DummyData(2, 0));
    keys.push_back(DummyData(3, 0));
    keys.push_back(DummyData(4, 0));

    for (size_t i = 0; i < keys.size(); ++i)
    {
      values.push_back(i);
    }

    {
      RTree<DummyData, int> uut(2, 1, -1);
      uut.buildIndex(keys, values);

      LOG_VART(uut.toString());
      HOOT_STR_EQUALS("RNode: \n"
                      "  Env[0:1,0:1]\n"
                      "  RNode: \n"
                      "    Env[0:0,0:0]\n"
                      "    Leaf: \n"
                      "      Env[0:0,0:0] : 0\n"
                      "    Env[0:1,0:1]\n"
                      "    RNode: \n"
                      "      Env[1:1,0:0]\n"
                      "      Leaf: \n"
                      "        Env[1:1,0:0] : 1\n"
                      "      Env[0:0,1:1]\n"
                      "      Leaf: \n"
                      "        Env[0:0,1:1] : 2\n"
                      "  Env[1:4,0:1]\n"
                      "  RNode: \n"
                      "    Env[1:2,0:1]\n"
                      "    RNode: \n"
                      "      Env[1:1,1:1]\n"
                      "      Leaf: \n"
                      "        Env[1:1,1:1] : 3\n"
                      "      Env[2:2,0:0]\n"
                      "      Leaf: \n"
                      "        Env[2:2,0:0] : 4\n"
                      "    Env[3:4,0:0]\n"
                      "    RNode: \n"
                      "      Env[3:3,0:0]\n"
                      "      Leaf: \n"
                      "        Env[3:3,0:0] : 5\n"
                      "      Env[4:4,0:0]\n"
                      "      Leaf: \n"
                      "        Env[4:4,0:0] : 6\n", uut.toString());
    }

    {
      RTree<DummyData, int> uut(2, 1, 3);
      uut.buildIndex(keys, values);

      LOG_VART(uut.toString());
      HOOT_STR_EQUALS("RNode: \n"
                      "  Env[0:1,0:1]\n"
                      "  RNode: \n"
                      "    Env[0:0,0:0]\n"
                      "    RNode: \n"
                      "      Env[0:0,0:0]\n"
                      "      Leaf: \n"
                      "        Env[0:0,0:0] : 0\n"
                      "    Env[0:1,0:1]\n"
                      "    RNode: \n"
                      "      Env[1:1,0:0]\n"
                      "      Leaf: \n"
                      "        Env[1:1,0:0] : 1\n"
                      "      Env[0:0,1:1]\n"
                      "      Leaf: \n"
                      "        Env[0:0,1:1] : 2\n"
                      "  Env[1:4,0:1]\n"
                      "  RNode: \n"
                      "    Env[1:2,0:1]\n"
                      "    RNode: \n"
                      "      Env[1:1,1:1]\n"
                      "      Leaf: \n"
                      "        Env[1:1,1:1] : 3\n"
                      "      Env[2:2,0:0]\n"
                      "      Leaf: \n"
                      "        Env[2:2,0:0] : 4\n"
                      "    Env[3:4,0:0]\n"
                      "    RNode: \n"
                      "      Env[3:3,0:0]\n"
                      "      Leaf: \n"
                      "        Env[3:3,0:0] : 5\n"
                      "      Env[4:4,0:0]\n"
                      "      Leaf: \n"
                      "        Env[4:4,0:0] : 6\n", uut.toString());
    }

    {
      RTree<DummyData, int> uut(4, 2, -1);
      uut.buildIndex(keys, values);

      LOG_VART(uut.toString());
      HOOT_STR_EQUALS("RNode: \n"
                      "  Env[0:0,0:0]\n"
                      "  Leaf: \n"
                      "    Env[0:0,0:0] : 0\n"
                      "  Env[0:1,0:1]\n"
                      "  Leaf: \n"
                      "    Env[1:1,0:0] : 1\n"
                      "    Env[0:0,1:1] : 2\n"
                      "  Env[1:2,0:1]\n"
                      "  Leaf: \n"
                      "    Env[1:1,1:1] : 3\n"
                      "    Env[2:2,0:0] : 4\n"
                      "  Env[3:4,0:0]\n"
                      "  Leaf: \n"
                      "    Env[3:3,0:0] : 5\n"
                      "    Env[4:4,0:0] : 6\n", uut.toString());
    }
  }

  void runQueryIndexTest()
  {
    vector<DummyData> keys;
    vector<int> values;

    keys.push_back(DummyData(0, 0));
    keys.push_back(DummyData(1, 0));
    keys.push_back(DummyData(0, 1));
    keys.push_back(DummyData(1, 1));
    keys.push_back(DummyData(2, 0));
    keys.push_back(DummyData(3, 0));
    keys.push_back(DummyData(4, 0));

    for (size_t i = 0; i < keys.size(); ++i)
    {
      values.push_back(i);
    }

    RTree<DummyData, int> uut(2, 1, -1);
    uut.buildIndex(keys, values);
    HOOT_STR_EQUALS("[7]{0, 1, 2, 3, 4, 5, 6}", uut.find(Coordinate(0, 0), 5));
    HOOT_STR_EQUALS("[3]{0, 1, 2}", uut.find(Coordinate(0, 0), 1.1));
    HOOT_STR_EQUALS("[4]{1, 4, 5, 6}", uut.find(Coordinate(4, 0), 3.01));
    HOOT_STR_EQUALS("[1]{3}", uut.find(Coordinate(1, 2), 1.01));
    HOOT_STR_EQUALS("[0]{}", uut.find(Coordinate(1, 2), 0.5));
  }

  set<int> bruteForce(Coordinate c, double radius, const vector<DummyData>& haystack)
  {
    Envelope e(c.x, c.x, c.y, c.y);
    set<int> result;
    for (size_t i = 0; i < haystack.size(); i++)
    {
      if (e.distance(&haystack[i].getEnvelope()) <= radius)
      {
        result.insert((int)i);
      }
    }

    return result;
  }

  void runRandomQueryTest()
  {
    vector<DummyData> keys;
    vector<int> values;

    for (size_t i = 0; i < 1000; ++i)
    {
      double x = Tgs::Random::instance()->generateUniform() * 100.0;
      double y = Tgs::Random::instance()->generateUniform() * 100.0;
      keys.push_back(DummyData(x, y));
      values.push_back(i);
    }

    for (int childCount = 2; childCount < 4; ++childCount)
    for (int bucketSize = childCount; bucketSize < 4; ++bucketSize)
    for (int depth = 0; depth <= 6; depth+=2)
    {
      LOG_DEBUG("childCount: " << childCount << " bucketSize: " << bucketSize << " depth: " <<
               depth);
      RTree<DummyData, int> uut(childCount, bucketSize, depth);
      LOG_DEBUG("Building index.");
      uut.buildIndex(keys, values);
      LOG_DEBUG("Built index.");

      int testCount = 50;
      double bruteForceTime = 0.0;
      double indexTime = 0.0;
      for (int i = 0; i < testCount; i++)
      {
        double x = Tgs::Random::instance()->generateUniform() * 300.0 - 150.0;
        double y = Tgs::Random::instance()->generateUniform() * 300.0 - 150.0;
        Coordinate c(x, y);
        double radius = Tgs::Random::instance()->generateUniform() * 10;
        double start = Tgs::Time::getTime();
        set<int> base = bruteForce(c, radius, keys);
        bruteForceTime += Tgs::Time::getTime() - start;
        start = Tgs::Time::getTime();
        set<int> r = uut.find(c, radius);
        indexTime += Tgs::Time::getTime() - start;
        HOOT_STR_EQUALS(base, r);
      }

      LOG_DEBUG("Mean searches: " << (double)uut.resetDistanceCount() / (double)testCount);
      LOG_DEBUG("Brute force mean time: " << bruteForceTime / (double)testCount);
      LOG_DEBUG("index mean time: " << indexTime / (double)testCount);
    }
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(RTreeTest);

}
