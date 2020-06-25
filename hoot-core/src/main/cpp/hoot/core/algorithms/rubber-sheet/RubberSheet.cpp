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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RubberSheet.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/criterion/RoundaboutCriterion.h>

// Tgs
#include <tgs/Statistics/Normal.h>
#include <tgs/Interpolation/DelaunayInterpolator.h>
#include <tgs/Interpolation/IdwInterpolator.h>
#include <tgs/Interpolation/KernelEstimationInterpolator.h>
#include <tgs/RandomForest/DataFrame.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

int RubberSheet::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, RubberSheet)

// register the interpolators that exist in TGS
HOOT_FACTORY_REGISTER(Interpolator, DelaunayInterpolator)
HOOT_FACTORY_REGISTER(Interpolator, KernelEstimationInterpolator)
HOOT_FACTORY_REGISTER(Interpolator, IdwInterpolator)

bool compareMatches(const RubberSheet::Match& m1, const RubberSheet::Match& m2)
{
  return m1.p > m2.p;
}

RubberSheet::RubberSheet()
{
  _emptyMatch.score = 0.0;
  _emptyMatch.p = 0.0;

  setConfiguration(conf());
}

void RubberSheet::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  setReference(config.getRubberSheetRef());
  setDebug(config.getRubberSheetDebug());
  setMinimumTies(config.getRubberSheetMinimumTies());
  setFailWhenMinimumTiePointsNotFound(config.getRubberSheetFailWhenMinimumTiePointsNotFound());
  setLogWarningWhenRequirementsNotFound(config.getRubberSheetLogMissingRequirementsAsWarning());
  setMaxAllowedWays(config.getRubberSheetMaxAllowedWays());

  LOG_VARD(_ref);
  LOG_VARD(_minimumTies);
  LOG_VARD(_failWhenMinTiePointsNotFound);
  LOG_VARD(_logWarningWhenRequirementsNotFound);
}

void RubberSheet::setCriteria(const QStringList& criteria, OsmMapPtr map)
{
  LOG_VARD(criteria.size());

  _criteria.reset();
  if (!criteria.isEmpty())
  {
    _criteria.reset(new OrCriterion());
    for (int i = 0; i < criteria.size(); i++)
    {
      const QString critName = criteria.at(i).trimmed();
      if (!critName.isEmpty())
      {
        LOG_VART(critName);
        ElementCriterionPtr crit =
          std::shared_ptr<ElementCriterion>(
            Factory::getInstance().constructObject<ElementCriterion>(critName.trimmed()));
        LOG_VART(crit.get());

        std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
          std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
        LOG_VART(conflatableCrit.get());
        std::shared_ptr<WayNodeCriterion> wayNodeCrit =
          std::dynamic_pointer_cast<WayNodeCriterion>(crit);
        LOG_VART(wayNodeCrit.get());
        if (!conflatableCrit && !wayNodeCrit)
        {
          throw IllegalArgumentException(
            "RubberSheet ElementCrition must be a ConflatableElementCriterion or WayNodeCriterion.");
        }
        if (conflatableCrit)
        {
          const GeometryTypeCriterion::GeometryType geometryType = conflatableCrit->getGeometryType();
          if (geometryType != GeometryTypeCriterion::GeometryType::Line &&
              geometryType != GeometryTypeCriterion::GeometryType::Polygon)
          {
            throw IllegalArgumentException(
              "RubberSheet element criterion must have a linear geometry type.");
          }
        }

        std::shared_ptr<OsmMapConsumer> mapConsumer =
          std::dynamic_pointer_cast<OsmMapConsumer>(crit);
        LOG_VART(mapConsumer.get());
        if (mapConsumer)
        {
          if (!map)
          {
            throw IllegalArgumentException("No map available to pass to map consumer.");
          }
          mapConsumer->setOsmMap(map.get());
        }

        _criteria->addCriterion(crit);
      }
    }
  }
}

