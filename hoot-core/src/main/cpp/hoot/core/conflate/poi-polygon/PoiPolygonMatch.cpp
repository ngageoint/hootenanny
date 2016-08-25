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
#include "PoiPolygonMatch.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/MatchThreshold.h>

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

//QString PoiPolygonMatch::_testUuid = "{87f8a061-d9f1-5bc9-ac28-27f07c8febae}";

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_matchDistance(ConfigOptions().getPoiPolygonMatchDistance()),
_reviewDistance(ConfigOptions().getPoiPolygonMatchReviewDistance()),
_nameScoreThreshold(ConfigOptions().getPoiPolygonMatchNameThreshold()),
_typeScoreThreshold(ConfigOptions().getPoiPolygonMatchTypeThreshold()),
_badGeomCount(0)
{
  _calculateMatch(map, eid1, eid2);
}

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf,
                                 double matchDistance, double reviewDistance,
                                 double nameScoreThreshold, double typeScoreThreshold) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_matchDistance(matchDistance),
_reviewDistance(reviewDistance),
_nameScoreThreshold(nameScoreThreshold),
_typeScoreThreshold(typeScoreThreshold),
_badGeomCount(0)
{
  _calculateMatch(map, eid1, eid2);
}

void PoiPolygonMatch::_calculateMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                      const ElementId& eid2)
{
  ConstElementPtr e1 = map->getElement(eid1);
  ConstElementPtr e2 = map->getElement(eid2);

  ConstElementPtr poi, poly;
  if (OsmSchema::getInstance().isPoiIsh(e1) && OsmSchema::getInstance().isBuildingIsh(e2))
  {
    _poiEid = eid1;
    _polyEid = eid2;
    poi = e1;
    poly = e2;
  }
  else if (OsmSchema::getInstance().isPoiIsh(e2) && OsmSchema::getInstance().isBuildingIsh(e1))
  {
    _poiEid = eid2;
    _polyEid = eid1;
    poi = e2;
    poly = e1;
  }
  else
  {
    LOG_WARN(e1->toString());
    LOG_WARN(e2->toString());
    throw IllegalArgumentException("Expected a POI & polygon, got: " + eid1.toString() + " " +
                                   eid2.toString());
  }

  shared_ptr<Geometry> gpoly = ElementConverter(map).convertToGeometry(poly);
  //may need a better way to handle this...(tried isValid)
  if (QString::fromStdString(gpoly->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      LOG_WARN("Invalid polygon passed to PoiPolygonMatchCreator: " << gpoly->toString());
    }
    _c.setMiss();
    return;
  }
  shared_ptr<Geometry> gpoi = ElementConverter(map).convertToGeometry(poi);

  const bool typeMatch = _calculateTypeMatch(map, poi, poly);

  const double nameScore = _calculateNameScore(poi, poly);
  const bool nameMatch = nameScore >= _nameScoreThreshold;

  // calculate the 2 sigma for the distance between the two objects
  const double sigma1 = e1->getCircularError() / 2.0;
  const double sigma2 = e1->getCircularError() / 2.0;
  const double ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;

  const double distance = gpoly->distance(gpoi.get());
  const double matchDistance = _matchDistance;
  const double reviewDistance = _reviewDistance + ce;
  const bool closeMatch = distance <= reviewDistance;

  int evidence = 0;
  evidence += typeMatch ? 1 : 0;
  evidence += nameMatch ? 1 : 0;
  evidence += distance <= matchDistance ? 2 : 0;

  if (!closeMatch)
  {
    _c.setMiss();
  }
  else if (evidence >= 3)
  {
    _c.setMatch();
  }
  else if (evidence >= 1)
  {
    _c.setReview();
  }
  else
  {
    _c.setMiss();
  }

 /* if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    //_uuid1 = e1->getTags().get("uuid");
    //_uuid2 = e2->getTags().get("uuid");
    QStringList names1 = e1->getTags().getNames();
    names1.append(e1->getTags().getPseudoNames());
    //_names1 = names1.join(",");
    QStringList names2 = e2->getTags().getNames();
    names2.append(e2->getTags().getPseudoNames());
    //_names2 = names2.join(",");
    //_circularError1 = e1->getCircularError();
    //_circularError2 = e2->getCircularError();

    LOG_VARD(_eid1);
    LOG_VARD(e1->getTags().get("uuid"));
    LOG_VARD(e1->getTags());
    LOG_VARD(_eid2);
    LOG_VARD(e2->getTags().get("uuid"));
    LOG_VARD(e2->getTags());
    LOG_VARD(typeMatch);
    LOG_VARD(nameMatch);
    LOG_VARD(nameScore);
    LOG_VARD(names1);
    LOG_VARD(names2);
    LOG_VARD(closeMatch);
    LOG_VARD(distance);
    LOG_VARD(reviewDistance);
    LOG_VARD(ce);
    LOG_VARD(e1->getCircularError());
    LOG_VARD(e2->getCircularError());
    LOG_VARD(evidence);
    LOG_DEBUG("**************************");
  }*/
}

