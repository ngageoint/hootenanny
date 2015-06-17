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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CustomPoiMatch.h"

// hoot
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/schema/TagComparator.h>

// tgs
#include <tgs/Statistics/Normal.h>

namespace hoot
{

CustomPoiMatch::CustomPoiMatch(const ConstOsmMapPtr& map, const PoiRfClassifierPtr& rf,
  const ElementId& eid1, const ElementId& eid2, ConstMatchThresholdPtr mt) :
  Match(mt),
  _eid1(eid1),
  _eid2(eid2),
  _rf(rf)
{
  _calculateClassification(map);
}

void CustomPoiMatch::_calculateClassification(const ConstOsmMapPtr& map)
{
  _p = _rf->classify(map, _eid1, _eid2);
}

map<QString, double> CustomPoiMatch::getFeatures(const shared_ptr<const OsmMap>& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

set< pair<ElementId, ElementId> > CustomPoiMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
  return result;
}

double CustomPoiMatch::getProbability() const
{
  return _p.getMatchP();
}

bool CustomPoiMatch::isConflicting(const Match& other, const ConstOsmMapPtr& map) const
{
  const CustomPoiMatch* cpm = dynamic_cast<const CustomPoiMatch*>(&other);
  if (cpm == 0)
  {
    return true;
  }
  else
  {
    shared_ptr<const MatchThreshold> mt(new MatchThreshold());

    // if they don't have any EIDs in common then it isn't a conflict.
    if (_eid1 != cpm->_eid1 && _eid2 != cpm->_eid2 &&
        _eid1 != cpm->_eid2 && _eid2 != cpm->_eid1)
    {
      return false;
    }

    // make sure all combinations of matches are also considered matches.
    CustomPoiMatch m1(map, _rf, _eid1, cpm->_eid1, mt);
    CustomPoiMatch m2(map, _rf, _eid1, cpm->_eid2, mt);
    CustomPoiMatch m3(map, _rf, _eid2, cpm->_eid1, mt);
    CustomPoiMatch m4(map, _rf, _eid2, cpm->_eid2, mt);

    if (m1.getType() == MatchType::Match &&
        m2.getType() == MatchType::Match &&
        m3.getType() == MatchType::Match &&
        m4.getType() == MatchType::Match)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
}

bool CustomPoiMatch::_isDistanceMatch(const ConstNodePtr &n1, const ConstNodePtr &n2) const
{
  double d = n1->toCoordinate().distance(n2->toCoordinate());

  double s1 = n1->getCircularError() / 2.0;
  double s2 = n2->getCircularError() / 2.0;
  // calculated the expected sigma for the distance value.
  double sigma = sqrt(s1 * s1 + s2 * s2);

  // if the distance is within 2 sigma of the expected distance, then call it a match.
  return d < sigma * 2.0;
}

bool CustomPoiMatch::_isExactMatch(const ConstNodePtr& n1, const ConstNodePtr& n2) const
{
  // distance that is considered an exact match. Defaults to 1mm.
  Meters epsilon = conf().getDouble(epsilonDistanceKey(), 1e-3);

  bool result = true;

  if (n1->getElementId() == n2->getElementId())
  {
    // pass, return = true
  }
  else if (n1->toCoordinate().distance(n2->toCoordinate()) > epsilon)
  {
    result = false;
  }
  else
  {
    // if at least one name pair is an exact match then it is an exact string match.
    double stringDistance = NameExtractor(new ExactStringDistance()).extract(n1, n2);

    if (stringDistance != 1.0)
    {
      result = false;
    }
  }

  return result;
}

class CustomStringDistance : public StringDistance
{
public:
  virtual double compare(const QString& s1, const QString& s2) const
  {
    double result = 0.0;

    QString e1 = LevenshteinDistance().toEnglish(s1);
    QString e2 = LevenshteinDistance().toEnglish(s2);

    // if there is an exact string match, then PLACES calls it a match.
    if (s1 == s2)
    {
      result = 1.0;
    }
    // if the length is less than 5 and there are 0 or 1 differences, then PLACES calls it a match.
    else if (s1.length() < 5 || s2.length() < 5)
    {
      int d = LevenshteinDistance().distance(s1, s2);
      d = min<int>(LevenshteinDistance().distance(e1, e2), d);
      if (d <= 1)
      {
        result = 1.0;
      }
    }
    // If the length is >= 5 and there is an 80% or greater match, then PLACES calls it a match.
    else
    {
      double s = LevenshteinDistance().score(s1, s2);
      s = max(LevenshteinDistance().score(e1, e2), s);
      if (s >= 0.8)
      {
        result = 1.0;
      }
    }

    return result;
  }

  virtual QString toString() const
  {
    return "PlacesStringDistance";
  }
};

bool CustomPoiMatch::_isNameMatch(const ConstNodePtr& n1, const ConstNodePtr& n2) const
{

  // if at least one name pair is a match then call it a match
  double stringDistance = NameExtractor(new CustomStringDistance()).extract(n1, n2);

  bool result = true;
  if (stringDistance != 1.0)
  {
    result = false;
  }

  return result;
}

QString CustomPoiMatch::toString() const
{
  stringstream ss;
  ss << "CustomPoiMatch: " << _eid1 << ", " << _eid2 << " p: " << _p.toString();
  return QString::fromStdString(ss.str());
}

}