void RubberSheet::_addIntersection(long nid, const set<long>& /*wids*/)
{
  NodePtr from = _map->getNode(nid);
  LOG_VART(from->getElementId());
  // the status type we're searching for
  Status s;
  if (from->getStatus() == Status::Unknown1)
  {
    s = Status::Unknown2;
  }
  else if (from->getStatus() == Status::Unknown2)
  {
    s = Status::Unknown1;
  }
  else
  {
    throw HootException("Expected either Unknown1 or Unknown2.");
  }
  LOG_VART(s);

  std::shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  double sum = 0.0;
  list<Match>& matches = _matches[nid];
  LOG_VART(matches.size());
  vector<long> neighbors = _map->getIndex().findNodes(from->toCoordinate(), _searchRadius);
  for (size_t i = 0; i < neighbors.size(); ++i)
  {
    NodePtr aNeighbor = _map->getNode(neighbors[i]);
    LOG_VART(aNeighbor->getElementId());
    NodeToWayMap::const_iterator it = n2w->find(neighbors[i]);
    if (aNeighbor->getStatus() == s && it != n2w->end() && it->second.size() >= 2)
    {
      double score = _nm.scorePair(nid, neighbors[i]);
      LOG_VART(QString::number(score, 'g', 10));

      if (score > 0.0)
      {
        Match m;
        m.nid1 = nid;
        m.nid2 = neighbors[i];
        m.score = score;
        matches.push_back(m);
        sum += m.score;
      }
    }
  }
  LOG_VART(matches.size());

  // don't go any lower than 1.0, that would scale the values up and made it look unrealistically
  // confident if there was only one intersection in a region.
  sum = max(1.0, sum);
  LOG_VART(sum);

  for (list<Match>::iterator it = matches.begin(); it != matches.end(); ++it)
  {
    it->p = it->score / sum;
    LOG_VART(it->p);
  }
}

void RubberSheet::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;

  // This has to be done here vs setConfiguration, b/c we may need a map to pass to some criteria.
  setCriteria(ConfigOptions().getRubberSheetElementCriteria(), map);

  if (!_criteria)
  {
    // check our way limit first
    if (_maxAllowedWays != -1 && map->getWayCount() > _maxAllowedWays)
    {
      LOG_WARN(
        "Skipping rubber sheeting of map having " <<
        StringUtils::formatLargeNumber(map->getWayCount()) << " ways out of a " <<
        StringUtils::formatLargeNumber(_maxAllowedWays) <<
        " maximum allowed for rubber sheeting.");
      return;
    }

    // no filtering criteria, so rubbersheet everything
    _calcAndApplyTransform(map);
  }
  else
  {
    // rubbersheet a filtered subset of the input
    _filterCalcAndApplyTransform(map);
  }
  _numProcessed = map->getWayCount();
}

bool RubberSheet::_calcAndApplyTransform(OsmMapPtr& map)
{
  LOG_INFO(
    "Rubber sheeting map having " <<
    StringUtils::formatLargeNumber(map->getWayCount()) << " ways out of a " <<
    StringUtils::formatLargeNumber(_maxAllowedWays) << " maximum allowed for rubber sheeting.");
  OsmMapWriterFactory::writeDebugMap(map, "rubbersheet-before-transform-calc");

  std::shared_ptr<OGRSpatialReference> oldSrs = _projection;
  bool success = calculateTransform(map);
  _projection = oldSrs;
  OsmMapWriterFactory::writeDebugMap(map, "rubbersheet-after-calculate-transform");

  if (success)
  {
    success = applyTransform(map);
    _numAffected = map->getWayCount();
    OsmMapWriterFactory::writeDebugMap(map, "rubbersheet-after-apply-transform");
  }

  return success;
}