double PoiPolygonMatch::_calculateNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  return
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))))
   .extract(e1, e2);
}

bool PoiPolygonMatch::_calculateTypeMatch(const ConstOsmMapPtr& /*map*/, ConstElementPtr e1,
                                          ConstElementPtr e2)
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();

  //be a little more restrictive with restaurants
  if (t1.get("amenity") == "restaurant" &&
      t2.get("amenity") == "restaurant" &&
      t1.contains("cuisine") && t2.contains("cuisine") &&
      t1.get("cuisine").toLower() != t2.get("cuisine").toLower())
  {
    return false;
  }

  const double tagScore = _getTagScore(e1, e2);

  /*if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    LOG_VARD(tagScore);
  }*/

  return tagScore >= _typeScoreThreshold;
}

double PoiPolygonMatch::_getTagScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  double result = 0.0;

  const QStringList t1List = _getRelatedTags(e1->getTags());
  const QStringList t2List = _getRelatedTags(e2->getTags());

  for (int i = 0; i < t1List.size(); i++)
  {
    for (int j = 0; j < t2List.size(); j++)
    {
      result = max(OsmSchema::getInstance().score(t1List.at(i), t2List.at(j)), result);
      /*if (e1->getTags().get("uuid") == _testUuid ||
          e2->getTags().get("uuid") == _testUuid)
      {
        LOG_VARD(t1List.at(i));
        LOG_VARD(t2List.at(j));
        LOG_VARD(result);
      }*/
    }
  }

  return result;
}

QStringList PoiPolygonMatch::_getRelatedTags(const Tags& tags) const
{
  QStringList tagsList;
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
  {
    if ((OsmSchema::getInstance().getCategories(it.key(), it.value()) &
         (OsmSchemaCategory::building() | OsmSchemaCategory::use() | OsmSchemaCategory::poi()))
          != OsmSchemaCategory::Empty)
    {
      tagsList.append(it.key() + "=" + it.value());
    }
  }
  return tagsList;
}

set< pair<ElementId, ElementId> > PoiPolygonMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_poiEid, _polyEid));
  return result;
}

map<QString, double> PoiPolygonMatch::getFeatures(const shared_ptr<const OsmMap>& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

QString PoiPolygonMatch::toString() const
{
  return QString("PoiPolygonMatch %1 %2 P: %3").arg(_poiEid.toString()).
    arg(_polyEid.toString()).arg(_c.toString());

  /*QString str =
    QString("PoiPolygonMatch %1 %2 P: %3").arg(_poiEid.toString()).
      arg(_polyEid.toString()).arg(_c.toString());
  str += " UUID1: " + _uuid1 + "\n";
  str += "UUID2: " + _uuid2 + "\n";
  str += "type match: " + QString::number(_typeMatch) + "\n";
  str += "name match: " + QString::number(_nameMatch) + "\n";
  str += "name score: " + QString::number(_nameScore) + "\n";
  str += "names 1: " + _names1 + "\n";
  str += "names 2: " + _names2 + "\n";
  str += "close match: " + QString::number(_closeMatch) + "\n";
  str += "distance: " + QString::number(_distance) + "\n";
  str += "review distance: " + QString::number(_reviewDistance) + "\n";
  str += "overall circular error: " + QString::number(_ce) + "\n";
  str += "circular error 1: " + QString::number(_circularError1) + "\n";
  str += "circular error 2: " + QString::number(_circularError2) + "\n";
  str += "evidence: " + QString::number(_evidence);
  return str;*/
}

}

