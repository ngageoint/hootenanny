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
#include "PlacesPoiMatchCreator.h"

// GDAL
#include <ogr_spatialref.h>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/index/metric-hybrid/RFqHybridTree.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

// tgs
#include <tgs/System/SystemInfo.h>

#include "PlacesPoiMatch.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, PlacesPoiMatchCreator)

class TreeKey
{
public:
  TreeKey() {}
  TreeKey(const TreeKey& dd) :
    _e(dd._e),
    _name(dd._name)
  {}

  TreeKey(const QString& name) : _name(name) {}

  TreeKey(double x, double y, const QString& name) :
    _e(x, x, y, y),
    _name(name)
  {}
  TreeKey(const Coordinate& c, const QString& name) :
    _e(c.x, c.x, c.y, c.y),
    _name(name)
  {}

  const Envelope& getEnvelope() const { return _e; }

  const QString& getMetricElement() const { return _name; }

private:
  Envelope _e;
  QString _name;
};

std::ostream& operator<<(std::ostream & o, const TreeKey& tk)
{
  o << tk.getEnvelope().toString() << " " << tk.getMetricElement();
  return o;
}

/**
 * Searches the specified map for any building match potentials.
 */
class PlacesPoiMatchVisitor : public ElementVisitor
{
public:
  /**
   * @param matchStatus If the element's status matches this status then it is checked for a match.
   */
  PlacesPoiMatchVisitor(const ConstOsmMapPtr& map,
    vector<const Match*>& result, Meters worstCircularError, const Envelope& bounds,
    ConstMatchThresholdPtr threshold) :
    _map(map),
    _result(result),
    _bounds(bounds),
    _threshold(threshold)
  {
    _worstCircularError = worstCircularError;
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
    _maxGroupSize = 0;
    _rejectScore = 0.9;

    _buildIndex();

    // used for bounds checks.
    _transform.reset(OGRCreateCoordinateTransformation(_map->getProjection().get(),
                                                       OsmMap::getWgs84().get()));
  }

  ~PlacesPoiMatchVisitor()
  {
    LOG_DEBUG(SystemInfo::getMemoryUsageString());
  }

  void checkForMatch(const shared_ptr<const Node>& n)
  {
    // find other nearby candidates
    set<long> neighbors = _findNeighbors(n);

    ElementId from(n->getElementId());

    _elementsEvaluated++;
    int neighborCount = 0;
    vector<double> classes;

    for (set<long>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      ElementId eid(ElementType::Node, *it);
      const shared_ptr<const Node>& neighbor = _map->getNode(*it);

      // make sure we only create one match per pair. Use the node with the larger circular error
      // to make sure we do the larger of the two queries. If the CE is equal then use the eid
      // with the lower id. Arbitrary, but it will prevent duplicates.
      if ((neighbor->getCircularError() < n->getCircularError()) ||
          (neighbor->getCircularError() == n->getCircularError() && from.getId() < eid.getId()))
      {
        if (OsmSchema::getInstance().isPoi(*n))
        {
          // score each candidate and push it on the result vector
          PlacesPoiMatch* m = new PlacesPoiMatch(_map, from, eid, _threshold);
          // if we're confident this is a miss
          if (m->getClassification().getMissP() >= _rejectScore)
          {
            delete m;
          }
          else
          {
            _result.push_back(m);
            neighborCount++;
          }
        }
      }
    }

    _neighborCountSum += neighborCount;
    _neighborCountMax = std::max(_neighborCountMax, neighborCount);
  }

  virtual void visit(const ConstElementPtr& e)
  {
    if (isMatchCandidate(e))
    {
      checkForMatch(dynamic_pointer_cast<const Node>(e));
    }
  }

  bool isMatchCandidate(ConstElementPtr element)
  {
    if (element->getElementType() == ElementType::Node)
    {
      const shared_ptr<const Node>& n(dynamic_pointer_cast<const Node>(element));

      bool inBounds = true;
      // if the bounds is specified, make sure this node is inside the bounds.
      if (_bounds.isNull() == false)
      {
        Coordinate uc = _projectToWgs84(n->toCoordinate());
        inBounds = _bounds.contains(uc);
      }

      if (inBounds && OsmSchema::getInstance().isPoi(*n))
      {
        return true;
      }
    }
    return false;
  }

private:

  auto_ptr< hybrid::RFqHybridTree<TreeKey, long, LevenshteinDistance> > _index;
  const ConstOsmMapPtr& _map;
  vector<const Match*>& _result;
  set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  /// reject any manipulation with a miss score >= _rejectScore
  double _rejectScore;
  Meters _worstCircularError;
  auto_ptr<OGRCoordinateTransformation> _transform;
  Envelope _bounds;
  ConstMatchThresholdPtr _threshold;

