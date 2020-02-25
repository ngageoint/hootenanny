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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiaryMatchComparator.h"

// hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/rnd/scoring/multiary/MultiaryMatchTrainingValidator.h>

// Qt
#include <QSet>

using namespace std;

namespace hoot
{

int MultiaryMatchComparator::logWarnCount = 0;

const QString matchMatchStr = "missMatch";
const QString matchMissStr = "matchMiss";
const QString matchReviewStr = "matchReview";
const QString missMatchStr = "missMatch";
const QString missReviewStr = "missReview";
const QString reviewMatchStr = "reviewMatch";
const QString reviewMissStr = "reviewMiss";
const QString reviewReviewStr = "reviewReview";

MultiaryMatchComparator::MultiaryMatchComparator()
{
  _tagErrors = true;
}

void MultiaryMatchComparator::_addToConfusionTable(const ConfusionTable& x, ConfusionTable& addTo)
  const
{
  bool foundOne = false;
  foreach (int i, x.keys())
  {
    foreach (int j, x[i].keys())
    {
      addTo[i][j] += x[i][j];
      if (x[i][j] > 0)
      {
        foundOne = true;
      }
    }
  }

  if (foundOne)
  {
    addTo[-1][-1] += 1;
  }
}

void MultiaryMatchComparator::_calculateNodeBasedStats(const ConstOsmMapPtr& conflated)
{
  // used to represent the stats for all layers.
  QString all = "All Layers";
  QString noCategory = "No Category";
  // A layer name to confusion table mapping
  QMap<QString, ConfusionTable> confusionTables;

  ScriptToOgrSchemaTranslatorPtr translator;

  if (_translationScript != "")
  {
    translator.reset(
      dynamic_cast<ScriptToOgrSchemaTranslator*>(
        ScriptSchemaTranslatorFactory::getInstance().createTranslator(_translationScript)));
    translator->getOgrOutputSchema();
  }


  UniqueElementIdVisitor eids;
  conflated->visitRo(eids);

  ElementConverter ec(conflated);

  foreach (ElementId eid, eids.getElementSet())
  {
    ConstElementPtr e = conflated->getElement(eid);
    Tags tags = e->getTags();

    if (ReviewMarker::isReviewUid(conflated, eid) == false)
    {
      QString hootWrong = tags.get(MetadataTags::HootWrong());

      ConfusionTable conf;

      // calculate the number of correct matches for this element.
      conf[MatchType::Match][MatchType::Match] += hootWrong.isEmpty() &&
          e->getStatus() == Status::Conflated ? 1 : 0;
      conf[MatchType::Match][MatchType::Miss] = hootWrong.contains(matchMissStr) ? 1 : 0;
      conf[MatchType::Match][MatchType::Review] = hootWrong.contains(matchReviewStr) ? 1 : 0;
      // take the expected set of matches then remove the actual matches and reviews, what is left
      // are the misses.
      conf[MatchType::Miss][MatchType::Match] = hootWrong.contains(missMatchStr) ? 1 : 0;
      conf[MatchType::Miss][MatchType::Miss] = hootWrong.isEmpty() &&
          e->getStatus() != Status::Conflated ? 1 : 0;
      conf[MatchType::Miss][MatchType::Review] = hootWrong.contains(missReviewStr) ? 1 : 0;
      conf[MatchType::Review][MatchType::Match] = hootWrong.contains(reviewMatchStr) ? 1 : 0;
      conf[MatchType::Review][MatchType::Miss] = hootWrong.contains(reviewMissStr) ? 1 : 0;
      conf[MatchType::Review][MatchType::Review] =
          tags.contains(MetadataTags::HootCorrectReview()) ? 1 : 0;

      _addToConfusionTable(conf, confusionTables[all]);

      if (translator)
      {
        std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature> translated =
            translator->translateToOgr(tags, e->getElementType(),
              ec.getGeometryType(e, false));

        bool foundCategory = false;
        foreach (const ScriptToOgrSchemaTranslator::TranslatedFeature& tf, translated)
        {
          _addToConfusionTable(conf, confusionTables[tf.tableName]);
          foundCategory = true;
        }

        if (foundCategory == false)
        {
          _addToConfusionTable(conf, confusionTables[noCategory]);
          LOG_VART(tags);
          foreach (const ScriptToOgrSchemaTranslator::TranslatedFeature& tf, translated)
          {
            LOG_VART(tf);
          }
        }
      }
    }
  }

  _nodeBasedStatsResult = "== Node-based Statistics\n\n";

  foreach (QString tableName, confusionTables.keys())
  {
    _nodeBasedStatsResult += "\n=== " + tableName + "\n\n";
    _nodeBasedStatsResult += _printConfusionTable(confusionTables[tableName]);
  }


  foreach (QString tableName, confusionTables.keys())
  {
    const ConfusionTable& ct = confusionTables[tableName];
    int noWorkCount = ct[MatchType::Review][MatchType::Review] +
        ct[MatchType::Match][MatchType::Match] +
        ct[MatchType::Miss][MatchType::Miss];
    int missMatchCount = ct[MatchType::Miss][MatchType::Match];
    int missReviewCount = ct[MatchType::Miss][MatchType::Review];
    int matchMissCount = ct[MatchType::Match][MatchType::Miss];
    int matchReviewCount = ct[MatchType::Match][MatchType::Review];
    int reviews = ct[MatchType::Review][MatchType::Miss] +
        ct[MatchType::Review][MatchType::Match] +
        ct[MatchType::Review][MatchType::Review];
    int totalCount = ct[-1][-1];
    _nodeBasedStatsResult += "\n" + tableName + "\t";
    _nodeBasedStatsResult += QString("%1\t%2\t%3\t%4\t%5\t%6\t%7").arg(noWorkCount).arg(missMatchCount).
        arg(missReviewCount).arg(matchMissCount).arg(matchReviewCount).arg(reviews).arg(totalCount);
  }
  _nodeBasedStatsResult += "\n";

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

double MultiaryMatchComparator::evaluateMatches(const ConstOsmMapPtr& in,
  const OsmMapPtr& conflated)
{
  OsmMapPtr copyIn(new OsmMap(in));
  MultiaryMatchTrainingValidator().apply(copyIn);

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

//    LOG_VAR(id);
//    LOG_VAR(actualMatchSet);
//    LOG_VAR(expectedMatchSet);
//    LOG_VAR(actualReviewSet);
//    LOG_VAR(expectedReviewSet);

    // confusion matrix contributions. Variables are named as: actualExpected

    // calculate the number of correct matches for this element.
    int matchMatch = (expectedMatchSet & actualMatchSet).size();
    int matchMiss = (actualMatchSet - (expectedReviewSet + expectedMatchSet)).size();
    int matchReview = (actualMatchSet & expectedReviewSet).size();
    // take the expected set of matches then remove the actual matches and reviews, what is left
    // are the misses.
    int missMatch = (expectedMatchSet - (actualMatchSet + actualReviewSet)).size();
    int missReview = (expectedReviewSet - actualReviewSet - actualMatchSet).size();
    int reviewMatch = (actualReviewSet & expectedMatchSet).size();
    int reviewMiss = (actualReviewSet - expectedReviewSet - expectedMatchSet).size();
    int reviewReview = (actualReviewSet & expectedReviewSet).size();

    if (matchMiss)
    {
      _tagWrong(conflated, id, matchMissStr);
    }
    if (matchReview)
    {
      _tagWrong(conflated, id, matchReviewStr);
    }
    if (missMatch)
    {
      _tagWrong(conflated, id, missMatchStr);
    }
    if (missReview)
    {
      _tagWrong(conflated, id, missReviewStr);
    }
    if (reviewMatch)
    {
      _tagWrong(conflated, id, reviewMatchStr);
    }
    if (reviewMiss)
    {
      _tagWrong(conflated, id, reviewMissStr);
    }
    if (reviewReview)
    {
      _tagReview(conflated, id, reviewReviewStr);
    }

    _confusion[MatchType::Match][MatchType::Match] += matchMatch;
    _confusion[MatchType::Miss][MatchType::Match] += missMatch;
    // we expected a match, but got a review.
    _confusion[MatchType::Review][MatchType::Match] += reviewMatch;
    _confusion[MatchType::Match][MatchType::Miss] += matchMiss;
    // we don't care about miss/miss. This isn't a meaningful value.
    // _confusion[MatchType::Miss][MatchType::Miss]
    _confusion[MatchType::Review][MatchType::Miss] += reviewMiss;
    _confusion[MatchType::Review][MatchType::Review] += (expectedReviewSet &
                                                         actualReviewSet).size();
    _confusion[MatchType::Match][MatchType::Review] += matchReview;
    _confusion[MatchType::Miss][MatchType::Review] += missReview;
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

  _calculateNodeBasedStats(conflated);

  return double(_tp) / double(_tp + _fn + _fp);
}

void MultiaryMatchComparator::_findActualMatches(const ConstOsmMapPtr& conflated)
{
  class TmpVisitor : public ConstElementVisitor
  {
  public:

    TmpVisitor(const IdToEid& expectedIdToEid, IdToEid& actualIdToEid,
      QHash<QString, IdClusterPtr>& matchGroups) :
      _actualIdToEid(actualIdToEid),
      _expectedIdToEid(expectedIdToEid),
      _matchGroups(matchGroups)
    {
    }

    virtual QString getDescription() const { return ""; }
    virtual std::string getClassName() const { return ""; }

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
    QList<ElementId> rEidList;

    // convert the set to a list
    foreach (ElementId eid, rEids)
    {
      rEidList.append(eid);
    }

    // go through all the possible pairwise combinations of rEidList (likely only two entries in
    // rEidList, but it doesn't hurt to be thorough)
    for (int i = 0; i < rEidList.size(); ++i)
    {
      QList<QString> allIds1 = _getAllIds(map->getElement(rEidList[i]));
      for (int j = i + 1; j < rEidList.size(); ++j)
      {
        QList<QString> allIds2 = _getAllIds(map->getElement(rEidList[j]));

        // add an actual review between each of the IDs in the two elements.
        foreach (QString id1, allIds1)
        {
          foreach (QString id2, allIds2)
          {
            _actualReviews.addReview(id1, id2);
          }
        }
      }
    }
  }
}


void MultiaryMatchComparator::_findExpectedMatches(const ConstOsmMapPtr& in)
{
  class TmpVisitor : public ConstElementVisitor
  {
  public:

    TmpVisitor(IdToEid& idToEid, QHash<QString, IdClusterPtr>& matchGroups) :
      _idToEid(idToEid),
      _matchGroups(matchGroups)
    {
    }

    virtual QString getDescription() const { return ""; }
    virtual std::string getClassName() const { return ""; }

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
  class TmpVisitor : public ConstElementVisitor
  {
  public:

    TmpVisitor(ReviewClusterIndex& index) :
      _index(index)
    {
    }

    virtual QString getDescription() const { return ""; }
    virtual std::string getClassName() const { return ""; }

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
  for (int i = 0; i <= MatchType::Review; i++)
  {
    for (int j = 0; j <= MatchType::Review; j++)
    {
      result += _confusion[i][j];
    }
  }

  return result;
}

QString MultiaryMatchComparator::_printConfusionTable(const ConfusionTable& x, bool emptyMissMiss)
  const
{
  QString result;
  QString left[3];
  // weird markup makes a pretty table in redmine.
  result += "|                 |        |\\3=.       expected     |\n";
  result += "|                 |        | miss  | match | review |\n";
  left[0] = "|/3. test outcome | miss  ";
  left[1] = "                  | match ";
  left[2] = "                  | review";
  for (int i = 0; i < 3; i++)
  {
    result += left[i];
    for (int j = 0; j < 3; j++)
    {
      if (i == 0 && j == 0 && emptyMissMiss)
      {
        result += QString(" |   -  ");
      }
      else
      {
        result += QString(" |%1").arg(x[i][j], 6, 10);
      }
    }
    result += "  |\n";
  }

  return result;
}


void MultiaryMatchComparator::_tagReview(const OsmMapPtr &map, const QString &id, QString value)
{
  if (_tagErrors)
  {
    ElementPtr e = map->getElement(_actualIdToEid[id]);

    if (e.get())
    {
      e->getTags().appendValue(MetadataTags::HootCorrectReview(), value);
    }
  }
}

void MultiaryMatchComparator::_tagWrong(const OsmMapPtr &map, const QString &id, QString value)
{
  if (_tagErrors)
  {
    ElementPtr e = map->getElement(_actualIdToEid[id]);

    if (e.get())
    {
      e->getTags().appendValue(MetadataTags::HootWrong(), value);
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
      ElementCriterionPtr(new ChainCriterion(
      ElementCriterionPtr(new ElementTypeCriterion(elementType)),
      ElementCriterionPtr(new TagKeyCriterion(MetadataTags::HootWrong())))),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
}

QString MultiaryMatchComparator::toString() const
{
  QString result = _nodeBasedStatsResult;

  result += "\n=== Edge-based Scoring\n\n";
  result += _printConfusionTable(_confusion, true);

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
