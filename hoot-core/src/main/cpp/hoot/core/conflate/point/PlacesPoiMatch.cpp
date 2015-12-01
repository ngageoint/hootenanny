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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PlacesPoiMatch.h"

// hoot
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/schema/TagComparator.h>

namespace hoot
{

QString PlacesPoiMatch::_matchName = "PLACES POI";

class PlacesStringDistance : public StringDistance
{
public:
  virtual double compare(const QString& s1, const QString& s2) const
  {
    double result = 0.0;

    QString e1 = LevenshteinDistance().toEnglish(s1);
    QString e2 = LevenshteinDistance().toEnglish(s2);

    // if there is an exact string match, then there is no need to do Levenshtein.
    if (s1 == s2)
    {
      result = 1.0;
    }
    else
    {
      double s = LevenshteinDistance().score(s1, s2);
      s = max(LevenshteinDistance().score(e1, e2), s);
      result = s;
    }

    return result;
  }

  virtual QString toString() const
  {
    return "PlacesStringDistance";
  }
};

PlacesPoiMatch::PlacesPoiMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
  const ElementId& eid2, ConstMatchThresholdPtr threshold) :
  Match(threshold),
  _eid1(eid1),
  _eid2(eid2)
{
  _goodnessThreshold = conf().getDouble(goodnessThresholdKey(), 0.3);
  _calculateClassification(map);
}

void PlacesPoiMatch::_calculateClassification(const ConstOsmMapPtr& map)
{
  ConstElementPtr e1 = map->getElement(_eid1);
  ConstElementPtr e2 = map->getElement(_eid2);

  ConstNodePtr n1 = dynamic_pointer_cast<const Node>(e1);
  ConstNodePtr n2 = dynamic_pointer_cast<const Node>(e2);

  if (n1 == 0 || n2 == 0)
  {
    throw HootException("Expected nodes, but got another element type.");
  }

  _p.clear();

  // If the points are within a small distance, epsilon, and at least one name pair is an exact
  // match.
  if (_isExactMatch(n1, n2))
  {
    _p.setMatchP(1.0);
  }
  // If the names are considered close enough and the distance is considered close enough
  else if (_calculateGoodness(n1, n2) >= _goodnessThreshold)
  {
    _p.setMatchP(1.0);
  }
  else
  {
    _p.setMissP(1.0);
  }
}

double PlacesPoiMatch::_calculateGoodness(const ConstNodePtr &n1, const ConstNodePtr &n2) const
{
  // This is the current understanding of goodness. This appears to be evolving.
  // See "Hootenanny - POI Conflation" for details.

  double d = n1->toCoordinate().distance(n2->toCoordinate());
  double dmax = max(n1->getCircularError(), n2->getCircularError());

  // if the distance is out of bounds then it is not a match.
  if (d > dmax)
  {
    return 0.0;
  }

  double wd1 = conf().getDouble(distanceWeightKey(), 0.1);
  double ws1 = conf().getDouble(stringWeightKey(), 0.9);
  // normalize so they add to 1.
  double wd = wd1 / (wd1 + ws1);
  double ws = ws1 / (wd1 + ws1);

  double s = NameExtractor(new PlacesStringDistance()).extract(n1, n2);

  ////
  // NOTE: If you make changes here you'll also likely need to modify the index query in
  // PlacesPoiMatchCreator.
  ////
  double G;
  // if the edit similarity is less than a threshold then it is no longer a candidate.
  if (s < conf().getDouble(minimumEditSimilarityKey(), minimumEditSimilarityDefault()))
  {
    G = 0.0;
  }
  else
  {
    G = sqrt(pow(wd * (1 - d / dmax), 2.0) + pow(ws * (s), 2.0));
  }

  return G;
}

set< pair<ElementId, ElementId> > PlacesPoiMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
  return result;
}

double PlacesPoiMatch::getProbability() const
{
  return _p.getMatchP();
}

bool PlacesPoiMatch::isConflicting(const Match& other, const ConstOsmMapPtr& /*threshold*/) const
{
  const PlacesPoiMatch* ppm = dynamic_cast<const PlacesPoiMatch*>(&other);
  if (ppm == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool PlacesPoiMatch::_isExactMatch(const ConstNodePtr& n1, const ConstNodePtr& n2) const
{
  // distance that is considered an exact match. Defaults to 1mm.
  Meters epsilon = conf().getDouble(epsilonDistanceKey(), 1e-3);

  bool result = true;

  if (n1->toCoordinate().distance(n2->toCoordinate()) > epsilon)
  {
    result = false;
  }
  else
  {
    // if at least one name pair is an exact match then it is an exact string match.
    double stringDistance = NameExtractor(new ExactStringDistance()).extract(n1, n2);

    if (stringDistance < 0.99)
    {
      result = false;
    }
  }

  return result;
}

QString PlacesPoiMatch::toString() const
{
  stringstream ss;
  ss << "PlacesPoiMatch: " << _eid1 << ", " << _eid2 << " p: " << _p.toString();
  return QString::fromStdString(ss.str());
}
}
