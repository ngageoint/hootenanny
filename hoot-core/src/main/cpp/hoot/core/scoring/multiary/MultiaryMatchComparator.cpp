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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiaryMatchComparator.h"

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/conflate/MarkForReviewMerger.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/ElementTypeCriterion.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/TagContainsFilter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/TextTable.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/GetTagValuesVisitor.h>
#include <hoot/core/visitors/SetTagVisitor.h>
#include <hoot/core/visitors/ElementIdSetVisitor.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSet>

using namespace std;

namespace hoot
{

unsigned int MultiaryMatchComparator::logWarnCount = 0;

MultiaryMatchComparator::MultiaryMatchComparator()
{
  _tagErrors = true;

  _confusion.resize(3);

  for (size_t i = 0; i < 3; i++)
  {
    _confusion[i].resize(3, 0);
  }
}

void MultiaryMatchComparator::_clearCache()
{
  // clears everything but the confusion matrix
  _actual.clear();
  _expected.clear();
  _actualIdToEid.clear();
  _actualMatchGroups.clear();
  _expectedMatchGroups.clear();
  _elementWrongCounts.clear();
}

double MultiaryMatchComparator::evaluateMatches(const ConstOsmMapPtr& in, const OsmMapPtr& conflated)
{
  _clearCache();
  // determine the expected & actual situation
  _findExpectedMatches(in);
  _findExpectedReviews(in);
  _findActualMatches(conflated);
  _findActualReviews(conflated);

//  LOG_VAR(_actualMatchGroups);
//  LOG_VAR(_expectedMatchGroups);
//  LOG_VAR(_actualReviews);
//  LOG_VAR(_expectedReviews);

  // go through all the IDs in the in data set.
  foreach (QString id, _expectedIdToEid.keys())
  {
//    LOG_VAR(id);
    assert(_actualMatchGroups.contains(id));
    assert(_expectedMatchGroups.contains(id));

    QSet<QString> actualMatchSet = *_actualMatchGroups[id];
    QSet<QString> expectedMatchSet = *_expectedMatchGroups[id];
    QSet<QString> actualReviewSet = *_actualReviews.getCluster(id);
    QSet<QString> expectedReviewSet = *_expectedReviews.getCluster(id);

    actualMatchSet -= id;
    expectedMatchSet -= id;
    actualReviewSet -= id;
    expectedReviewSet -= id;

//    LOG_VAR(actualMatchSet);
//    LOG_VAR(expectedMatchSet);
//    LOG_VAR(actualReviewSet);
//    LOG_VAR(expectedReviewSet);

    // calculate the number of correct matches for this element.
    int correctMatches = (expectedMatchSet & actualMatchSet).size();

    // calculate the number of incorrect matches for this element.
    int incorrectMatches = (actualMatchSet - expectedMatchSet).size();

    // calculate the number of missed matches for this element.
    int missedMatches = (expectedMatchSet - actualMatchSet).size();

    if (incorrectMatches || missedMatches)
    {
      _tagWrong(conflated, id);
    }

    // Weighting counts based on the cluster size is a bit unorthodoxed, but if we use the more
    // typical pairwise comparison the large clusters will have a disproportionately large impact
    // on the result. (e.g. a cluster of size 4 will have 4x the impact of a cluster with size 2)

    _confusion[MatchType::Match][MatchType::Match] += correctMatches;
    _confusion[MatchType::Miss][MatchType::Match] += missedMatches;
    // we expected a match, but got a review.
    _confusion[MatchType::Review][MatchType::Match] += (actualReviewSet & expectedMatchSet).size();
    _confusion[MatchType::Match][MatchType::Miss] += incorrectMatches;
    // we don't care about miss/miss. This isn't a meaningful value.
    // _confusion[MatchType::Miss][MatchType::Miss]
    _confusion[MatchType::Review][MatchType::Miss] += (actualReviewSet -
                                                       expectedReviewSet -
                                                       expectedMatchSet).size();
    _confusion[MatchType::Review][MatchType::Review] += (expectedReviewSet &
                                                         actualReviewSet).size();
    _confusion[MatchType::Match][MatchType::Review] += (actualMatchSet &
                                                        expectedReviewSet).size();
    _confusion[MatchType::Miss][MatchType::Review] += (expectedReviewSet -
                                                       actualReviewSet -
                                                       actualMatchSet).size();
  }

  // we're counting both sides of each pair, so divide by two.
  _confusion[MatchType::Match][MatchType::Match] /= 2;
  _confusion[MatchType::Miss][MatchType::Match] /= 2;
  _confusion[MatchType::Review][MatchType::Match] /= 2;
  _confusion[MatchType::Match][MatchType::Miss] /= 2;
  _confusion[MatchType::Miss][MatchType::Miss] /= 2;
  _confusion[MatchType::Review][MatchType::Miss] /= 2;
  _confusion[MatchType::Match][MatchType::Review] /= 2;
  _confusion[MatchType::Miss][MatchType::Review] /= 2;
  _confusion[MatchType::Review][MatchType::Review] /= 2;

  _setElementWrongCounts(conflated);

  // what did we get right (excluding miss/miss)
  _tp = _confusion[MatchType::Match][MatchType::Match] +
      _confusion[MatchType::Review][MatchType::Review];
  // what did we miss?
  _fn = _confusion[MatchType::Miss][MatchType::Match] +
      _confusion[MatchType::Review][MatchType::Match] +
      _confusion[MatchType::Miss][MatchType::Review];
  // where were we over-zealous?
  _fp = _confusion[MatchType::Match][MatchType::Miss] +
      _confusion[MatchType::Match][MatchType::Review] +
      _confusion[MatchType::Review][MatchType::Miss];

  return double(_tp) / double(_tp + _fn + _fp);
}

void MultiaryMatchComparator::_findActualMatches(const ConstOsmMapPtr& conflated)
{
  class TmpVisitor : public ElementVisitor
  {
  public:
    TmpVisitor(const IdToEid& expectedIdToEid, IdToEid& actualIdToEid,
      QHash<QString, IdClusterPtr>& matchGroups) :
      _actualIdToEid(actualIdToEid),
      _expectedIdToEid(expectedIdToEid),
      _matchGroups(matchGroups)
    {
    }

    virtual void visit(const ConstElementPtr& e)
    {
      QList<QString> ids = _getAllIds(e);

      foreach (QString oneId, ids)
      {
        if (_expectedIdToEid.contains(oneId) == false)
        {
          LOG_WARN("Got an ID in the conflated output that was not in the expected input: " <<
            oneId);
        }
      }

      IdClusterPtr cluster(new IdCluster());

      foreach (QString id, ids)
      {
        if (_matchGroups.contains(id))
        {
          LOG_WARN("One ID has appeared in multiple elements: " << id);
        }
        _actualIdToEid[id] = e->getElementId();
        _matchGroups[id] = cluster;
        cluster->insert(id);
      }
    }

  private:
    IdToEid& _actualIdToEid;
    const IdToEid& _expectedIdToEid;
    QHash<QString, IdClusterPtr>& _matchGroups;
  };

  TmpVisitor v(_expectedIdToEid, _actualIdToEid, _actualMatchGroups);
  conflated->visitRo(v);
}

void MultiaryMatchComparator::_findActualReviews(const ConstOsmMapPtr& map)
{
  set<ReviewMarker::ReviewUid> reviews = ReviewMarker::getReviewUids(map);

  foreach (ReviewMarker::ReviewUid ruid, reviews)
  {
    set<ElementId> rEids = ReviewMarker::getReviewElements(map, ruid);

    QSet<QString> allIds;

    foreach (ElementId eid, rEids)
    {
      allIds += QSet<QString>::fromList(_getAllIds(map->getElement(eid)));
    }

    bool foundExpectedPair = false;
    // the IDs we will use to create the actual review.
    QList<QString> idsToAdd;

    foreach (QString id, allIds)
    {
      IdClusterPtr c = _expectedReviews.getCluster(id);
      QSet<QString> copy = *c;
      copy -= id;
      QSet<QString> intersection = copy & allIds;

      if (intersection.size() > 0)
      {
        idsToAdd.append(id);
        idsToAdd.append(*intersection.begin());
        foundExpectedPair = true;
        break;
      }
    }

    // for now this only when there are two elements in the review. You'll need to do a bit of
    // noodling if this fails when we expand to buildings or similar that cause 3 or more elements
    // in a review.
    assert (rEids.size() == 2 || !foundExpectedPair);

    // if we didn't find what we were looking for...
    if (!foundExpectedPair)
    {
      // arbitrarily pick the first ID from each cluster. There is no need to report a bad review
      // for every ID in the cluster. Seems kinda silly.
      foreach (ElementId eid, rEids)
      {
        idsToAdd += *_getAllIds(map->getElement(eid)).begin();
      }
    }

    // add an actual review between each of the chosen IDs.
    for (int i = 0; i < idsToAdd.size(); ++i)
    {
      for (int j = i + 1; j < idsToAdd.size(); ++j)
      {
        _actualReviews.addReview(idsToAdd[i], idsToAdd[j]);
      }
    }
  }
}


void MultiaryMatchComparator::_findExpectedMatches(const ConstOsmMapPtr& in)
{
  class TmpVisitor : public ElementVisitor
  {
  public:
    TmpVisitor(IdToEid& idToEid, QHash<QString, IdClusterPtr>& matchGroups) :
      _idToEid(idToEid),
      _matchGroups(matchGroups)
    {
    }

    virtual void visit(const ConstElementPtr& e)
    {
      const Tags& t = e->getTags();
      QString id = MetadataTags::TrainingId();

      if (t.contains(id))
      {
        _idToEid[t[id]] = e->getElementId();
        if (_matchGroups.contains(t[id]) == false)
        {
          _matchGroups.insert(t[id], IdClusterPtr(new IdCluster()));
        }
        _matchGroups[t[id]]->insert(t[id]);
      }

      QString match = MetadataTags::TrainingMatch();

      if (t.contains(match))
      {
        if (t[match] == "none")
        {
          // pass. No matches.
        }
        else if (t[match] == "todo")
        {
          LOG_WARN("Got an unexpected todo match. " << e->getElementId());
        }
        else if (t.contains(id))
        {
          if (_matchGroups.contains(t[match]) == false)
          {
            _matchGroups.insert(t[match], IdClusterPtr(new IdCluster()));
          }
          // copy all the matches in t[id] into t[match]
          *_matchGroups[t[match]] += *_matchGroups[t[id]];

          // map this t[id] to the same group as t[match]
          _matchGroups[t[id]] = _matchGroups[t[match]];
        }
        else
        {
          LOG_WARN("There is a match on an element without an ID tag. " << e->getElementId());
        }
      }
    }

  private:
    IdToEid& _idToEid;
    QHash<QString, IdClusterPtr>& _matchGroups;
  };

  TmpVisitor v(_expectedIdToEid, _expectedMatchGroups);
  in->visitRo(v);

  // validate the match groups
  foreach (QString k, _expectedMatchGroups.keys())
  {
    if (_expectedMatchGroups[k]->contains(k) == false)
    {
      LOG_WARN(QString("Group with ID %1 doesn't contain the element with ID %1.").arg(k));
    }
  }
}

void MultiaryMatchComparator::_findExpectedReviews(const ConstOsmMapPtr& in)
{
  class TmpVisitor : public ElementVisitor
  {
  public:
    TmpVisitor(ReviewClusterIndex& index) :
      _index(index)
    {
    }

    virtual void visit(const ConstElementPtr& e)
    {
      if (e->getStatus().isInput())
      {
        const Tags& t = e->getTags();

        if (t.contains(MetadataTags::TrainingReview()))
        {
          if (t[MetadataTags::TrainingReview()] == "none")
          {
            // pass. No matches.
          }
          else if (t[MetadataTags::TrainingReview()] == "todo")
          {
            LOG_WARN("Got an unexpected todo match. " << e->getElementId());
          }
          else if (t.contains(MetadataTags::TrainingId()))
          {
            QStringList l = t.getList(MetadataTags::TrainingReview());

            foreach (QString s, l)
            {
              _index.addReview(t[MetadataTags::TrainingId()], s);
            }
          }
          else
          {
            LOG_WARN("There is a match on an element without an ID tag. " << e->getElementId());
          }
        }
      }
    }

  private:
    ReviewClusterIndex& _index;
  };

  TmpVisitor v(_expectedReviews);
  in->visitRo(v);
}

QList<QString> MultiaryMatchComparator::_getAllIds(ConstElementPtr e)
{
  const Tags& tags = e->getTags();
  QList<QString> result;

  // is this an ID key?
  if (e->getTags().contains(MetadataTags::TrainingId()))
  {
    QStringList idList = tags.getList(MetadataTags::TrainingId());

    foreach (QString id, idList)
    {
      result += id;
    }
  }

  return result;
}

double MultiaryMatchComparator::getFScore(double beta) const
{
  double n = (1 + beta * beta) * getTp();
  double d = (1 + beta * beta) * getTp() + beta * beta * getFn() + getFp();

  return n / d;
}

double MultiaryMatchComparator::getPercentCorrect() const
{
  return double(_confusion[MatchType::Match][MatchType::Match] +
    _confusion[MatchType::Miss][MatchType::Miss] +
    _confusion[MatchType::Review][MatchType::Review]) / double(getTotalCount());
}

double MultiaryMatchComparator::getPercentWrong() const
{
  return double(_confusion[MatchType::Match][MatchType::Miss] +
    _confusion[MatchType::Match][MatchType::Review] +
    _confusion[MatchType::Miss][MatchType::Match] +
    _confusion[MatchType::Miss][MatchType::Review]) / double(getTotalCount());
}

double MultiaryMatchComparator::getPercentUnnecessaryReview() const
{
  return double(_confusion[MatchType::Review][MatchType::Match] +
    _confusion[MatchType::Review][MatchType::Miss]) / double(getTotalCount());
}

int MultiaryMatchComparator::getTotalCount() const
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

void MultiaryMatchComparator::_tagWrong(const OsmMapPtr &map, const QString &id)
{
  if (_tagErrors)
  {
    ElementPtr e = map->getElement(_actualIdToEid[id]);

    if (e.get())
    {
      e->getTags()[MetadataTags::HootWrong()] = "1";
    }
  }
}

QSet<QString> MultiaryMatchComparator::_toQSet(set<QString> s)
{
  QSet<QString> result;
  foreach (QString str, s)
  {
    result.insert(str);
  }
  return result;
}

void MultiaryMatchComparator::_setElementWrongCounts(const ConstOsmMapPtr& map)
{
  _setElementWrongCount(map, ElementType::Node);
  _setElementWrongCount(map, ElementType::Way);
  _setElementWrongCount(map, ElementType::Relation);
}

void MultiaryMatchComparator::_setElementWrongCount(const ConstOsmMapPtr& map,
                                            const ElementType::Type& elementType)
{
  _elementWrongCounts[elementType] =
    (int)FilteredVisitor::getStat(
      new ChainCriterion(
        new ElementTypeCriterion(elementType),
        new TagKeyCriterion(MetadataTags::HootWrong())),
      new ElementCountVisitor(),
      map);
}

QString MultiaryMatchComparator::toString() const
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
