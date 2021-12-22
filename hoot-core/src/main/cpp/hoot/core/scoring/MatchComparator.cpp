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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MatchComparator.h"

// hoot
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagContainsCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/TextTable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/visitors/UniqueTagValuesVisitor.h>

using namespace std;

namespace hoot
{

int MatchComparator::logWarnCount = 0;

/**
 * Traverses the OsmMap and creates a map from REF tags to all the uuids that have that REF.
 */
class GetRefUuidVisitor : public ConstElementVisitor
{
public:

  using RefToUuid = map<QString, set<QString>>;

  GetRefUuidVisitor(QString ref) : _ref(ref) { }
  ~GetRefUuidVisitor() override = default;

  const RefToUuid& getRefToUuid() const { return _ref2Uuid; }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void visit(const ConstElementPtr& e) override
  {
    QStringList refs;
    if (e->getTags().contains(_ref))
      e->getTags().readValues(_ref, refs);

    // #5891 the MatchScoringMapPreparer should have removed all todos
    if (refs.contains("todo"))
      throw HootException("Unexpected todo found in " + _ref);
    refs.removeAll("none");

    QString uuid = e->getTags()["uuid"];
    if (!refs.empty() && uuid.isEmpty())
    {
      LOG_TRACE("refs: " << refs);
      LOG_TRACE("Element: " << e->toString());
      throw HootException("uuid must be provided on all REF* features.");
    }

    for (const auto& r : refs)
    {
      if (r.isEmpty() == false)
        _ref2Uuid[r].insert(uuid);
    }
  }

private:

  QString _ref;
  RefToUuid _ref2Uuid;
};

/**
 * Traverses the OsmMap and creates a map from uuid tags to ElementIds.
 */
class UuidToEidVisitor : public ConstElementVisitor
{
public:

  UuidToEidVisitor() = default;
  ~UuidToEidVisitor() override = default;

  const MatchComparator::UuidToEid& getUuidToEid() const { return _uuidToEid; }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void visit(const ConstElementPtr& e) override
  {
    QString uuid;
    if (e->getTags().contains("uuid"))
      uuid = e->getTags().get("uuid");

    if (!uuid.isEmpty())
    {
      _uuidToEid.insert(uuid, e->getElementId());

      //If an item was merged during conflation, its uuid is its original uuid prepended or appended
      //with the uuid's of the merged items.  So, add map entries for all the uuid parts, if it
      //contains multiple features.
      const QStringList uuidParts = uuid.split(";");
      for (const auto& part : uuidParts)
        _uuidToEid.insert(part, e->getElementId());
    }
  }

private:

  QString _ref;
  MatchComparator::UuidToEid _uuidToEid;
};

MatchComparator::MatchComparator()
  : _tagErrors(true),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  //  Initialize the confustion matrix
  for (size_t i = 0; i < CONFUSION_SIZE; ++i)
  {
    for (size_t j = 0; j < CONFUSION_SIZE; ++j)
      _confusion[i][j] = 0;
  }
}