void RubberSheet::_filterCalcAndApplyTransform(OsmMapPtr& map)
{
  // Potentially filtering out potentially multiple feature types here. Not sure if it would be
  // beneficial to rubbersheet each criterion separately...
  LOG_DEBUG("Filtering map before rubbersheeting...");
  OsmMapWriterFactory::writeDebugMap(map, "rubbersheet-before-filtering");

  _projection = map->getProjection();

  ElementCriterionPtr roundaboutFilter(
    OrCriterionPtr(
      new OrCriterion(
        std::shared_ptr<RoundaboutCriterion>(new RoundaboutCriterion()),
        std::shared_ptr<TagKeyCriterion>(new TagKeyCriterion(MetadataTags::HootSpecial())))));
  std::shared_ptr<CopyMapSubsetOp> mapCopier;

  // copy out elements meeting the filter criteria into a map
  OsmMapPtr toModify(new OsmMap());
  ElementCriterionPtr toModifyFilter(
    new ChainCriterion(_criteria, NotCriterionPtr(new NotCriterion(roundaboutFilter))));
  LOG_VARD(_criteria->toString());
  mapCopier.reset(new CopyMapSubsetOp(map, toModifyFilter));
  mapCopier->apply(toModify);
  LOG_DEBUG(
    "Element count for map being modified: " <<
    StringUtils::formatLargeNumber(toModify->getElementCount()));
  if (toModify->size() == 0)
  {
    // nothing to rubbersheet
    return;
  }

  // check our way limit
  if (_maxAllowedWays != -1 && toModify->getWayCount() > _maxAllowedWays)
  {
    LOG_WARN(
      "Skipping rubber sheeting of map having " <<
      StringUtils::formatLargeNumber(toModify->getWayCount()) << " ways out of a " <<
      StringUtils::formatLargeNumber(_maxAllowedWays) << " maximum allowed for rubber sheeting.");
    return;
  }

  OsmMapWriterFactory::writeDebugMap(toModify, "rubbersheet-to-modify");

  // copy out elements not meeting filter criteria into another map
  OsmMapPtr toNotModify(new OsmMap());
  ElementCriterionPtr toNotModifyFilter(
    new OrCriterion(NotCriterionPtr(new NotCriterion(_criteria)), roundaboutFilter));
  mapCopier.reset(new CopyMapSubsetOp(map, toNotModifyFilter));
  mapCopier->apply(toNotModify);
  LOG_DEBUG(
    "Element count for map not being modified: " <<
    StringUtils::formatLargeNumber(toNotModify->getElementCount()));
  OsmMapWriterFactory::writeDebugMap(toNotModify, "rubbersheet-to-not-modify");

  // run the rubbersheeting on just the elements we want to modify
  if (_calcAndApplyTransform(toModify))
  {
    map.reset(); // possibly reduce some memory consumption

    if (_projection.get() != 0)
    {
      MapProjector::project(toNotModify, _projection);
      MapProjector::project(toModify, _projection);
    }
    OsmMapWriterFactory::writeDebugMap(toNotModify, "rubbersheet-to-not-modify-after-reproject");
    OsmMapWriterFactory::writeDebugMap(toModify, "rubbersheet-to-modify-after-reproject");

    // append what we rubbersheeted back to what we didn't rubbersheet to become the final map
    toNotModify->append(toModify, true);
    map = toNotModify;
    LOG_DEBUG(
      "Element count for result map: " << StringUtils::formatLargeNumber(map->getElementCount()));
    OsmMapWriterFactory::writeDebugMap(map, "rubbersheet-result-map");
  }
}

bool RubberSheet::applyTransform(std::shared_ptr<OsmMap>& map)
{
  LOG_DEBUG("Applying transform...");

  _map = map;

  if (!_interpolator2to1)
  {
    const QString msg =
      "No appropriate interpolator was specified, skipping rubber sheet transform.";
    if (_logWarningWhenRequirementsNotFound)
    {
      LOG_WARN(msg);
    }
    else
    {
      LOG_INFO(msg);
    }
    return false;
  }

  if (_projection.get() != 0)
  {
    MapProjector::project(_map, _projection);
  }
  else
  {
    // if it is already planar then nothing will be done.
    MapProjector::projectToPlanar(_map);
  }

  int ctr = 0;
  const NodeMap& nm = map->getNodes();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    const NodePtr& n = it->second;

    if (_ref == false || n->getStatus() == Status::Unknown2)
    {
      Coordinate c = n->toCoordinate();
      Coordinate newC = _translate(c, n->getStatus());

      n->setX(newC.x);
      n->setY(newC.y);
    }

    ctr++;
    if (ctr % 1000 == 0)
    {
      PROGRESS_INFO(
        "Applied rubber sheet transform to " << StringUtils::formatLargeNumber(ctr) << " / " <<
        StringUtils::formatLargeNumber(nm.size()) << " nodes...");
    }
  }

  return true;
}

