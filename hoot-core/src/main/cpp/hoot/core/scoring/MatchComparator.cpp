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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchComparator.h"

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/MarkForReviewMerger.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/StatusFilter.h>
#include <hoot/core/filters/TagContainsFilter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/TextTable.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/GetTagValuesVisitor.h>
#include <hoot/core/visitors/SetTagVisitor.h>
#include <hoot/core/visitors/SetVisitor.h>
#include <hoot/core/filters/HasTagCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/CountVisitor.h>
#include <hoot/core/filters/ElementTypeCriterion.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * Traverses the OsmMap and creates a map from REF tags to all the uuids that have that REF.
 */
class GetRefUuidVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:
  typedef map<QString, set<QString> > RefToUuid;

  GetRefUuidVisitor(QString ref) : _ref(ref) {}

  virtual ~GetRefUuidVisitor() {}

  const RefToUuid& getRefToUuid() const { return _ref2Uuid; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e)
  {
    QStringList refs;
    if (e->getTags().contains(_ref))
    {
      e->getTags().readValues(_ref, refs);
    }

    // #5891 the MatchScoringMapPreparer should have removed all todos
    if (refs.contains("todo"))
    {
      throw HootException("Unexpected todo found in " + _ref);
    }
    refs.removeAll("none");

    QString uuid = e->getTags()["uuid"];
    if (refs.size() > 0 && uuid.isEmpty())
    {
      LOG_WARN("refs: " << refs);
      LOG_WARN("Element: " << e->toString());
      throw HootException("uuid must be provided on all REF* features.");
    }

    for (int i = 0; i < refs.size(); i++)
    {
      QString r = refs[i].trimmed();
      if (r.isEmpty() == false)
      {
        _ref2Uuid[r].insert(uuid);
      }
    }
  }

private:
  const OsmMap* _map;
  QString _ref;
  RefToUuid _ref2Uuid;
};

/**
 * Traverses the OsmMap and creates a map from uuid tags to ElementIds.
 */
class UuidToEidVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:

  UuidToEidVisitor() {}

  virtual ~UuidToEidVisitor() {}

  const MatchComparator::UuidToEid& getUuidToEid() const { return _uuidToEid; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e)
  {
    QString uuid;
    if (e->getTags().contains("uuid"))
    {
      uuid = e->getTags().get("uuid");
    }

    if (!uuid.isEmpty())
    {
      _uuidToEid.insert(uuid, e->getElementId());

      //If an item was merged during conflation, its uuid is its original uuid prepended or appended
      //with the uuid's of the merged items.  So, add map entries for all the uuid parts, if it
      //contains multiple features.
      const QStringList uuidParts = uuid.split(";");
      for (int i = 0; i < uuidParts.size(); i++)
      {
        _uuidToEid.insert(uuidParts.at(i), e->getElementId());
      }
    }
  }

private:
  const OsmMap* _map;
  QString _ref;
  MatchComparator::UuidToEid _uuidToEid;
};

MatchComparator::MatchComparator()
{
  _tagErrors = true;

  _confusion.resize(3);

  for (size_t i = 0; i < 3; i++)
  {
    _confusion[i].resize(3, 0);
  }
}

void MatchComparator::_addWrong(const Tags& t1, const Tags& t2)
{
  OsmSchemaCategory c1 = OsmSchema::getInstance().getCategories(t1);
  OsmSchemaCategory c2 = OsmSchema::getInstance().getCategories(t2);
  QString s1 = c1.toString();
  QString s2 = c2.toString();

  if (s1.isEmpty())
  {
    s1 = "<no category>";
  }
  if (s2.isEmpty())
  {
    s2 = "<no category>";
  }

  if (s2 < s1)
  {
    swap(s1, s2);
  }

  _wrongBreakdown[s1][s2] = _wrongBreakdown[s1][s2].toInt() + 1;
}

void MatchComparator::_clearCache()
{
  // clears everything but the confusion matrix
  _actual.clear();
  _expected.clear();
  _actualUuidToEid.clear();
  _actualMatchGroups.clear();
  _actualReviewGroups.clear();
  _expectedMatchGroups.clear();
  _expectedReviewGroups.clear();
  _elementWrongCounts.clear();
}

