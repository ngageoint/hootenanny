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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Hoot
#include <hoot/core/conflate/MatchSet.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MergerCreator.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/conflate/match-graph/GreedyConstrainedMatches.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/MatchClassification.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/conflate/Match.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>

// Standard
#include <iostream>

// Tgs
#include <tgs/StreamUtils.h>

#include "../../TestUtils.h"


namespace hoot
{
using namespace std;
using namespace Tgs;

class ConstrainedFakeMatch : public Match
{
public:
  ConstrainedFakeMatch() : Match(MatchThresholdPtr()) {}

  ConstrainedFakeMatch(ElementId eid1, ElementId eid2, double p,
    ConstMatchThresholdPtr threshold) :
    Match(threshold),
    _eid1(eid1),
    _eid2(eid2),
    _p(p)
  {}

  ConstrainedFakeMatch* addConflict(const Match* conflict)
  {
    _conflicts.insert(conflict);
    return this;
  }

  virtual const MatchClassification& getClassification() const
  {
    _c.setMatchP(_p);
    _c.setMissP(1 - _p);
    _c.setReviewP(0.0);
    return _c;
  }

  virtual QString getMatchName() const { return "Fake Match"; }

  virtual double getProbability() const { return _p; }

  virtual bool isConflicting(const Match& other, const ConstOsmMapPtr& /*map*/) const
  {
    // this isn't a good way to do this since it relies on pointers, but it works for the unit test.
    if (_conflicts.find(&other) == _conflicts.end())
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  virtual set< pair<ElementId, ElementId> > getMatchPairs() const
  {
    set< pair<ElementId, ElementId> > result;
    result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
    return result;
  }

  ConstrainedFakeMatch* init(ElementId eid1, ElementId eid2, double p,
    ConstMatchThresholdPtr threshold)
  {
    _eid1 = eid1;
    _eid2 = eid2;
    _p = p;
    _threshold = threshold;
    return this;
  }

  virtual QString toString() const
  {
    stringstream ss;
    ss << "pairs: " << getMatchPairs() << " p: " << getProbability();
    return QString::fromStdString(ss.str());
  }

  MatchType getType() const { return _threshold->getType(*this); }

private:

  mutable MatchClassification _c;
  ElementId _eid1, _eid2;
  double _p;
  MatchSet _conflicts;
  ConstMatchThresholdPtr _threshold;
};

class ConstrainedFakeCreator : public MergerCreator
{
public:

  virtual bool createMergers(const MatchSet&, vector<Merger*>&) const
  {
    assert(false);
    return false;
  }

  virtual bool isConflicting(const ConstOsmMapPtr& map, const Match* m1, const Match* m2) const
  {
    const ConstrainedFakeMatch* cfm1 = dynamic_cast<const ConstrainedFakeMatch*>(m1);
    const ConstrainedFakeMatch* cfm2 = dynamic_cast<const ConstrainedFakeMatch*>(m2);

    return cfm1->isConflicting(*cfm2, map);
  }

  vector<MergerCreator::Description> getAllCreators() const
  {
    return vector<Description>();
  }

};

class GreedyConstrainedMatchesTest : public CppUnit::TestFixture
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

    vector<const Match*> matches;

    // force the pointers to be in order which forces the set to be consistent between runs.
    ConstrainedFakeMatch* fm = new ConstrainedFakeMatch[4];
    MatchThresholdPtr mt(new MatchThreshold(0.5, 0.5));

    matches.push_back(fm[0].init(a1, b1, 0.8, mt)->addConflict(&fm[1]));
    matches.push_back(fm[1].init(a2, b1, 1, mt)->addConflict(&fm[2]));
    matches.push_back(fm[2].init(a2, b2, 0.9, mt));
    matches.push_back(fm[3].init(a3, b3, 0.9, mt));

    ConstOsmMapPtr empty;
    GreedyConstrainedMatches uut(empty);

    uut.addMatches(matches.begin(), matches.end());
    vector<const Match*> subsetVector = uut.calculateSubset();

    MatchSet matchSet;
    matchSet.insert(subsetVector.begin(), subsetVector.end());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.9, uut.getScore(), 0.001);
    CPPUNIT_ASSERT_EQUAL((size_t)2, matchSet.size());
    CPPUNIT_ASSERT_EQUAL(true, matchSet.find(&fm[1]) != matchSet.end());
    CPPUNIT_ASSERT_EQUAL(true, matchSet.find(&fm[3]) != matchSet.end());
  }

  virtual void setUp()
  {
    MergerFactory::getInstance().clear();
    MergerFactory::getInstance().registerCreator(new ConstrainedFakeCreator());
  }

  virtual void tearDown()
  {
    MergerFactory::getInstance().clear();
    MergerFactory::getInstance().registerDefaultCreators();
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GreedyConstrainedMatchesTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GreedyConstrainedMatchesTest, "quick");

}

