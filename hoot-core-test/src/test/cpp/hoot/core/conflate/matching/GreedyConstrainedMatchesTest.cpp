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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchSet.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/merging/MergerCreator.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <iostream>

// Tgs
#include <tgs/StreamUtils.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

class GreedyConstrainedFakeMatch : public Match
{
public:

  GreedyConstrainedFakeMatch() : Match(MatchThresholdPtr()) {}
  GreedyConstrainedFakeMatch(ElementId eid1, ElementId eid2, double p,
    ConstMatchThresholdPtr threshold) :
    Match(threshold),
    _eid1(eid1),
    _eid2(eid2),
    _p(p)
  {}

  void addConflict(ConstMatchPtr conflict)
  {
    _conflicts.insert(conflict);
  }

  virtual const MatchClassification& getClassification() const override
  {
    _c.setMatchP(_p);
    _c.setMissP(1 - _p);
    _c.setReviewP(0.0);
    return _c;
  }

  virtual QString getName() const override { return "Fake Match"; }

  virtual double getProbability() const override { return _p; }

  virtual bool isConflicting(const ConstMatchPtr& other, const ConstOsmMapPtr& /*map*/,
                             const QHash<QString, ConstMatchPtr>& /*matches*/) const override
  {
    QString otherString = other->toString();
    for (MatchSet::iterator it = _conflicts.begin(); it != _conflicts.end(); ++it)
    {
      if (otherString == (*it)->toString())
        return false;
    }
    return true;
  }

  virtual set<pair<ElementId, ElementId>> getMatchPairs() const override
  {
    set<pair<ElementId, ElementId>> result;
    result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
    return result;
  }

  virtual QString toString() const override
  {
    stringstream ss;
    ss << "pairs: " << getMatchPairs() << " p: " << getProbability();
    return QString::fromStdString(ss.str());
  }

  virtual QString getDescription() const override { return ""; }

private:

  mutable MatchClassification _c;
  ElementId _eid1, _eid2;
  double _p;
  MatchSet _conflicts;
};

class GreedyConstrainedFakeCreator : public MergerCreator
{
public:

  virtual bool createMergers(const MatchSet&, vector<MergerPtr>&) const override
  {
    assert(false);
    return false;
  }

  virtual bool isConflicting(const ConstOsmMapPtr& map, ConstMatchPtr m1, ConstMatchPtr m2,
                             const QHash<QString, ConstMatchPtr>& /*matches*/) const
  {
    return m1->isConflicting(m2, map);
  }

  vector<CreatorDescription> getAllCreators() const
  {
    return vector<CreatorDescription>();
  }
};

class GreedyConstrainedMatchesTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(GreedyConstrainedMatchesTest);
  CPPUNIT_TEST(runFindSubgraphsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runFindSubgraphsTest()
  {
    // See this for a visual.
    // https://insightcloud.digitalglobe.com/redmine/attachments/download/1638/Hootenanny%20-%20Graph%20Based%20Conflation%20-%202013-06-21.pptx
    ElementId a1 = ElementId::way(1);
    ElementId a2 = ElementId::way(2);
    ElementId a3 = ElementId::way(3);
    // unused
    //ElementId a4 = ElementId::way(4);

    ElementId b1 = ElementId::way(5);
    ElementId b2 = ElementId::way(5);
    ElementId b3 = ElementId::way(6);

    MatchThresholdPtr mt(new MatchThreshold(0.5, 0.5));
    vector<std::shared_ptr<GreedyConstrainedFakeMatch>> fm(4);
    fm[0].reset(new GreedyConstrainedFakeMatch(a1, b1, 0.8, mt));
    fm[1].reset(new GreedyConstrainedFakeMatch(a2, b1, 1.0, mt));
    fm[2].reset(new GreedyConstrainedFakeMatch(a2, b2, 0.9, mt));
    fm[3].reset(new GreedyConstrainedFakeMatch(a3, b3, 0.9, mt));

    fm[0]->addConflict(fm[1]);
    fm[1]->addConflict(fm[2]);

    ConstOsmMapPtr empty;
    GreedyConstrainedMatches uut(empty);

    uut.addMatches(fm.begin(), fm.end());
    vector<ConstMatchPtr> subsetVector = uut.calculateSubset();

    MatchSet matchSet;
    matchSet.insert(subsetVector.begin(), subsetVector.end());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.9, uut.getScore(), 0.001);
    CPPUNIT_ASSERT_EQUAL((size_t)2, matchSet.size());
    CPPUNIT_ASSERT_EQUAL(true, matchSet.find(fm[1]) != matchSet.end());
    CPPUNIT_ASSERT_EQUAL(true, matchSet.find(fm[3]) != matchSet.end());
  }

  virtual void setUp()
  {
    MergerFactory::getInstance().reset();
    MergerFactory::getInstance().registerCreator(
          MergerCreatorPtr(new GreedyConstrainedFakeCreator()));
  }

  virtual void tearDown()
  {
    MergerFactory::getInstance().reset();
    MergerFactory::getInstance().registerDefaultCreators();
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GreedyConstrainedMatchesTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GreedyConstrainedMatchesTest, "quick");

}