std::shared_ptr<DataFrame> RubberSheet::_buildDataFrame(Status s) const
{
  std::shared_ptr<DataFrame> df(new DataFrame());
  vector<string> labels;
  labels.push_back("x");
  labels.push_back("y");
  labels.push_back("dx");
  labels.push_back("dy");
  vector<int> types(4, DataFrame::Numerical);
  df->setFactorLabels(labels);
  df->setFactorTypes(types);

  vector<double> d(4);

  // TODO: This should be replaced with an circular error based method at some point.
  double multiplier;
  if (_ref)
  {
    multiplier = 1.0;
  }
  else
  {
    multiplier = 0.5;
  }

  for (size_t i = 0; i < _ties.size(); i++)
  {
    if (s == Status::Unknown1)
    {
      d[0] = _ties[i].c1.x;
      d[1] = _ties[i].c1.y;
      d[2] = -_ties[i].dx() * multiplier;
      d[3] = -_ties[i].dy() * multiplier;
    }
    else
    {
      d[0] = _ties[i].c2.x;
      d[1] = _ties[i].c2.y;
      d[2] = _ties[i].dx() * multiplier;
      d[3] = _ties[i].dy() * multiplier;
    }
    LOG_VART(d);
    df->addDataVector("", d);
  }

  return df;
}

std::shared_ptr<Interpolator> RubberSheet::_buildInterpolator(Status s) const
{
  std::shared_ptr<DataFrame> df = _buildDataFrame(s);

  vector<std::string> candidates;
  if (_interpolatorClassName.empty())
  {
    candidates = Factory::getInstance().getObjectNamesByBase(Interpolator::className());
  }
  else
  {
    candidates.push_back(_interpolatorClassName);
  }

  double bestError = numeric_limits<double>::max();
  std::shared_ptr<Interpolator> bestCandidate;
  for (size_t i = 0; i < candidates.size(); i++)
  {
    QElapsedTimer timer;
    timer.start();
    LOG_STATUS(
      "Running rubber sheet interpolator: (" << (i + 1) << " / " <<
      candidates.size() << "): " << candidates[i] << "...");

    std::shared_ptr<Interpolator> candidate(
      Factory::getInstance().constructObject<Interpolator>(candidates[i]));
    // Setting this upper limit prevents some runaway optimizations.  Those conditions were going
    // to be fixed as part of #2893 at one point but that was cancelled. The default config value
    // was determined in a way to be high enough so as not to affect existing tests, as well as
    // accomodate a reasonable runtime based on the size of the dataset.  Some tweaking to the value
    // may need to occur over time.
    int maxOptIterations = ConfigOptions().getRubberSheetMaxInterpolatorIterations();
    if (maxOptIterations == -1)
    {
      maxOptIterations = INT_MAX;
    }
    candidate->setMaxAllowedPerLoopOptimizationIterations(maxOptIterations);
    vector<string> ind;
    ind.push_back("x");
    ind.push_back("y");
    vector<string> dep;
    dep.push_back("dx");
    dep.push_back("dy");

    candidate->setData(df);
    candidate->setIndependentColumns(ind);
    candidate->setDependentColumns(dep);
    double error = candidate->estimateError();
    LOG_DEBUG("candidate: " << candidate->toString() << " RMSE: " << error);
    if (error < bestError)
    {
      bestCandidate = candidate;
      bestError = error;
    }
    LOG_DEBUG(
      "Max interpolator loop iterations: " << candidate->getMaxOptimizationLoopIterations());

    LOG_STATUS(
      "Rubber sheet interpolation complete in " <<
      StringUtils::millisecondsToDhms(timer.elapsed()) << " total for: " << candidates[i] << ".");
  }

  if (bestCandidate.get() == 0)
  {
    throw HootException("Unable to determine rubber sheeting interpolation candidate.");
  }

  LOG_DEBUG("Best candidate: " << bestCandidate->toString());

  return bestCandidate;
}

bool RubberSheet::calculateTransform(std::shared_ptr<OsmMap>& map)
{
  LOG_DEBUG("Calculating transform...");

  _map = map;

  // if it is already planar then nothing will be done.
  MapProjector::projectToPlanar(_map);
  _projection = _map->getProjection();

  return _findTies();
}

