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
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/match-graph/MatchGraph.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/MatchClassification.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

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

class FakeMatch : public Match
{
public:
  FakeMatch() : Match(boost::shared_ptr<MatchThreshold>()) {}

  FakeMatch(ElementId eid1, ElementId eid2, double p, const boost::shared_ptr<const MatchThreshold> mt) :
    Match(mt),
    _eid1(eid1),
    _eid2(eid2),
    _p(p)
  {}

  virtual const MatchClassification& getClassification() const
  {
    _c.setMatchP(_p);
    _c.setMissP(1 - _p);
    _c.setReviewP(0.0);
    return _c;
  }

  virtual QString getMatchName() const { return "Fake Match"; }

  virtual double getProbability() const { return _p; }

  virtual bool isConflicting(const Match& /*other*/, const ConstOsmMapPtr& /*map*/) const
  {
    return false;
  }

  virtual set< pair<ElementId, ElementId> > getMatchPairs() const
  {
    set< pair<ElementId, ElementId> > result;
    result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
    return result;
  }

  FakeMatch* init(ElementId eid1, ElementId eid2, double p, ConstMatchThresholdPtr mt)
  {
    _eid1 = eid1;
    _eid2 = eid2;
    _p = p;
    _threshold = mt;
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
  boost::shared_ptr<const MatchThreshold> _threshold;
};

class MatchGraphTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MatchGraphTest);
  CPPUNIT_TEST(runFindSubgraphsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runFindSubgraphsTest()
  {
    ElementId a = ElementId::way(1);
    ElementId b = ElementId::way(2);
    ElementId c = ElementId::way(3);
    ElementId d = ElementId::way(4);
    ElementId e = ElementId::way(5);
    ElementId f = ElementId::way(6);
    vector<const Match*> matches;
    boost::shared_ptr<MatchThreshold> mt(new MatchThreshold(0.5, 0.5));

    // force the pointers to be in order which forces the set to be consistent between runs.
    FakeMatch* fm = new FakeMatch[7];

    matches.push_back(fm[0].init(a, b, 1, mt));
    matches.push_back(fm[1].init(a, c, .1, mt));
    matches.push_back(fm[2].init(c, d, 1, mt));
    // duplicates are silly, but allowable
    matches.push_back(fm[3].init(d, e, .9, mt));
    matches.push_back(fm[4].init(d, e, .9, mt));
    matches.push_back(fm[5].init(c, e, .1, mt));
    // doesn't really make sense, but it shouldn't throw an error.
    matches.push_back(fm[6].init(f, f, 1, mt));

    MatchGraph uut;

    uut.addMatches(matches.begin(), matches.end());

    ConstOsmMapPtr empty;

    vector< set<const Match*, MatchPtrComparator> > subgraphs = uut.findSubgraphs(empty);

    stringstream ss;
    ss << subgraphs;
    CPPUNIT_ASSERT_EQUAL(string("[3]{[1]{pairs: [1]{(Way:1, Way:2)} p: 1}, [3]{pairs: [1]{(Way:3, Way:4)} p: 1, pairs: [1]{(Way:4, Way:5)} p: 0.9, pairs: [1]{(Way:4, Way:5)} p: 0.9}, [1]{pairs: [1]{(Way:6, Way:6)} p: 1}}"),
      ss.str());

    delete [] fm;
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchGraphTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchGraphTest, "quick");

}