bool MatchComparator::_debugLog(QString uuid1, QString uuid2, const ConstOsmMapPtr& in,
  const ConstOsmMapPtr& /*conflated*/)
{
  TagContainsFilter tcf(Filter::KeepMatches, "uuid", uuid1);
  tcf.addPair("uuid", uuid2);
  SetVisitor sv;
  FilteredVisitor fv2(tcf, sv);
  in->visitRo(fv2);
  const set<ElementId>& s = sv.getElementSet();

  //QStringList interesting = QString("009489").split(";");
  QStringList interesting;

  bool found = false;

  for (set<ElementId>::const_iterator it = s.begin(); it != s.end(); ++it)
  {
    QString ref1 = in->getElement(*it)->getTags()["REF1"];
    QString ref2 = in->getElement(*it)->getTags()["REF2"];
    for (int i = 0; i < interesting.size(); i++)
    {
      if (ref1.contains(interesting[i]) || ref2.contains(interesting[i]))
      {
        found = true;
      }
    }
  }

  if (found)
  {
    LOG_INFO("Miss:");
    for (set<ElementId>::const_iterator it = s.begin(); it != s.end(); ++it)
    {
      cout << "#############" << endl;
      cout << in->getElement(*it)->getTags().toString() << endl;
    }
  }

  return found;
}

double MatchComparator::getPertyScore() const
{
  /*
    match accuracy = (Cest INTERSECTION Cref) / Cref =
     # of members in the intersection set between _actual and _expected) / # of _expected members
    */
  LOG_VARD(_actual.size());
  LOG_VARD(_actual);
  LOG_VARD(_expected.size());
  LOG_VARD(_expected);
  assert(_expected.size() > 0);
  set<UuidPair> intersection;
  set_intersection(_actual.begin(), _actual.end(), _expected.begin(), _expected.end(),
    insert_iterator<std::set<UuidPair> >(intersection, intersection.begin()));
  LOG_VARD(intersection.size());
  LOG_VARD(intersection);
  const double pertyScore = (double)intersection.size() / (double)_expected.size();
  LOG_VARD(pertyScore);
  return pertyScore;
}