bool RubberSheet::_findTies()
{
  _nm.setMap(_map);
  // The search radius is two times the max circular error which handles if
  // both nodes are the worst circular error.
  _searchRadius = WorstCircularErrorVisitor::getWorstCircularError(_map) * 2;

  int ctr = 0;

  std::shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  // go through all the intersections w/ 2 or more ways intersecting
  for (NodeToWayMap::const_iterator it = n2w->begin(); it != n2w->end(); ++it)
  {
    long nid = it->first;
    LOG_VART(nid);
    const set<long>& wids = it->second;
    LOG_VART(wids);
    // find all the potential matches
    if (wids.size() >= 3)
    {
      // calculate the probability of each match and add it to our list
      _addIntersection(nid, wids);
    }

    ctr++;
    if (ctr % 1000 == 0)
    {
      PROGRESS_INFO(
        "Processed intersections for " << StringUtils::formatLargeNumber(ctr) << " / " <<
        StringUtils::formatLargeNumber(n2w->size()) << " nodes...");
    }
  }

  // go through all the paired intersections
  ctr = 0;
  for (MatchList::const_iterator it = _matches.begin(); it != _matches.end(); ++it)
  {
    NodePtr n = _map->getNode(it->first);
    LOG_VART(n->getElementId());
    LOG_VART(n->getStatus());
    // only look if this is Unknown1
    if (n->getStatus() == Status::Unknown1)
    {
      const list<Match>& l = it->second;
      for (list<Match>::const_iterator lt = l.begin(); lt != l.end(); ++lt)
      {
        // set the new score for the pair to the product of the pair
        Match m1 = *lt;
        LOG_TRACE(m1.score);
        const Match& m2 = _findMatch(m1.nid2, m1.nid1);
        LOG_TRACE(m2.score);
        m1.p = m1.p * m2.p;
        _finalPairs.push_back(m1);
      }
    }

    ctr++;
    if (ctr % 100 == 0)
    {
      PROGRESS_INFO(
        "Processed paired intersections for " << StringUtils::formatLargeNumber(ctr) << " / " <<
        StringUtils::formatLargeNumber(_matches.size()) << " matches...");
    }
  }
  LOG_VART(_finalPairs.size());
  sort(_finalPairs.begin(), _finalPairs.end(), compareMatches);

  set<long> touched;
  LOG_DEBUG("Found " << _finalPairs.size() << " potential tie points.");

  ctr = 0;
  for (size_t i = 0; i < _finalPairs.size(); ++i)
  {
    NodePtr n1 = _map->getNode(_finalPairs[i].nid1);
    LOG_VART(n1->getElementId());
    NodePtr n2 = _map->getNode(_finalPairs[i].nid2);
    LOG_VART(n2->getElementId());
    if (touched.find(n1->getId()) == touched.end() &&
        touched.find(n2->getId()) == touched.end())
    {
      if (_finalPairs[i].p > 0.5)
      {
        Tie t;
        t.c1 = n1->toCoordinate();
        t.c2 = n2->toCoordinate();
        LOG_VART(t.c1);
        LOG_VART(t.c2);
        _ties.push_back(t);
      }
      touched.insert(n1->getId());
      touched.insert(n2->getId());

      if (_debug)
      {
        n1->getTags()[MetadataTags::HootMatchScore()] = QString("%1").arg(_finalPairs[i].score);
        n1->getTags()[MetadataTags::HootMatchP()] = QString("%1").arg(_finalPairs[i].p);
        n1->getTags()[MetadataTags::HootMatchOrder()] =
          QString("%1 of %2").arg(i).arg(_finalPairs.size());
        n2->getTags()[MetadataTags::HootMatchP()] = QString("%1").arg(_finalPairs[i].p);
        n2->getTags()[MetadataTags::HootMatchOrder()] =
          QString("%1 of %2").arg(i).arg(_finalPairs.size());
      }
    }

    ctr++;
    if (ctr % 100 == 0)
    {
      PROGRESS_INFO(
        "Potential tie points processed: " << StringUtils::formatLargeNumber(ctr) << " / " <<
        StringUtils::formatLargeNumber(_finalPairs.size()));
    }
  }

  LOG_VARD(_ties.size());
  LOG_VARD(_minimumTies);
  if ((long)_ties.size() >= _minimumTies)
  {
    LOG_DEBUG(
      "Found " << _ties.size() << " tie points, out of a required minimum of " <<
      _minimumTies << ", which is enough to perform rubbersheeting.");

    // experimentally determine the best interpolator.
    _interpolatorClassName.clear();
    _interpolator2to1 = _buildInterpolator(Status::Unknown2);
    // make sure we use the same interpolator for both directions.
    _interpolatorClassName = _interpolator2to1->getClassName();
    LOG_DEBUG(_interpolator2to1->toString());
    if (_ref == false)
    {
      _interpolator1to2 = _buildInterpolator(Status::Unknown1);
      LOG_DEBUG(_interpolator1to2->toString());
    }
  }
  else
  {
    LOG_VARD(_failWhenMinTiePointsNotFound);
    if (_failWhenMinTiePointsNotFound)
    {
      throw HootException(
        QString("Error rubbersheeting due to not finding enough tie points.  ") +
        QString("The minimum allowable tie points configured is %1 and %2 tie points were found.")
          .arg(QString::number(_minimumTies))
          .arg(QString::number(_ties.size())));
    }
    else
    {
      const QString msg =
        QString("Skipping rubbersheeting due to not finding enough tie points.  ") +
        QString("The minimum allowable tie points configured is %1 and %2 tie points were found.")
          .arg(QString::number(_minimumTies))
          .arg(QString::number(_ties.size()));
      if (_logWarningWhenRequirementsNotFound)
      {
        LOG_WARN(msg);
      }
      else
      {
        LOG_INFO(msg);
      }
    }

    _interpolator1to2.reset();
    _interpolator2to1.reset();
    _interpolatorClassName.clear();

    return false;
  }

  return true;
}