void MatchComparator::_addWrong(const Tags& t1, const Tags& t2)
{
  OsmSchemaCategory c1 = OsmSchema::getInstance().getCategories(t1);
  OsmSchemaCategory c2 = OsmSchema::getInstance().getCategories(t2);
  QString s1 = c1.toString();
  QString s2 = c2.toString();

  if (s1.isEmpty())
    s1 = "<no category>";
  if (s2.isEmpty())
    s2 = "<no category>";
  if (s2 < s1)
    swap(s1, s2);

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

bool MatchComparator::_debugLog(const QString& uuid1, const QString& uuid2, const ConstOsmMapPtr& in,
                                const ConstOsmMapPtr& /*conflated*/) const
{
  TagContainsCriterion tcf("uuid", uuid1);
  tcf.addPair("uuid", uuid2);
  UniqueElementIdVisitor sv;
  FilteredVisitor fv2(tcf, sv);
  in->visitRo(fv2);
  const set<ElementId>& s = sv.getElementSet();

  QStringList interesting;

  bool found = false;

  for (const auto& eid : s)//set<ElementId>::const_iterator it = s.begin(); it != s.end(); ++it)
  {
    QString ref1 = in->getElement(eid)->getTags()[MetadataTags::Ref1()];
    QString ref2 = in->getElement(eid)->getTags()[MetadataTags::Ref2()];
    for (const auto& value : interesting)//int i = 0; i < interesting.size(); i++)
    {
      if (ref1.contains(value) || ref2.contains(value))
        found = true;
    }
  }

  if (found)
  {
    LOG_INFO("Miss:");
    for (set<ElementId>::const_iterator it = s.begin(); it != s.end(); ++it)
    {
      LOG_INFO("#############");
      LOG_INFO(in->getElement(*it)->getTags().toString());
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
  LOG_VART(_actual);
  LOG_VARD(_expected.size());
  LOG_VART(_expected);
  if(_expected.size() <= 0)
  {
    throw HootException(
      "Expected matches size equal to zero. Are there any matches in the conflated data?");
  }
  set<UuidPair> intersection;
  set_intersection(_actual.begin(), _actual.end(), _expected.begin(), _expected.end(),
    insert_iterator<std::set<UuidPair>>(intersection, intersection.begin()));
  LOG_VARD(intersection.size());
  LOG_VART(intersection);
  const double pertyScore = static_cast<double>(intersection.size()) / static_cast<double>(_expected.size());
  LOG_VARD(pertyScore);
  return pertyScore;
}

double MatchComparator::evaluateMatches(const ConstOsmMapPtr& in, const OsmMapPtr& conflated)
{
  LOG_DEBUG("Evaluating matches...");

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

  long numPairsParsed = 0;
  for (const auto& m : allPairs)
  {
    int expectedIndex;
    int actualIndex;

    //  expected match
    if (_expectedMatchGroups.findT(m.first) == _expectedMatchGroups.findT(m.second))
      expectedIndex = MatchType::Match;
    else if (_expectedReviewGroups.findT(m.first) == _expectedReviewGroups.findT(m.second)) //  expected review
      expectedIndex = MatchType::Review;
    else
      expectedIndex = MatchType::Miss;

    // if this is an actual match
    if (_actualMatchGroups.findT(m.first) == _actualMatchGroups.findT(m.second))
      actualIndex = MatchType::Match;
    else if (_isNeedsReview(m.first, m.second, conflated))  //  actual review
      actualIndex = MatchType::Review;
    else
      actualIndex = MatchType::Miss;

    if (actualIndex != expectedIndex)
    {
      if (actualIndex != MatchType::Review)
      {
        QList<ElementId> eid1s =  _actualUuidToEid.values(m.first);
        for (const auto& eid1 : eid1s)
        {
          // sometimes elements are removed during conflation. If they were supposed to be matched,
          // then mark it as an error.
          if (!eid1.isNull())
            _tagWrong(conflated, m.first);
        }

        QList<ElementId> eid2s =  _actualUuidToEid.values(m.second);
        for (const auto& eid2 : eid2s)
        {
          // sometimes elements are removed during conflation. If they were supposed to be matched,
          // then mark it as an error.
          if (!eid2.isNull())
            _tagWrong(conflated, m.second);
        }

        for (const auto& eid1 : eid1s)
        {
          for (const auto& eid2 : eid2s)
          {
            if (!eid1.isNull() && !eid2.isNull())
            {
              ElementPtr e1 = conflated->getElement(eid1);
              ElementPtr e2 = conflated->getElement(eid2);
              _addWrong(e1->getTags(), e2->getTags());
            }
          }
        }
      }

      _tagError(conflated, m.first, "1");
      _tagError(conflated, m.second, "2");

      //The info from these tags can be misleading if you are conflating the same data type twice
      //in the same conflation job (e.g. poi to poi AND poi to poly), due to the fact that in
      //those cases multiple actual/expected states can exist and this logic only records one
      //of them.
      const MatchType expectedMatchType(expectedIndex);
      const MatchType actualMatchType(actualIndex);
      _tagTestOutcome(conflated, m.first, expectedMatchType.toString(), actualMatchType.toString());
      _tagTestOutcome(conflated, m.second, expectedMatchType.toString(), actualMatchType.toString());
    }

    _confusion[actualIndex][expectedIndex]++;

    numPairsParsed++;
    if (numPairsParsed % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO("Processed " << numPairsParsed << " of " << allPairs.size() << " match pairs.");
    }
  }

  _setElementWrongCounts(conflated);

  _tp = _confusion[MatchType::Match][MatchType::Match];
  _fn = _confusion[MatchType::Miss][MatchType::Match] +
        _confusion[MatchType::Review][MatchType::Match];
  _fp = _confusion[MatchType::Match][MatchType::Miss] +
        _confusion[MatchType::Match][MatchType::Review];

  LOG_VAR(_wrongBreakdown);
  LOG_VAR(TextTable(_wrongBreakdown).toWikiString());

  return static_cast<double>(_tp) / static_cast<double>(_tp + _fn + _fp);
}

void MatchComparator::_createMatches(const set<QString>& uuids1, const set<QString>& uuids2,
                                     set<UuidPair>& matches, Tgs::DisjointSetMap<QString>& groups) const
{
  // create a match between all the combinations of ref1 uuid to ref2 uuid
  for (const auto& u1 : uuids1)
  {
    for (const auto& u2 : uuids2)
    {
      matches.emplace(u1, u2);
      groups.joinT(u1, u2);
    }
  }
  LOG_VART(matches.size());
}

void MatchComparator::_findActualMatches(const ConstOsmMapPtr& in, const ConstOsmMapPtr& conflated)
{
  UuidToEidVisitor uuidToEidVisitor;
  conflated->visitRo(uuidToEidVisitor);
  _actualUuidToEid = uuidToEidVisitor.getUuidToEid();

  // read out all the uuids in the conflated data
  set<QString> cUuids;
  UniqueTagValuesVisitor vc("uuid", cUuids);
  conflated->visitRo(vc);
  LOG_TRACE("cUuids size: " << cUuids.size());

  // go through all the reviews in the conflated map
  set<ReviewMarker::ReviewUid> ruuid = ReviewMarker::getReviewUids(conflated);
  for (const auto& r : ruuid)//set<ReviewMarker::ReviewUid>::iterator it = ruuid.begin(); it != ruuid.end(); ++it)
  {
    set<QString> u1;
    set<QString> u2;

    set<ElementId> eids = ReviewMarker::getReviewElements(conflated, r);
    for (const auto& p : eids)//set<ElementId>::iterator eid = eids.begin(); eid != eids.end(); ++eid)
    {
      ConstElementPtr element = conflated->getElement(p);
      if (!element.get())
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Missing element for " + p.toString());
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
        continue;
      }
      QString uuidStr = element->getTags()["uuid"];
      if (uuidStr.isEmpty())
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Missing uuid for " + p.toString());
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
        continue;
      }

      if (conflated->getElement(p)->getStatus() == Status::Unknown1)
        u1.insert(uuidStr);
      else if (conflated->getElement(p)->getStatus() == Status::Unknown2)
        u2.insert(uuidStr);
      else if (u1.empty() && !u2.empty())
        u1.insert(uuidStr);
      else
        u2.insert(uuidStr);
    }

    // create a match between all the combinations of ref1 uuid to ref2 uuid
    for (const auto& ref1 : u1)//set<QString>::const_iterator iit = u1.begin(); iit != u1.end(); ++iit)
    {
      for (const auto& ref2 : u2)//set<QString>::const_iterator jt = u2.begin(); jt != u2.end(); ++jt)
        _actual.emplace(ref1, ref2);//.insert(UuidPair(*iit, *jt));
    }
  }

  // read out all the uuids in in1 and in2
  set<QString> in1Uuids;
  UniqueTagValuesVisitor gtv1("uuid", in1Uuids);
  StatusCriterion sf1(Status::Unknown1);
  FilteredVisitor fvIn1(sf1, gtv1);
  in->visitRo(fvIn1);
  LOG_TRACE("in1Uuids size: " << in1Uuids.size());

  set<QString> in2Uuids;
  UniqueTagValuesVisitor gtv2("uuid", in2Uuids);
  StatusCriterion sf2(Status::Unknown2);
  FilteredVisitor fvIn2(sf2, gtv2);
  in->visitRo(fvIn2);
  LOG_TRACE("in2Uuids size: " << in2Uuids.size());

  for (const auto& value : cUuids)
  {
    QStringList cList = Tags::split(value);
    set<QString> u1;
    set<QString> u2;

    for (const auto& c_value : cList)
    {
      // split the uids into two groups.
      if (in1Uuids.find(c_value) != in1Uuids.end())
      {
        u1.insert(c_value);
        LOG_TRACE("Inserted " << c_value << " into u1.");
      }
      else if (in2Uuids.find(c_value) != in2Uuids.end())
      {
        u2.insert(c_value);
        LOG_TRACE("Inserted " << c_value << " into u2.");
      }
      else
      {
        LOG_TRACE("Missing UUID: " << c_value);
        throw HootException("Conflated uuid was not found in either input: " + c_value);
      }
    }
    // create a match between all the combinations of ref1 uuid to ref2 uuid
    _createMatches(u1, u2, _actual, _actualMatchGroups);
  }
  LOG_VARD(_actual.size());
}

void MatchComparator::_findExpectedMatches(const ConstOsmMapPtr& in)
{
  // extract all of the REF2 values in in2
  GetRefUuidVisitor ref1(MetadataTags::Ref1());
  in->visitRo(ref1);
  LOG_VARD(ref1.getRefToUuid().size());

  GetRefUuidVisitor ref2(MetadataTags::Ref2());
  in->visitRo(ref2);
  LOG_VARD(ref2.getRefToUuid().size());

  GetRefUuidVisitor review(MetadataTags::Review());
  in->visitRo(review);

  // go through all the ref1 uuids
  for (auto it = ref1.getRefToUuid().begin(); it != ref1.getRefToUuid().end(); ++it)
  {
    // match the ref1 to ref2 uuids
    auto jt = ref2.getRefToUuid().find(it->first);
    if (jt != ref2.getRefToUuid().end())
    {
      const set<QString>& ref1Uuids = it->second;
      const set<QString>& ref2Uuids = jt->second;
      // create a match between all the combinations of ref1 uuid to ref2 uuid
      _createMatches(ref1Uuids, ref2Uuids, _expected, _expectedMatchGroups);
    }

    // match the ref1 to review uuids
    auto kt = review.getRefToUuid().find(it->first);
    if (kt != review.getRefToUuid().end())
    {
      const set<QString>& ref1Uuids = it->second;
      const set<QString>& reviewUuids = kt->second;
      // create a match between all the combinations of ref1 uuid to review uuid
      _createMatches(ref1Uuids, reviewUuids, _expected, _expectedReviewGroups);
    }
  }
  LOG_VARD(_expected.size());
}


double MatchComparator::getPercentCorrect() const
{
  return double(_confusion[MatchType::Match][MatchType::Match] +
                _confusion[MatchType::Miss][MatchType::Miss] +
                _confusion[MatchType::Review][MatchType::Review]) / static_cast<double>(getTotalCount());
}

double MatchComparator::getPercentWrong() const
{
  return double(_confusion[MatchType::Match][MatchType::Miss] +
                _confusion[MatchType::Match][MatchType::Review] +
                _confusion[MatchType::Miss][MatchType::Match] +
                _confusion[MatchType::Miss][MatchType::Review]) / static_cast<double>(getTotalCount());
}

double MatchComparator::getPercentUnnecessaryReview() const
{
  return double(_confusion[MatchType::Review][MatchType::Match] +
                _confusion[MatchType::Review][MatchType::Miss]) / static_cast<double>(getTotalCount());
}

int MatchComparator::getTotalCount() const
{
  int result = 0;
  for (size_t i = 0; i < CONFUSION_SIZE; ++i)
  {
    for (size_t j = 0; j < CONFUSION_SIZE; ++j)
      result += _confusion[i][j];
  }
  return result;
}

bool MatchComparator::_isNeedsReview(const QString& uuid1, const QString& uuid2,
                                     const ConstOsmMapPtr& conflated) const
{
  QList<ElementId> eid1s = _actualUuidToEid.values(uuid1);
  QList<ElementId> eid2s = _actualUuidToEid.values(uuid2);

  bool result = false;
  for (const auto& eid1 : eid1s)
  {
    for (const auto& eid2 : eid2s)
    {
      if (eid1.isNull() || eid2.isNull())
      {
        // So far this message is ok, change from LOG_INFO to LOG_DEBUG.
        // More information please see issue 167. https://github.com/ngageoint/hootenanny/issues/167
        LOG_DEBUG("No actual element exists.");
        return false;
      }
      if (ReviewMarker::isNeedsReview(conflated, conflated->getElement(eid1), conflated->getElement(eid2)))
        result = true;
    }
  }
  return result;
}

void MatchComparator::_tagTestOutcome(const OsmMapPtr& map, const QString& uuid,
                                      const QString& expected, const QString& actual)
{
  SetTagValueVisitor stv1(MetadataTags::HootExpected(), expected);
  SetTagValueVisitor stv2(MetadataTags::HootActual(), actual);
  _tagVisitor(map, uuid, &stv1, &stv2);
}

void MatchComparator::_tagError(const OsmMapPtr& map, const QString& uuid, const QString& value)
{
  // If the uuid contains the first uuid, set mismatch.
  SetTagValueVisitor stv(MetadataTags::HootMismatch(), value);
  _tagVisitor(map, uuid, &stv);
}

void MatchComparator::_tagWrong(const OsmMapPtr& map, const QString &uuid)
{
  // If the uuid contains the first uuid, set wrong.
  SetTagValueVisitor stv(MetadataTags::HootWrong(), "1");
  _tagVisitor(map, uuid, &stv);
}

void MatchComparator::_tagVisitor(const OsmMapPtr& map, const QString& uuid, SetTagValueVisitor* v1, SetTagValueVisitor* v2)
{
  for (auto it = _actualUuidToEid.begin(); it != _actualUuidToEid.end(); ++it)
  {
    if (it.key().contains(uuid))
    {
      ElementPtr element = map->getElement(it.value());
      if (v1 != nullptr)
        v1->visit(element);
      if (v2 != nullptr)
        v2->visit(element);
    }
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
  _elementWrongCounts[elementType] =
    static_cast<int>(FilteredVisitor::getStat(
      std::make_shared<ChainCriterion>(
        std::make_shared<ElementTypeCriterion>(elementType),
        std::make_shared<TagKeyCriterion>(MetadataTags::HootWrong())),
      std::make_shared<ElementCountVisitor>(),
      map));
}

QString MatchComparator::toString() const
{
  QString result;

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
        result += QString(" |   -  ");
      else
        result += QString(" |%1").arg(_confusion[i][j], 6, 10);
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
  const int totalCorrect = _confusion[MatchType::Match][MatchType::Match] +
                           _confusion[MatchType::Review][MatchType::Review];
  result += QString("correct match count: %1\n").arg(totalCorrect);
  result += QString("wrong match count: %1\n")
              .arg(_confusion[MatchType::Miss][MatchType::Match] +
                   _confusion[MatchType::Miss][MatchType::Review] +
                   _confusion[MatchType::Match][MatchType::Miss] +
                   _confusion[MatchType::Match][MatchType::Review]);
  const int totalUnnecessaryReviews = _confusion[MatchType::Review][MatchType::Miss] +
                                      _confusion[MatchType::Review][MatchType::Match];
  result += QString("unnecessary review count: %1\n").arg(totalUnnecessaryReviews);
  result += QString("ratio of unnecessary reviews to correct matches: %1\n")
    .arg(static_cast<double>(totalUnnecessaryReviews) / static_cast<double>(totalCorrect));

  return result;
}

}