double MatchComparator::evaluateMatches(const ConstOsmMapPtr& in, const OsmMapPtr& conflated)
{
  _clearCache();
  // determine the pairwise UUID expected matches
  _findExpectedMatches(in);
  // determine the pairwise UUID actual matches
  _findActualMatches(in, conflated);

  _tp = 0;
  _fp = 0;
  _fn = 0;
  // true negative doesn't have a good definition in this case.

  set<UuidPair> allPairs;
  allPairs.insert(_actual.begin(), _actual.end());
  allPairs.insert(_expected.begin(), _expected.end());

  for (set<UuidPair>::const_iterator it = allPairs.begin(); it != allPairs.end(); ++it)
  {
    int expectedIndex;
    int actualIndex;

    const UuidPair& m = *it;

    // if this is an expected match
    if (_expectedMatchGroups.findT(m.first) == _expectedMatchGroups.findT(m.second))
    {
      expectedIndex = MatchType::Match;
    }
    // if this is an expected review
    else if (_expectedReviewGroups.findT(m.first) == _expectedReviewGroups.findT(m.second))
    {
      expectedIndex = MatchType::Review;
    }
    else
    {
      expectedIndex = MatchType::Miss;
    }

    // if this is an actual match
    if (_actualMatchGroups.findT(m.first) == _actualMatchGroups.findT(m.second))
    {
      actualIndex = MatchType::Match;
    }
    // if this is an actual review
    else if (_isNeedsReview(m.first, m.second, conflated))
    {
      actualIndex = MatchType::Review;
    }
    else
    {
      actualIndex = MatchType::Miss;
    }

    if (actualIndex != expectedIndex)
    {
      if (actualIndex != MatchType::Review)
      {
        QList<ElementId> eid1s =  _actualUuidToEid.values(m.first);
        for (int eidIndex = 0; eidIndex < eid1s.size(); eidIndex++)
        {
          ElementId eid1 = eid1s.at(eidIndex);
          // sometimes elements are removed during conflation. If they were supposed to be matched,
          // then mark it as an error.
          if (!eid1.isNull())
          {
            _tagWrong(conflated, m.first);
          }
        }

        QList<ElementId> eid2s =  _actualUuidToEid.values(m.second);
        for (int eidIndex = 0; eidIndex < eid2s.size(); eidIndex++)
        {
          ElementId eid2 = eid2s.at(eidIndex);
          // sometimes elements are removed during conflation. If they were supposed to be matched,
          // then mark it as an error.
          if (!eid2.isNull())
          {
            _tagWrong(conflated, m.second);
          }
        }

        for (int eid1Index = 0; eid1Index < eid1s.size(); eid1Index++)
        {
          ElementId eid1 = eid1s.at(eid1Index);
          for (int eid2Index = 0; eid2Index < eid2s.size(); eid2Index++)
          {
            ElementId eid2 = eid2s.at(eid2Index);
            if (!eid1.isNull() && !eid2.isNull())
            {
              ElementPtr e1 = conflated->getElement(eid1);
              ElementPtr e2 = conflated->getElement(eid2);
              _addWrong(e1->getTags(), e2->getTags());
            }
          }
        }
      }

      _tagError(conflated, it->first, "1");
      _tagError(conflated, it->second, "2");

      //This info from these tags can be misleading if you are conflating the same data type twice
      //in the same conflation job (e.g. poi to poi AND poi to poly), due to the fact that in
      //those cases multiple actual/expected states can exist and this logic only records one
      //of them.
      //@todo The expected miss/actual review tags are sometimes inaccurate.
      const MatchType expectedMatchType(expectedIndex);
      const MatchType actualMatchType(actualIndex);
      _tagTestOutcome(
        conflated, it->first, expectedMatchType.toString(), actualMatchType.toString());
      _tagTestOutcome(
        conflated, it->second, expectedMatchType.toString(), actualMatchType.toString());
    }

    _confusion[actualIndex][expectedIndex]++;
  }

  _setElementWrongCounts(conflated);

  _tp = _confusion[MatchType::Match][MatchType::Match];
  _fn = _confusion[MatchType::Miss][MatchType::Match] +
      _confusion[MatchType::Review][MatchType::Match];
  _fp = _confusion[MatchType::Match][MatchType::Miss] +
      _confusion[MatchType::Match][MatchType::Review];

  LOG_VAR(_wrongBreakdown);
  LOG_VAR(TextTable(_wrongBreakdown).toWikiString());

  return double(_tp) / double(_tp + _fn + _fp);
}

void MatchComparator::_createMatches(const set<QString>& uuids1, const set<QString>& uuids2,
  set<UuidPair>& matches, Tgs::DisjointSetMap<QString>& groups)
{
  // create a match between all the combinations of ref1 uuid to ref2 uuid
  for (set<QString>::const_iterator u1 = uuids1.begin(); u1 != uuids1.end(); ++u1)
  {
    for (set<QString>::const_iterator u2 = uuids2.begin(); u2 != uuids2.end(); ++u2)
    {
      matches.insert(UuidPair(*u1, *u2));
      groups.joinT(*u1, *u2);
    }
  }
}

