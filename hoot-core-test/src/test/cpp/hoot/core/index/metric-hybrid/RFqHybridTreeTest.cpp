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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/index/metric-hybrid/RFqHybridTree.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>

// Qt
#include <QStringList>

// Standard
#include <math.h>

// Tgs
#include <tgs/Optimization/NelderMead.h>
#include <tgs/System/Time.h>

using namespace geos::geom;
using namespace hoot::hybrid;
using namespace std;

namespace hoot
{

class RFqHybridDummyData
{
public:

  RFqHybridDummyData() {}
  RFqHybridDummyData(const RFqHybridDummyData& dd) :
    _e(dd._e),
    _eid(dd._eid),
    _name(dd._name)
  {}

  RFqHybridDummyData(const QString& name) : _name(name) {}

  RFqHybridDummyData(double x, double y, const QString& name, ElementId eid) :
    _e(x, x, y, y),
    _eid(eid),
    _name(name)
  {}
  RFqHybridDummyData(const Coordinate& c, const QString& name, ElementId eid) :
    _e(c.x, c.x, c.y, c.y),
    _eid(eid),
    _name(name)
  {}

  ElementId getElementId() const { return _eid; }

  const Envelope& getEnvelope() const { return _e; }

  const QString& getMetricElement() const { return _name; }

private:

  Envelope _e;
  ElementId _eid;
  QString _name;
};

std::ostream& operator<<(std::ostream & o, const RFqHybridDummyData& dd)
{
  o << dd.getEnvelope().toString() << " " << dd.getMetricElement() << " " << dd.getElementId();
  return o;
}

class RFqHybridTreeTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RFqHybridTreeTest);
  CPPUNIT_TEST(runEmptyIndexTest);
  CPPUNIT_TEST(runBuildIndexTest);
  CPPUNIT_TEST(runQueryIndexTest);
  CPPUNIT_TEST(runRandomQueryTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runEmptyIndexTest()
  {
    vector<RFqHybridDummyData> keys;
    vector<int> values;

    Tgs::Random::instance()->seed(0);
    RFqHybridTree<RFqHybridDummyData, int, LevenshteinDistance> uut(1, 2, -1, -1);
    uut.buildIndex(keys, values);

    HOOT_STR_EQUALS("", uut.toString());
  }

  void runBuildIndexTest()
  {
    vector<RFqHybridDummyData> keys;
    vector<int> values;

    int c = 0;
    keys.push_back(RFqHybridDummyData(0, 0, "cat", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(1, 0, "car", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(0, 1, "banana", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(1, 1, "apple", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(2, 0, "bat", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(3, 0, "cap", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(4, 0, "bandana", ElementId::node(c++)));

    Tgs::Random::instance()->seed(0);

    for (size_t i = 0; i < keys.size(); ++i)
    {
      values.push_back(i);
    }

    {
      RFqHybridTree<RFqHybridDummyData, int, LevenshteinDistance> uut(1, 2, -1, -1);
      uut.buildIndex(keys, values);

      LOG_VART(uut.toString());
      HOOT_STR_EQUALS("RNode: \n"
                      "  Env[0:1,0:1]\n"
                      "  FqNode: \n"
                      "    d: 1\n"
                      "    Leaf: \n"
                      "      Env[0:0,0:0] cat Node(0) : 0\n"
                      "    d: 2\n"
                      "    Leaf: \n"
                      "      Env[1:1,0:0] car Node(1) : 1\n"
                      "    d: 4\n"
                      "    Leaf: \n"
                      "      Env[0:0,1:1] banana Node(2) : 2\n"
                      "  Env[1:4,0:1]\n"
                      "  FqNode: \n"
                      "    d: 0\n"
                      "    Leaf: \n"
                      "      Env[2:2,0:0] bat Node(4) : 4\n"
                      "    d: 2\n"
                      "    Leaf: \n"
                      "      Env[3:3,0:0] cap Node(5) : 5\n"
                      "    d: 5\n"
                      "    RNode: \n"
                      "      Env[1:1,1:1]\n"
                      "      Leaf: \n"
                      "        Env[1:1,1:1] apple Node(3) : 3\n"
                      "      Env[4:4,0:0]\n"
                      "      Leaf: \n"
                      "        Env[4:4,0:0] bandana Node(6) : 6\n", uut.toString());
    }

    // test what happens if RTree has a smaller depth
    {
      RFqHybridTree<RFqHybridDummyData, int, LevenshteinDistance> uut(1, 2, 3, 5);
      uut.buildIndex(keys, values);

      LOG_VART(uut.toString());
      HOOT_STR_EQUALS("RNode: \n"
                      "  Env[0:1,0:1]\n"
                      "  FqNode: \n"
                      "    d: 5\n"
                      "    RNode: \n"
                      "      Env[0:0,0:0]\n"
                      "      FqNode: \n"
                      "        d: 5\n"
                      "        FqNode: \n"
                      "          d: 1\n"
                      "          Leaf: \n"
                      "            Env[0:0,0:0] cat Node(0) : 0\n"
                      "      Env[0:1,0:1]\n"
                      "      FqNode: \n"
                      "        d: 0\n"
                      "        FqNode: \n"
                      "          d: 5\n"
                      "          Leaf: \n"
                      "            Env[0:0,1:1] banana Node(2) : 2\n"
                      "        d: 5\n"
                      "        FqNode: \n"
                      "          d: 0\n"
                      "          Leaf: \n"
                      "            Env[1:1,0:0] car Node(1) : 1\n"
                      "  Env[1:4,0:1]\n"
                      "  FqNode: \n"
                      "    d: 0\n"
                      "    RNode: \n"
                      "      Env[1:1,1:1]\n"
                      "      FqNode: \n"
                      "        d: 5\n"
                      "        FqNode: \n"
                      "          d: 5\n"
                      "          Leaf: \n"
                      "            Env[1:1,1:1] apple Node(3) : 3\n"
                      "    d: 4\n"
                      "    RNode: \n"
                      "      Env[3:3,0:0]\n"
                      "      FqNode: \n"
                      "        d: 5\n"
                      "        FqNode: \n"
                      "          d: 1\n"
                      "          Leaf: \n"
                      "            Env[3:3,0:0] cap Node(5) : 5\n"
                      "    d: 5\n"
                      "    RNode: \n"
                      "      Env[2:2,0:0]\n"
                      "      FqNode: \n"
                      "        d: 4\n"
                      "        FqNode: \n"
                      "          d: 2\n"
                      "          Leaf: \n"
                      "            Env[2:2,0:0] bat Node(4) : 4\n"
                      "    d: 6\n"
                      "    RNode: \n"
                      "      Env[4:4,0:0]\n"
                      "      FqNode: \n"
                      "        d: 1\n"
                      "        FqNode: \n"
                      "          d: 6\n"
                      "          Leaf: \n"
                      "            Env[4:4,0:0] bandana Node(6) : 6\n", uut.toString());
    }
  }

  void runQueryIndexTest()
  {
    vector<RFqHybridDummyData> keys;
    vector<int> values;

    int c = 0;
    keys.push_back(RFqHybridDummyData(0, 0, "cat", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(1, 0, "car", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(0, 1, "banana", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(1, 1, "apple", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(2, 0, "bat", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(3, 0, "cap", ElementId::node(c++)));
    keys.push_back(RFqHybridDummyData(4, 0, "bandana", ElementId::node(c++)));

    for (size_t i = 0; i < keys.size(); ++i)
    {
      values.push_back(i);
    }

    RFqHybridTree<RFqHybridDummyData, int, LevenshteinDistance> uut;
    uut.buildIndex(keys, values);

    HOOT_STR_EQUALS("[7]{0, 1, 2, 3, 4, 5, 6}", uut.find(Coordinate(0, 0), 5, RFqHybridDummyData("cat"), 10));
    HOOT_STR_EQUALS("[2]{0, 1}", uut.find(Coordinate(0, 0), 1.1, RFqHybridDummyData("cap"), 1));
    HOOT_STR_EQUALS("[3]{1, 4, 5}", uut.find(Coordinate(4, 0), 3.01, RFqHybridDummyData("pat"), 2));
    HOOT_STR_EQUALS("[1]{3}", uut.find(Coordinate(1, 2), 1.01, RFqHybridDummyData("apples"), 2));
    HOOT_STR_EQUALS("[0]{}", uut.find(Coordinate(1, 2), 0.5, RFqHybridDummyData("car"), 10));
  }

  static set<int> bruteForce(Coordinate c, double radius, const RFqHybridDummyData& term, int D,
                      const vector<RFqHybridDummyData>& haystack)
  {
    Envelope e(c.x, c.x, c.y, c.y);
    set<int> result;
    for (size_t i = 0; i < haystack.size(); i++)
    {
      if (e.distance(&haystack[i].getEnvelope()) <= radius &&
        (int)LevenshteinDistance::distance(
            term.getMetricElement(), haystack[i].getMetricElement()) <= D)
      {
        result.insert((int)i);
      }
    }

    return result;
  }

  class BuildKeysVisitor : public ElementConstOsmMapVisitor
  {
  public:

    BuildKeysVisitor(vector<RFqHybridDummyData>& keys) : _keys(keys) {}

    virtual void visit(const std::shared_ptr<const Element>& e)
    {
      if (e->getElementType() == ElementType::Node)
      {
        const std::shared_ptr<const hoot::Node>& n =
          std::dynamic_pointer_cast<const hoot::Node>(e);
        QStringList names = n->getTags().getNames();
        set<QString> nameSet;
        for (int i = 0; i < names.size(); i++)
        {
          nameSet.insert(names[i].toLower());
          nameSet.insert(ToEnglishDictionaryTranslator().toEnglish(names[i]).toLower());
        }
        for (set<QString>::const_iterator it = nameSet.begin(); it != nameSet.end(); ++it)
        {
          RFqHybridDummyData d(n->toCoordinate(), *it, n->getElementId());
          _keys.push_back(d);
        }
      }
    }

    virtual QString getDescription() const {return ""; }
    virtual std::string getClassName() const { return ""; }

  private:

    vector<RFqHybridDummyData>& _keys;
  };

  class OptimizeFunction : public Tgs::NelderMead::Function
  {
  public:
    class TestRun
    {
    public:
      int childCount;
      int bucketSize;
      int rDepth;
      int fqDepth;
      double score;

      bool operator== (const TestRun& s1) const
      {
        return (s1.childCount == childCount && s1.bucketSize == bucketSize &&
                s1.rDepth == rDepth && s1.fqDepth == fqDepth);
      }

      QString toString() const
      {
        QString result = QString("childCount: %1 bucketSize: %2 rDepth: %3 fqDepth: %4 score: %5")
            .arg(childCount).arg(bucketSize).arg(rDepth).arg(fqDepth).arg(score);
        return result;
      }
    };


    OptimizeFunction(vector<RFqHybridDummyData>& keys, vector<int>& values, const Envelope& bounds)
      :
      _keys(keys),
      _values(values),
      _bounds(bounds)
    {

    }

    ~OptimizeFunction()
    {
    }

    void addTestRun(const TestRun& tr)
    {
      _results.push_back(tr);
    }

    virtual double f(Tgs::Vector v)
    {
      LOG_INFO(v[0] << " " << v[1] << " " << v[2] << " " << v[3]);
      TestRun s;
      s.childCount = min(max(2, (int)round(v[0])), 50);
      s.bucketSize = min(max(1, (int)round(v[1])), 10);
      s.rDepth = min(40, max(-1, (int)round(v[2])));
      s.fqDepth = min(40, max(s.rDepth, (int)round(v[3])));

      for (size_t i = 0; i < _results.size(); ++i)
      {
        if (s == _results[i])
        {
          return s.score;
        }
      }

      scoreSettings(s);
      _results.push_back(s);
      LOG_INFO("Score: " << s.score);

      return s.score;
    }

    const TestRun& getBest() const
    {
      double best = -1e12;
      size_t bestI = 0;
      for (size_t i = 0; i < _results.size(); ++i)
      {
        if (_results[i].score > best)
        {
          best = _results[i].score;
          bestI = i;
        }
      }

      return _results[bestI];
    }

    void printResult()
    {
      LOG_INFO("Best: " << getBest().toString());
    }

    void scoreSettings(TestRun& score)
    {
      LOG_DEBUG("childCount: " << score.childCount << " bucketSize: " << score.bucketSize <<
                " rDepth: " << score.rDepth << " fqDepth: " << score.fqDepth);
      RFqHybridTree<RFqHybridDummyData, int, LevenshteinDistance> uut(score.bucketSize,
        score.childCount, score.rDepth, score.fqDepth);
      LOG_DEBUG("Building index.");
      double start = Tgs::Time::getTime();
      uut.buildIndex(_keys, _values);
      double indexBuildTime = Tgs::Time::getTime() - start;
      LOG_DEBUG("Built index.");

      Tgs::Random::instance()->seed(0);
      int testCount = 500;
      double bruteForceTime = 0.0;
      double indexTime = 0.0;
      for (int i = 0; i < testCount; i++)
      {
        double x = Tgs::Random::instance()->generateUniform() * _bounds.getWidth() + _bounds.getMinX();
        double y = Tgs::Random::instance()->generateUniform() * _bounds.getHeight() + _bounds.getMinY();
        Coordinate c(x, y);
        //double radius = Tgs::Random::instance()->generateUniform() * 10;
        double radius = 112000;
        QString searchStr = _keys[Tgs::Random::instance()->generateInt(_keys.size())].getMetricElement();
        int queryD = max(1, (int)(searchStr.size() * .2));

        start = Tgs::Time::getTime();
        //set<int> base = bruteForce(c, radius, searchStr, queryD, _keys);
        bruteForceTime += Tgs::Time::getTime() - start;
        start = Tgs::Time::getTime();
        set<int> r = uut.find(c, radius, searchStr, queryD);
        indexTime += Tgs::Time::getTime() - start;
        //HOOT_STR_EQUALS(base, r);
      }

      LOG_DEBUG("Mean searches: " << (double)uut.resetDistanceCount() / (double)testCount);
      //LOG_DEBUG("Brute force mean time: " << bruteForceTime / (double)testCount);
      LOG_DEBUG("index mean time: " << indexTime / (double)testCount);

      double etc = ((double)indexTime / (double)testCount) * (double)_keys.size() + indexBuildTime;
      //double bruteEtc = ((double)bruteForceTime / (double)testCount) * (double)_keys.size();
      LOG_DEBUG("ETC: " << etc);
      //LOG_DEBUG("Brute force ETC: " << bruteEtc << " speedup: " << bruteEtc / etc);

      score.score = -etc;
    }

  private:
    vector<RFqHybridDummyData>& _keys;
    vector<int>& _values;
    Envelope _bounds;

    vector<TestRun> _results;

  };

  void runRandomQueryTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmPbfReader(true).read("test-files/index/metric-hybrid/TinyGeoNamesOrg.osm.pbf", map);

    MapProjector::projectToPlanar(map);

    vector<RFqHybridDummyData> keys;
    vector<int> values;

    BuildKeysVisitor visitor(keys);
    map->visitRo(visitor);

    for (size_t i = 0; i < keys.size(); ++i)
    {
      values.push_back(i);
    }
    LOG_INFO("Key count: " << keys.size());

    Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(map);

//    OptimizeFunction::TestRun best;
//    best.score = -1e12;
//    for (int i = 0; i < 20; i++)
//    {
//      OptimizeFunction* of = new OptimizeFunction(keys, values, bounds);
//      Tgs::NelderMead optimizer(4, of, 1e-9);
//      Tgs::Vector result;
//      result.prepare(4);

//      for (int j = 0; j < 5; j++)
//      {
//        result[0] = 2 + Tgs::Random::instance()->generateInt(8); // child count
//        result[1] = 1 + Tgs::Random::instance()->generateInt(8); // bucket size
//        result[2] = -1 + Tgs::Random::instance()->generate(20); // r depth
//        result[3] = -1 + Tgs::Random::instance()->generate(30); // fq depth
//        optimizer.step(result, of->f(result));
//      }

//      optimizer.setMaxNoChange(10);
//      while (optimizer.done() == false)
//      {
//        double e = of->f(result);
//        result = optimizer.step(result, e);
//      }

//      OptimizeFunction::TestRun thisBest = of->getBest();
//      if (thisBest.score > best.score)
//      {
//        best = thisBest;
//      }

//      LOG_INFO("###########################");
//      LOG_INFO(best.toString());
//      LOG_INFO("###########################");
//    }

    OptimizeFunction::TestRun best;
    best.bucketSize = 1;
    best.childCount = 5;
    best.rDepth = 7;
    best.fqDepth = 22;
    best.score = -1e12;
    Tgs::Vector v;
    v.prepare(4);
    v[0] = best.childCount;
    v[1] = best.bucketSize;
    v[2] = best.rDepth;
    v[3] = best.fqDepth;
    OptimizeFunction ofDummy(keys, values, bounds);
    ofDummy.f(v);

    OptimizeFunction of(keys, values, bounds);
    of.f(v);
    double delta = 5;
    //int iterations = 200;
    int iterations = 4;
    for (int i = 0; i < iterations; i++)
    {
      Tgs::Random::instance()->seed(i);
      v[0] = bound(1, of.getBest().childCount + rdelta(delta), 10);
      v[1] = bound(1, of.getBest().bucketSize + rdelta(delta), 10);
      v[2] = bound(-1, of.getBest().rDepth + rdelta(delta), 40);
      v[3] = bound(-1, of.getBest().fqDepth + rdelta(delta), 40);
      of.f(v);

      LOG_INFO("###########################");
      LOG_INFO(of.getBest().toString());
      LOG_INFO("###########################");

      delta = 5 - (i / (iterations / 4));
    }

  }

  int rdelta(int delta)
  {
    return Tgs::Random::instance()->generateInt(delta * 2 + 1) - delta;
  }

  int bound(int min, int v, int max)
  {
    v = v < min ? min : v;
    v = v > max ? max : v;
    return v;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RFqHybridTreeTest, "quick");

}