  void _buildIndex()
  {
    LOG_DEBUG(SystemInfo::getMemoryUsageString());
    LOG_DEBUG("Creating index. (node count: " << _map->getNodeMap().size() << ")");
    vector<TreeKey> keys;
    vector<long> nids;

    // reserve some space to reduce reallocs
    keys.reserve(_map->getNodeMap().size() * 2);
    nids.reserve(_map->getNodeMap().size() * 2);

    for (NodeMap::const_iterator it = _map->getNodeMap().begin();
         it != _map->getNodeMap().end(); ++it)
    {
      const shared_ptr<const Node>& n = it->second;
      set<QString> allNames = _getNamePermutations(n->getTags().getNames());
      for (set<QString>::iterator it = allNames.begin(); it != allNames.end(); ++it)
      {
        TreeKey tk(n->toCoordinate(), *it);
        keys.push_back(tk);
        nids.push_back(n->getId());
      }
    }

    LOG_DEBUG(SystemInfo::getMemoryUsageString());
    LOG_DEBUG("Calling build index.");
    _index.reset(new hybrid::RFqHybridTree<TreeKey, long, LevenshteinDistance>());
    _index->buildIndex(keys, nids);
    LOG_DEBUG("Built index with " << keys.size() << " keys and " << _map->getNodeMap().size() <<
             " nodes.");
    LOG_DEBUG(SystemInfo::getMemoryUsageString());
  }

  set<long> _findNeighbors(const ConstNodePtr& n)
  {
    set<long> result;

    set<QString> allNames = _getNamePermutations(n->getTags().getNames());

    double minSimilarity = ConfigOptions().getPlacesMinimumEditSimilarity();

    for (set<QString>::const_iterator it = allNames.begin(); it != allNames.end(); ++it)
    {
      const QString& s = *it;
      // if the string we're comparing against is longer then D may be bigger than
      // (1 - minSimilarity) * string length. Account for that below.
      int D = (int)round((double)s.size() / minSimilarity) - s.size();
      set<long> r = _index->find(n->toCoordinate(), n->getCircularError(), s, D);
      result.insert(r.begin(), r.end());
    }

    return result;
  }

  set<QString> _getNamePermutations(QStringList names)
  {
    set<QString> result;

    for (int i = 0; i < names.size(); ++i)
    {
      result.insert(names[i]);
      result.insert(LevenshteinDistance::toEnglish(names[i]));
    }

    return result;
  }

  Coordinate _projectToWgs84(const Coordinate& c)
  {
    Coordinate result;

    result.x = c.x;
    result.y = c.y;
    _transform->Transform(1, &result.x, &result.y);

    return result;
  }
};

class WorstCircularErrorVisitor : public ElementConstOsmMapVisitor
{
public:
  Meters _worst;

  WorstCircularErrorVisitor()
  {
    _worst = -1;
  }

  Meters getWorstCircularError() { return _worst; }

  virtual void visit(const shared_ptr<const Element>& e)
  {
    _worst = max(_worst, e->getCircularError());
  }
};


PlacesPoiMatchCreator::PlacesPoiMatchCreator()
{
}

PlacesPoiMatchCreator::~PlacesPoiMatchCreator()
{
}

Match* PlacesPoiMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2)
{
  Match* result = 0;

  if (eid1.getType() == ElementType::Node && eid2.getType() == ElementType::Node)
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);

    if (OsmSchema::getInstance().isPoi(*e1) && OsmSchema::getInstance().isPoi(*e2))
    {
      result = new PlacesPoiMatch(map, eid1, eid2, getMatchThreshold());
    }
  }

  return result;
}

void PlacesPoiMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match *> &matches,
  ConstMatchThresholdPtr threshold)
{
  LOG_DEBUG(SystemInfo::getMemoryUsageString());
  {
    WorstCircularErrorVisitor wav;
    map->visitRo(wav);
    LOG_DEBUG("Worst circular error: " << wav.getWorstCircularError());
    PlacesPoiMatchVisitor v(map, matches, wav.getWorstCircularError(), _bounds, threshold);
    map->visitRo(v);
    LOG_DEBUG(SystemInfo::getMemoryUsageString());
  }
  LOG_DEBUG(SystemInfo::getMemoryUsageString());
}

vector<MatchCreator::Description> PlacesPoiMatchCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "PLACES Match Creator", MatchCreator::POI, false));

  return result;
}

bool PlacesPoiMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map)
{
  if (!_matchCandidateChecker.get())
  {
    vector<const Match*> emptyVec;
    _matchCandidateChecker.reset(
      new PlacesPoiMatchVisitor(map, emptyVec, -1.0, _bounds, shared_ptr<MatchThreshold>()));
  }
  return _matchCandidateChecker->isMatchCandidate(element);
}

shared_ptr<MatchThreshold> PlacesPoiMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getPoiMatchThreshold(), config.getPoiMissThreshold(),
                         config.getPoiReviewThreshold()));
  }
  return _matchThreshold;
}

}