void MatchComparator::_findActualMatches(const ConstOsmMapPtr& in, const ConstOsmMapPtr& conflated)
{
  UuidToEidVisitor uuidToEidVisitor;
  conflated->visitRo(uuidToEidVisitor);
  _actualUuidToEid = uuidToEidVisitor.getUuidToEid();

  // read out all the uuids in the conflated data
  set<QString> cUuids;
  GetTagValuesVisitor vc("uuid", cUuids);
  conflated->visitRo(vc);
  //LOG_DEBUG("cUuids size: " << cUuids.size());

  // go through all the reviews in the conflated map
  set<ReviewMarker::ReviewUid> ruuid = ReviewMarker::getReviewUids(conflated);
  for (set<ReviewMarker::ReviewUid>::iterator it = ruuid.begin(); it != ruuid.end(); it++)
  {
    set<QString> u1;
    set<QString> u2;

    set<ElementId> eids = ReviewMarker::getReviewElements(conflated, *it);
    for (set<ElementId>::iterator eid = eids.begin(); eid != eids.end(); eid++)
    {
      ElementId p = *eid;
      QString uuidStr = conflated->getElement(p)->getTags()["uuid"];
      if (uuidStr.isEmpty())
      {
        LOG_WARN("Missing uuid for " + p.toString());
        continue;
      }

      if (conflated->getElement(p)->getStatus() == Status::Unknown1)
      {
        u1.insert(uuidStr);
      }
      else if (conflated->getElement(p)->getStatus() == Status::Unknown2)
      {
        u2.insert(uuidStr);
      }
      else if (u1.size() == 0 && u2.size() > 0)
      {
        u1.insert(uuidStr);
      }
      else if (u2.size() == 0 && u1.size() > 0)
      {
        u2.insert(uuidStr);
      }
      else
      {
        u2.insert(uuidStr);
      }
    }

    // create a match between all the combinations of ref1 uuid to ref2 uuid
    for (set<QString>::const_iterator iit = u1.begin(); iit != u1.end(); ++iit)
    {
      for (set<QString>::const_iterator jt = u2.begin(); jt != u2.end(); ++jt)
      {
        _actual.insert(UuidPair(*iit, *jt));
      }
    }
  }

  // read out all the uuids in in1 and in2
  set<QString> in1Uuids;
  GetTagValuesVisitor gtv1("uuid", in1Uuids);
  StatusCriterion sf1(Status::Unknown1);
  FilteredVisitor fvIn1(sf1, gtv1);
  in->visitRo(fvIn1);
  //LOG_DEBUG("in1Uuids size: " << in1Uuids.size());

  set<QString> in2Uuids;
  GetTagValuesVisitor gtv2("uuid", in2Uuids);
  StatusCriterion sf2(Status::Unknown2);
  FilteredVisitor fvIn2(sf2, gtv2);
  in->visitRo(fvIn2);
  //LOG_DEBUG("in2Uuids size: " << in2Uuids.size());

  for (set<QString>::const_iterator it = cUuids.begin(); it != cUuids.end(); ++it)
  {
    QStringList cList = Tags::split(*it);
    set<QString> u1;
    set<QString> u2;

    for (int i = 0; i < cList.size(); i++)
    {
      // split the uids into two groups.
      if (in1Uuids.find(cList[i]) != in1Uuids.end())
      {
        u1.insert(cList[i]);
        //LOG_DEBUG("Inserted " << cList[i] << " into u1.");
      }
      else if (in2Uuids.find(cList[i]) != in2Uuids.end())
      {
        u2.insert(cList[i]);
        //LOG_DEBUG("Inserted " << cList[i] << " into u2.");
      }
      else
      {
        LOG_WARN("Missing UUID: " << cList[i]);
        throw HootException("Conflated uuid wasn't found in either input.");
      }
    }
    // create a match between all the combinations of ref1 uuid to ref2 uuid
    _createMatches(u1, u2, _actual, _actualMatchGroups);
  }
}

void MatchComparator::_findExpectedMatches(const ConstOsmMapPtr& in)
{
  // extract all of the REF2 values in in2
  GetRefUuidVisitor ref1("REF1");
  in->visitRo(ref1);

  GetRefUuidVisitor ref2("REF2");
  in->visitRo(ref2);

  GetRefUuidVisitor review("REVIEW");
  in->visitRo(review);

  // go through all the ref1 uuids
  for (GetRefUuidVisitor::RefToUuid::const_iterator it = ref1.getRefToUuid().begin();
    it != ref1.getRefToUuid().end(); ++it)
  {
    // match the ref1 to ref2 uuids
    GetRefUuidVisitor::RefToUuid::const_iterator jt = ref2.getRefToUuid().find(it->first);
    if (jt != ref2.getRefToUuid().end())
    {
      const set<QString>& ref1Uuids = it->second;
      const set<QString>& ref2Uuids = jt->second;
      // create a match between all the combinations of ref1 uuid to ref2 uuid
      _createMatches(ref1Uuids, ref2Uuids, _expected, _expectedMatchGroups);
    }

    // match the ref1 to review uuids
    GetRefUuidVisitor::RefToUuid::const_iterator kt = review.getRefToUuid().find(it->first);
    if (kt != review.getRefToUuid().end())
    {
      const set<QString>& ref1Uuids = it->second;
      const set<QString>& reviewUuids = kt->second;
      // create a match between all the combinations of ref1 uuid to review uuid
      _createMatches(ref1Uuids, reviewUuids, _expected, _expectedReviewGroups);
    }
  }
}


