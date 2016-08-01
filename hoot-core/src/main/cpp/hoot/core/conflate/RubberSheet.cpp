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

#include "RubberSheet.h"

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>

// Tgs
#include <tgs/Statistics/Normal.h>
#include <tgs/Interpolation/DelaunayInterpolator.h>
#include <tgs/Interpolation/IdwInterpolator.h>
#include <tgs/Interpolation/KernelEstimationInterpolator.h>
#include <tgs/RandomForest/DataFrame.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RubberSheet)

// register the interpolators that exist in TGS.
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
  _ref = ConfigOptions().getRubberSheetRef();
  _debug = ConfigOptions().getRubberSheetDebug();
  _minimumTies = ConfigOptions().getRubberSheetMinimumTies();
}

void RubberSheet::_addIntersection(long nid, const set<long>& /*wids*/)
{
  shared_ptr<Node> from = _map->getNode(nid);
  // the status type we're searching for.
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

  shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  double sum = 0.0;
  list<Match>& matches = _matches[nid];
  vector<long> neighbors = _map->getIndex().findNodes(from->toCoordinate(), _searchRadius);
  for (size_t i = 0; i < neighbors.size(); ++i)
  {
    shared_ptr<Node> aNeighbor = _map->getNode(neighbors[i]);
    NodeToWayMap::const_iterator it = n2w->find(neighbors[i]);
    if (aNeighbor->getStatus() == s && it != n2w->end() && it->second.size() >= 2)
    {
      double score = _nm.scorePair(nid, neighbors[i]);

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

  // don't go any lower than 1.0, that would scale the values up and made it look unrealistically
  // confident if there was only one intersection in a region.
  sum = max(1.0, sum);

  for (list<Match>::iterator it = matches.begin(); it != matches.end(); ++it)
  {
    it->p = it->score / sum;
  }
}

void RubberSheet::apply(shared_ptr<OsmMap>& map)
{
  shared_ptr<OGRSpatialReference> oldSrs = _projection;
  calculateTransform(map);
  _projection = oldSrs;

  applyTransform(map);
}

void RubberSheet::applyTransform(shared_ptr<OsmMap>& map)
{
  _map = map;

  if (!_interpolator2to1)
  {
    LOG_WARN("No appropriate interpolator was specified, skipping rubber sheet transform.");
    return;
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

  const NodeMap& nm = map->getNodeMap();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    const shared_ptr<Node>& n = it->second;

    if (_ref == false || n->getStatus() == Status::Unknown2)
    {
      Coordinate c = n->toCoordinate();
      Coordinate newC = _translate(c, n->getStatus());

      n->setX(newC.x);
      n->setY(newC.y);
    }
  }
}

shared_ptr<DataFrame> RubberSheet::_buildDataFrame(Status s) const
{
  shared_ptr<DataFrame> df(new DataFrame());
  vector<string> labels;
  labels.push_back("x");
  labels.push_back("y");
  labels.push_back("dx");
  labels.push_back("dy");
  vector<int> types(4, DataFrame::Numerical);
  df->setFactorLabels(labels);
  df->setFactorTypes(types);

  vector<double> d(4);

  /// @todo This should be replaced with an circular error based method at some point.
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
    df->addDataVector("", d);
  }

  return df;
}

shared_ptr<Interpolator> RubberSheet::_buildInterpolator(Status s) const
{
  shared_ptr<DataFrame> df = _buildDataFrame(s);

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
  shared_ptr<Interpolator> bestCandidate;
  for (size_t i = 0; i < candidates.size(); i++)
  {
    shared_ptr<Interpolator> candidate(Factory::getInstance().constructObject<Interpolator>(
      candidates[i]));
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
    LOG_INFO("candidate: " << candidate->toString() << " RMSE: " << error);
    if (error < bestError)
    {
      bestCandidate = candidate;
      bestError = error;
    }
  }

  if (bestCandidate.get() == 0)
  {
    throw HootException("Unable to determine interpolation candidate.");
  }

  LOG_INFO("Best candidate: " << bestCandidate->toString());

  return bestCandidate;
}

void RubberSheet::calculateTransform(shared_ptr<OsmMap>& map)
{
  _map = map;

  // if it is already planar then nothing will be done.
  MapProjector::projectToPlanar(_map);
  _projection = _map->getProjection();

  _findTies();
}

void RubberSheet::_findTies()
{
  _nm.setMap(_map);
  // The search radius is two times the max circular error which handles if
  // both nodes are the worst circular error.
  _searchRadius = WorstCircularErrorVisitor::getWorstCircularError(_map) * 2;

  shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  // go through all the intersections w/ 2 or more roads intersecting
  for (NodeToWayMap::const_iterator it = n2w->begin(); it != n2w->end(); ++it)
  {
    long nid = it->first;
    const set<long>& wids = it->second;
    // find all the potential matches
    if (wids.size() >= 3)
    {
      // calculate the probability of each match and add it to our list
      _addIntersection(nid, wids);
    }
  }

  // go through all the paired intersections
  for (MatchList::const_iterator it = _matches.begin(); it != _matches.end(); ++it)
  {
    shared_ptr<Node> n = _map->getNode(it->first);
    // only look if this is Unknown1
    if (n->getStatus() == Status::Unknown1)
    {
      const list<Match>& l = it->second;
      for (list<Match>::const_iterator lt = l.begin(); lt != l.end(); ++lt)
      {
        // set the new score for the pair to the product of the pair
        Match m1 = *lt;
        const Match& m2 = _findMatch(m1.nid2, m1.nid1);
        m1.p = m1.p * m2.p;
        _finalPairs.push_back(m1);
      }
    }
  }

  sort(_finalPairs.begin(), _finalPairs.end(), compareMatches);

  QStringList t;
  t << "amenity=cafe";
  t << "highway=stop";
  t << "amenity=place_of_worship";
  t << "amenity=parking";
  t << "amenity=toilets";
  t << "amenity=bench";

  set<long> touched;
  LOG_INFO("Found " << _finalPairs.size() << " potential ties.");

  for (size_t i = 0; i < _finalPairs.size(); ++i)
  {
    shared_ptr<Node> n1 = _map->getNode(_finalPairs[i].nid1);
    shared_ptr<Node> n2 = _map->getNode(_finalPairs[i].nid2);
    if (touched.find(n1->getId()) == touched.end() &&
        touched.find(n2->getId()) == touched.end())
    {
      if (_finalPairs[i].p > 0.5)
      {
        if (_debug)
        {
          QString k = t[i % t.size()].split("=")[0];
          QString v = t[i % t.size()].split("=")[1];

          n1->getTags()[k] = v;
          n2->getTags()[k] = v;
        }
        Tie t;
        t.c1 = n1->toCoordinate();
        t.c2 = n2->toCoordinate();
        _ties.push_back(t);
      }
      touched.insert(n1->getId());
      touched.insert(n2->getId());

      if (_debug)
      {
        n1->getTags()["hoot:match:score"] = QString("%1").arg(_finalPairs[i].score);
        n1->getTags()["hoot:match:p"] = QString("%1").arg(_finalPairs[i].p);
        n1->getTags()["hoot:match:order"] = QString("%1 of %2").arg(i).arg(_finalPairs.size());
        n2->getTags()["hoot:match:p"] = QString("%1").arg(_finalPairs[i].p);
        n2->getTags()["hoot:match:order"] = QString("%1 of %2").arg(i).arg(_finalPairs.size());
      }
    }
  }

  LOG_INFO("Found " << _ties.size() << " confident ties.");

  if ((long)_ties.size() >= _minimumTies)
  {
    // experimentally determine the best interpolator.
    _interpolatorClassName.clear();
    _interpolator2to1 = _buildInterpolator(Status::Unknown2);
    // make sure we use the same interpolator for both directions.
    _interpolatorClassName = _interpolator2to1->getClassName();
    LOG_INFO(_interpolator2to1->toString());
    if (_ref == false)
    {
      _interpolator1to2 = _buildInterpolator(Status::Unknown1);
      LOG_INFO(_interpolator1to2->toString());
    }
  }
  else
  {
    _interpolator1to2.reset();
    _interpolator2to1.reset();
    _interpolatorClassName.clear();
  }
}

const RubberSheet::Match& RubberSheet::_findMatch(long nid1, long nid2)
{
  const list<Match>& matches = _matches[nid1];

  for (list<Match>::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    if (it->nid2 == nid2)
    {
      return *it;
    }
  }

  return _emptyMatch;
}

shared_ptr<Interpolator> RubberSheet::_readInterpolator(QIODevice& is)
{
  QDataStream ds(&is);
  QString projStr;
  ds >> projStr;
  _projection.reset(new OGRSpatialReference());
  _projection->importFromProj4(projStr.toUtf8().data());

  QString interpolatorClass;
  ds >> interpolatorClass;
  shared_ptr<Interpolator> result;
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

void RubberSheet::_writeInterpolator(shared_ptr<const Interpolator> interpolator, QIODevice& os)
  const
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
    tiePointDistances.push_back(tiePoint.c1.distance(tiePoint.c2));
  }
  return tiePointDistances;
}

}
