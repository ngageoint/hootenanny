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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchGraph.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

class FakeMatch : public Match
{
public:

  FakeMatch() : Match(std::shared_ptr<MatchThreshold>()) {}
  FakeMatch(ElementId eid1, ElementId eid2, double p,
            const std::shared_ptr<const MatchThreshold>& mt) :
    Match(mt, eid1, eid2),
    _p(p)
  {}

  virtual const MatchClassification& getClassification() const
  {
    _c.setMatchP(_p);
    _c.setMissP(1 - _p);
    _c.setReviewP(0.0);
    return _c;
  }

  virtual double getProbability() const { return _p; }

  virtual bool isConflicting(const ConstMatchPtr& /*other*/, const ConstOsmMapPtr& /*map*/,
                             const QHash<QString, ConstMatchPtr>& /*matches*/) const
  {
    return false;
  }

  virtual set<pair<ElementId, ElementId>> getMatchPairs() const
  {
    set<pair<ElementId, ElementId>> result;
    result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
    return result;
  }

  virtual QString toString() const
  {
    stringstream ss;
    ss << "pairs: " << getMatchPairs() << " p: " << getProbability();
    return QString::fromStdString(ss.str());
  }

  MatchType getType() const { return _threshold->getType(*this); }

  QString getDescription() const override { return ""; }
  QString getName() const override { return "Fake Match"; }
  QString getClassName() const override { return ""; }

private:

  mutable MatchClassification _c;
  double _p;
};

class MatchGraphTest : public HootTestFixture
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

    std::shared_ptr<MatchThreshold> mt = std::make_shared<MatchThreshold>(0.5, 0.5);
    vector<std::shared_ptr<FakeMatch>> fm(7);
    fm[0] = std::make_shared<FakeMatch>(a, b, 1.0, mt);
    fm[1] = std::make_shared<FakeMatch>(a, c, 0.1, mt);
    fm[2] = std::make_shared<FakeMatch>(c, d, 1.0, mt);
    // duplicates are silly, but allowable
    fm[3] = std::make_shared<FakeMatch>(d, e, 0.9, mt);
    fm[4] = std::make_shared<FakeMatch>(d, e, 0.9, mt);
    fm[5] = std::make_shared<FakeMatch>(c, e, 0.1, mt);
    // doesn't really make sense, but it shouldn't throw an error.
    fm[6] = std::make_shared<FakeMatch>(f, f, 1.0, mt);

    MatchGraph uut;

    uut.addMatches(fm.begin(), fm.end());

    ConstOsmMapPtr empty;

    vector<set<ConstMatchPtr, MatchPtrComparator>> subgraphs = uut.findSubgraphs(empty);

    stringstream ss;
    ss << subgraphs;
    CPPUNIT_ASSERT_EQUAL(string("[3]{[1]{pairs: [1]{(Way(1), Way(2))} p: 1}, [3]{pairs: [1]{(Way(3), Way(4))} p: 1, pairs: [1]{(Way(4), Way(5))} p: 0.9, pairs: [1]{(Way(4), Way(5))} p: 0.9}, [1]{pairs: [1]{(Way(6), Way(6))} p: 1}}"),
      ss.str());
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchGraphTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchGraphTest, "quick");

}