double MatchComparator::getPercentCorrect() const
{
  return double(_confusion[MatchType::Match][MatchType::Match] +
    _confusion[MatchType::Miss][MatchType::Miss] +
    _confusion[MatchType::Review][MatchType::Review]) / double(getTotalCount());
}

double MatchComparator::getPercentWrong() const
{
  return double(_confusion[MatchType::Match][MatchType::Miss] +
    _confusion[MatchType::Match][MatchType::Review] +
    _confusion[MatchType::Miss][MatchType::Match] +
    _confusion[MatchType::Miss][MatchType::Review]) / double(getTotalCount());
}

double MatchComparator::getPercentUnnecessaryReview() const
{
  return double(_confusion[MatchType::Review][MatchType::Match] +
    _confusion[MatchType::Review][MatchType::Miss]) / double(getTotalCount());
}

int MatchComparator::getTotalCount() const
{
  int result = 0;
  for (size_t i = 0; i < _confusion.size(); i++)
  {
    for (size_t j = 0; j < _confusion[i].size(); j++)
    {
      result += _confusion[i][j];
    }
  }

  return result;
}

bool MatchComparator::_isNeedsReview(QString uuid1, QString uuid2, const ConstOsmMapPtr& conflated)
{
  QList<ElementId> eid1s = _actualUuidToEid.values(uuid1);
  QList<ElementId> eid2s = _actualUuidToEid.values(uuid2);

  bool result = false;
  for (int eid1Index = 0; eid1Index < eid1s.size(); eid1Index++)
  {
    ElementId eid1 = eid1s.at(eid1Index);
    for (int eid2Index = 0; eid2Index < eid2s.size(); eid2Index++)
    {
      ElementId eid2 = eid2s.at(eid2Index);
      if (eid1.isNull() || eid2.isNull())
      {
        //So far this message is ok, change from LOG_INFO to LOG_DEBUG.
        //More information please see issue 167. https://github.com/ngageoint/hootenanny/issues/167
        LOG_DEBUG("No actual element exists.");
        return false;
      }

      if (ReviewMarker().isNeedsReview(conflated, conflated->getElement(eid1),
        conflated->getElement(eid2)))
      {
        result = true;
      }
    }
  }
  return result;
}

void MatchComparator::_tagTestOutcome(const OsmMapPtr& map, const QString uuid,
                                      const QString expected, const QString actual)
{
  SetTagVisitor stv1("hoot:expected", expected);
  MatchComparator::UuidToEid::iterator it = _actualUuidToEid.begin();
  while (it != _actualUuidToEid.end())
  {
    if (it.key().contains(uuid))
    {
      shared_ptr<Element> eid = map->getElement(it.value());
      stv1.visit(eid);
    }
    it++;
  }

  SetTagVisitor stv2("hoot:actual", actual);
  it = _actualUuidToEid.begin();
  while (it != _actualUuidToEid.end())
  {
    if (it.key().contains(uuid))
    {
      shared_ptr<Element> eid = map->getElement(it.value());
      stv2.visit(eid);
    }
    it++;
  }
}

void MatchComparator::_tagError(const OsmMapPtr &map, const QString &uuid, const QString& value)
{
  // if the uuid contains the first uuid, set mismatch
  SetTagVisitor stv("hoot:mismatch", value);
  MatchComparator::UuidToEid::iterator it = _actualUuidToEid.begin();
  while (it != _actualUuidToEid.end())
  {
    if (it.key().contains(uuid))
    {
      shared_ptr<Element> eid = map->getElement(it.value());
      stv.visit(eid);
    }
    it++;
  }
}