const RubberSheet::Match& RubberSheet::_findMatch(long nid1, long nid2)
{
  const list<Match>& matches = _matches[nid1];

  for (list<Match>::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    LOG_VART(it->nid2);
    if (it->nid2 == nid2)
    {
      return *it;
    }
  }

  return _emptyMatch;
}

std::shared_ptr<Interpolator> RubberSheet::_readInterpolator(QIODevice& is)
{
  QDataStream ds(&is);
  QString projStr;
  ds >> projStr;
  _projection.reset(new OGRSpatialReference());
  _projection->importFromProj4(projStr.toUtf8().data());

  QString interpolatorClass;
  ds >> interpolatorClass;
  std::shared_ptr<Interpolator> result;
  result.reset(Factory::getInstance().constructObject<Interpolator>(
    interpolatorClass.toStdString()));
  result->readInterpolator(is);
  return result;
}

Coordinate RubberSheet::_translate(const Coordinate& c, Status s)
{
  _matchPoint.resize(2);
  _matchPoint[0] = c.x;
  _matchPoint[1] = c.y;

  const vector<double>* delta;
  if (s == Status::Unknown1)
  {
    delta = &_interpolator1to2->interpolate(_matchPoint);
  }
  else
  {
    delta = &_interpolator2to1->interpolate(_matchPoint);
  }

  return Coordinate(c.x + (*delta)[0], c.y + (*delta)[1]);
}

void RubberSheet::_writeInterpolator(
  const std::shared_ptr<const Interpolator>& interpolator, QIODevice& os) const
{
  // this could be modified to write an empty or null interpolator if needed.
  if (!interpolator)
  {
    throw HootException("An invalid interpolator was specified. Too few tie points?");
  }
  QDataStream ds(&os);
  char* projStr = 0;
  _projection->exportToProj4(&projStr);
  ds << QString(projStr);
  delete [] projStr;

  ds << QString::fromStdString(interpolator->getClassName());
  interpolator->writeInterpolator(os);
}

vector<double> RubberSheet::calculateTiePointDistances()
{
  if (_ties.size() == 0)
  {
    throw HootException("No tie points have been generated.");
  }

  vector<double> tiePointDistances;
  for (vector<Tie>::const_iterator it = _ties.begin(); it != _ties.end(); ++it)
  {
    Tie tiePoint = *it;
    LOG_TRACE(tiePoint.toString());
    tiePointDistances.push_back(tiePoint.c1.distance(tiePoint.c2));
  }
  return tiePointDistances;
}

QStringList RubberSheet::getCriteria() const
{
  QStringList criteria;
  criteria.append(QString::fromStdString(LinearCriterion::className()));
  criteria.append(QString::fromStdString(PolygonCriterion::className()));
  return criteria;
}

}