void MatchComparator::_tagWrong(const OsmMapPtr &map, const QString &uuid)
{
  // if the uuid contains the first uuid, set mismatch
  SetTagVisitor stv("hoot:wrong", "1");
  MatchComparator::UuidToEid::iterator it = _actualUuidToEid.begin();
  while (it != _actualUuidToEid.end())
  {
    if (it.key().contains(uuid))
    {
      shared_ptr<Element> eid = map->getElement(it.value());
      stv.visit(eid);
    }
    it++;
  }
}

void MatchComparator::_setElementWrongCounts(const ConstOsmMapPtr& map)
{
  _setElementWrongCount(map, ElementType::Node);
  _setElementWrongCount(map, ElementType::Way);
  _setElementWrongCount(map, ElementType::Relation);
}

void MatchComparator::_setElementWrongCount(const ConstOsmMapPtr& map,
                                            const ElementType::Type& elementType)
{
  FilteredVisitor elementWrongVisitor(
    new ChainCriterion(
      new ElementTypeCriterion(elementType),
      new HasTagCriterion("hoot:wrong")),
    new CountVisitor());
  FilteredVisitor& filteredVisitor = const_cast<FilteredVisitor&>(elementWrongVisitor);
  SingleStatistic* singleStat =
    dynamic_cast<SingleStatistic*>(&elementWrongVisitor.getChildVisitor());
  map->visitRo(filteredVisitor);
  _elementWrongCounts[elementType] = singleStat->getStat();
}

QString MatchComparator::toString() const
{
  QString result;
  int total = 0;
  int correct = 0;

  QString left[3];
  // weird markup makes a pretty table in redmine.
  result += "|                 |        |\\3=.       expected     |\n";
  result += "|                 |        | miss  | match | review |\n";
  left[0] = "|/3. test outcome | miss  ";
  left[1] = "                  | match ";
  left[2] = "                  | review";
  for (size_t i = 0; i < 3; i++)
  {
    result += left[i];
    for (size_t j = 0; j < 3; j++)
    {
      if (i == 0 && j == 0)
      {
        result += QString(" |   -  ");
      }
      else
      {
        result += QString(" |%1").arg(_confusion[i][j], 6, 10);
      }
      total += _confusion[i][j];
      if (i == j)
      {
        correct += _confusion[i][j];
      }
    }
    result += "  |\n";
  }

  result += "\n";
  result += QString("correct: %1\n").arg(getPercentCorrect());
  result += QString("wrong: %1\n").arg(getPercentWrong());
  result += QString("unnecessary reviews: %1\n").arg(getPercentUnnecessaryReview());

  result += "\n";
  result += QString("elements involved in a wrong match:\n");
  result += QString("  nodes: %1\n").arg(_elementWrongCounts[ElementType::Node]);
  result += QString("  ways: %1\n").arg(_elementWrongCounts[ElementType::Way]);
  result += QString("  relations: %1\n").arg(_elementWrongCounts[ElementType::Relation]);
  const int totalCorrect =
    _confusion[MatchType::Match][MatchType::Match] +
    _confusion[MatchType::Review][MatchType::Review];
  result += QString("correct match count: %1\n").arg(totalCorrect);
  result += QString("wrong match count: %1\n")
              .arg(_confusion[MatchType::Miss][MatchType::Match] +
                   _confusion[MatchType::Miss][MatchType::Review] +
                   _confusion[MatchType::Match][MatchType::Miss] +
                   _confusion[MatchType::Match][MatchType::Review]);
  const int totalUnnecessaryReviews =
    _confusion[MatchType::Review][MatchType::Miss] +
    _confusion[MatchType::Review][MatchType::Match];
  result += QString("unnecessary review count: %1\n").arg(totalUnnecessaryReviews);
  result += QString("ratio of unnecessary reviews to correct matches: %1\n")
    .arg((double)totalUnnecessaryReviews / (double)totalCorrect);

  return result;
}

}
