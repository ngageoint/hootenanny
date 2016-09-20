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

// gdal
#include <ogr_geometry.h>

// geos
#include <geos/geom/Geometry.h>
//#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>
//using namespace geos::geom;

// hoot
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EdgeDistanceExtractor.h>

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

QString PoiPolygonMatch::_testUuid = "{db8f0c0e-35c2-5657-ac30-811e2c1554de}";
QMultiMap<QString, double> PoiPolygonMatch::_poiMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_polyMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_poiReviewRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_polyReviewRefIdsToDistances;

//TODO: reduce these constructors

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_badGeomCount(0),
_map(map),
_distance(-1.0),
_nameScore(-1.0),
_nameMatch(false),
_exactNameMatch(false),
_matchDistance(ConfigOptions().getPoiPolygonMatchDistance()),
_reviewDistance(ConfigOptions().getPoiPolygonMatchReviewDistance()),
_nameScoreThreshold(ConfigOptions().getPoiPolygonMatchNameThreshold()),
_typeScoreThreshold(ConfigOptions().getPoiPolygonMatchTypeThreshold())
{
  _calculateMatch(eid1, eid2);
}

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf,
                                 set<ElementId> areaIds = set<ElementId>()) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_badGeomCount(0),
_map(map),
_distance(-1.0),
_nameScore(-1.0),
_nameMatch(false),
_exactNameMatch(false),
_matchDistance(ConfigOptions().getPoiPolygonMatchDistance()),
_reviewDistance(ConfigOptions().getPoiPolygonMatchReviewDistance()),
_nameScoreThreshold(ConfigOptions().getPoiPolygonMatchNameThreshold()),
_typeScoreThreshold(ConfigOptions().getPoiPolygonMatchTypeThreshold()),
_areaIds(areaIds)
{
  _calculateMatch(eid1, eid2);
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
_badGeomCount(0),
_map(map),
_distance(-1.0),
_nameScore(-1.0),
_nameMatch(false),
_exactNameMatch(false),
_matchDistance(matchDistance),
_reviewDistance(reviewDistance),
_nameScoreThreshold(nameScoreThreshold),
_typeScoreThreshold(typeScoreThreshold)
{
  _calculateMatch(eid1, eid2);
}

bool PoiPolygonMatch::isPoly(const Element& e)
{
  return OsmSchema::getInstance().isArea(e.getTags(), e.getElementType()) &&
         (OsmSchema::getInstance().getCategories(e.getTags()).intersects(
           OsmSchemaCategory::building() | OsmSchemaCategory::poi()) ||
          e.getTags().getNames().size() > 0);
}

bool PoiPolygonMatch::isPoi(const Element& e)
{
  return e.getElementType() == ElementType::Node &&
         (OsmSchema::getInstance().getCategories(e.getTags()).intersects(
           OsmSchemaCategory::building() | OsmSchemaCategory::poi()) ||
          e.getTags().getNames().size() > 0);
}

bool PoiPolygonMatch::isArea(const Element& e)
{
  return isPoly(e) && !OsmSchema::getInstance().isBuilding(e.getTags(), e.getElementType());
}

void PoiPolygonMatch::_calculateMatch(const ElementId& eid1, const ElementId& eid2)
{
  ConstElementPtr e1 = _map->getElement(eid1);
  ConstElementPtr e2 = _map->getElement(eid2);

  ConstElementPtr poi, poly;
  bool e1IsPoi = false;
  if (isPoi(*e1) && isPoly(*e2))
  {
    _poiEid = eid1;
    _polyEid = eid2;
    poi = e1;
    poly = e2;
    e1IsPoi = true;
  }
  else if (isPoi(*e2) && isPoly(*e1))
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

  shared_ptr<Geometry> gpoly = ElementConverter(_map).convertToGeometry(poly);
  //may need a better way to handle this...(already tried using isValid())
  if (QString::fromStdString(gpoly->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      LOG_WARN("Invalid polygon passed to PoiPolygonMatchCreator: " << gpoly->toString());
    }
    _class.setMiss();
    return;
  }
  shared_ptr<Geometry> gpoi = ElementConverter(_map).convertToGeometry(poi);

  _distance = gpoly->distance(gpoi.get());
  _nameScore = _getNameScore(poi, poly);
  _nameMatch = _nameScore >= _nameScoreThreshold;
  _exactNameMatch = _getExactNameScore(poi, poly) == 1.0;

  double ce = -1.0;
  double matchDistance = -1.0;
  double reviewDistance = -1.0;
  double reviewDistancePlusCe = -1.0;
  bool closeMatch = false;
  bool typeMatch = false;
  bool addressMatch = false;
  double typeScore = -1.0;
  int evidence = -1;

  if (!_triggersParkRule(poi, poly, gpoly, gpoi))
  {
    // calculate the 2 sigma for the distance between the two objects
    const double sigma1 = e1->getCircularError() / 2.0;
    const double sigma2 = e1->getCircularError() / 2.0;
    ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;

    matchDistance =
      max(_getMatchDistanceForType(_t1BestKvp), _getMatchDistanceForType(_t2BestKvp));
    reviewDistance =
      max(_getReviewDistanceForType(_t1BestKvp), _getReviewDistanceForType(_t2BestKvp));
    reviewDistancePlusCe = reviewDistance + ce;
    closeMatch = _distance <= reviewDistancePlusCe;

    typeScore = _getTypeScore(poi, poly);
    typeMatch = typeScore >= _typeScoreThreshold;
    //const bool exactTypeMatch = typeScore == 1.0;

    addressMatch = _getAddressMatch(poly, poi);

    evidence = 0;
    evidence += typeMatch ? 1 : 0;
    evidence += _nameMatch ? 1 : 0;
    evidence += addressMatch ? 1 : 0;
    evidence += _distance <= matchDistance ? 2 : 0;

    if (!closeMatch)
    {
      _class.setMiss();
    }
    else if (evidence >= 3)
    {
      _class.setMatch();
    }
    else if (evidence >= 1)
    {
      _class.setReview();
    }
    else
    {
      _class.setMiss();
    }
  }
  /*else
  {
    _class.setMiss();
  }*/

  if (Log::getInstance().getLevel() == Log::Debug)
  {
    //output feature distances for all feature types which fell within the match threshold
    const QString ref2 = poi->getTags().get("REF2");
    const QString review = poi->getTags().get("REVIEW");
    if (ref2 == poly->getTags().get("REF1").split(";")[0])
    {
      if (e1IsPoi)
      {
        _poiMatchRefIdsToDistances.insert(_t1BestKvp, _distance);
        _polyMatchRefIdsToDistances.insert(_t2BestKvp, _distance);
      }
      else
      {
        _poiMatchRefIdsToDistances.insert(_t2BestKvp, _distance);
        _polyMatchRefIdsToDistances.insert(_t1BestKvp, _distance);
      }
    }
    else if (review == poly->getTags().get("REF1").split(";")[0])
    {
      if (e1IsPoi)
      {
        _poiReviewRefIdsToDistances.insert(_t1BestKvp, _distance);
        _polyReviewRefIdsToDistances.insert(_t2BestKvp, _distance);
      }
      else
      {
        _poiReviewRefIdsToDistances.insert(_t2BestKvp, _distance);
        _polyReviewRefIdsToDistances.insert(_t1BestKvp, _distance);
      }
    }
  }

  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(_eid1);
    LOG_VARD(e1->getTags().get("uuid"));
    LOG_VARD(e1->getTags());
    LOG_VARD(_eid2);
    LOG_VARD(e2->getTags().get("uuid"));
    LOG_VARD(e2->getTags());
    LOG_VARD(typeScore);
    LOG_VARD(typeMatch);
    LOG_VARD(_nameMatch);
    LOG_VARD(_exactNameMatch);
    LOG_VARD(_nameScore);
    LOG_VARD(addressMatch);
    LOG_VARD(closeMatch);
    LOG_VARD(_distance);
    LOG_VARD(_matchDistance);
    LOG_VARD(_reviewDistance);
    LOG_VARD(reviewDistancePlusCe);
    LOG_VARD(ce);
    LOG_VARD(e1->getCircularError());
    LOG_VARD(e2->getCircularError());
    LOG_VARD(evidence);
    LOG_VARD(_class);
    LOG_DEBUG("**************************");
  }
}

void PoiPolygonMatch::printMatchDistanceInfo()
{
  _printMatchDistanceInfo("POI Match", _poiMatchRefIdsToDistances);
  _printMatchDistanceInfo("Poly Match", _polyMatchRefIdsToDistances);
  _printMatchDistanceInfo("POI Review", _poiReviewRefIdsToDistances);
  _printMatchDistanceInfo("Poly Review", _polyReviewRefIdsToDistances);
}

void PoiPolygonMatch::resetMatchDistanceInfo()
{
  _poiMatchRefIdsToDistances.clear();
  _polyMatchRefIdsToDistances.clear();
  _poiReviewRefIdsToDistances.clear();
  _polyReviewRefIdsToDistances.clear();
}

void PoiPolygonMatch::_printMatchDistanceInfo(const QString matchType,
                                              const QMultiMap<QString, double>& distanceInfo)
{
  foreach (QString type, distanceInfo.uniqueKeys())
  {
    if (!type.trimmed().isEmpty())
    {
      double maxDistance = 0.0;
      double minimumDistance = 99999;
      double averageDistance = 0.0;
      int numberOfEntries = 0;

      QList<double> distances = distanceInfo.values(type);
      qSort(distances.begin(), distances.end());
      double sumDist = 0.0;
      QString distancesStr = "";
      for (QList<double>::const_iterator itr = distances.begin(); itr != distances.end(); ++itr)
      {
        const double dist = *itr;
        maxDistance = max(maxDistance, dist);
        minimumDistance = min(minimumDistance, dist);
        sumDist += dist;
        distancesStr += QString::number(dist) + ", ";
        numberOfEntries++;
      }
      distancesStr.chop(2);
      averageDistance = sumDist / (double)distances.size();

      LOG_DEBUG(matchType.toUpper() << " distance info for type: " << type);
      LOG_VARD(maxDistance);
      LOG_VARD(minimumDistance);
      LOG_VARD(averageDistance);
      LOG_VARD(numberOfEntries);
      LOG_VARD(distancesStr);
      LOG_DEBUG("**************************");
    }
  }
}

double PoiPolygonMatch::_getNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  return
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))))
   .extract(e1, e2);
}

double PoiPolygonMatch::_getExactNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  //TODO: fix
  /*return
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new ExactStringDistance())))
   .extract(e1, e2);*/
   return ExactStringDistance().compare(e1->getTags().get("name"), e2->getTags().get("name"));
}

double PoiPolygonMatch::_getTypeScore(ConstElementPtr e1, ConstElementPtr e2)
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();

  //be a little more restrictive with restaurants
  if (t1.get("amenity") == "restaurant" &&
      t2.get("amenity") == "restaurant" &&
      t1.contains("cuisine") && t2.contains("cuisine"))
  {
    const QString t1Cuisine = t1.get("cuisine").toLower();
    const QString t2Cuisine = t2.get("cuisine").toLower();
    if (t1Cuisine != t2Cuisine &&
        //Don't return false on regional, since its location dependent and we don't take that into
        //account.
        t1Cuisine != "regional" && t2Cuisine != "regional")
    {
      return false;
    }
  }

  const double typeScore = _getTagScore(e1, e2);

  if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    LOG_VARD(typeScore);
  }

  return typeScore;
}

double PoiPolygonMatch::_getTagScore(ConstElementPtr e1, ConstElementPtr e2)
{
  double result = 0.0;
  _t1BestKvp = "";
  _t2BestKvp = "";

  const QStringList t1List = _getRelatedTags(e1->getTags());
  const QStringList t2List = _getRelatedTags(e2->getTags());

  for (int i = 0; i < t1List.size(); i++)
  {
    for (int j = 0; j < t2List.size(); j++)
    {
      const QString t1Kvp = t1List.at(i);
      const QString t2Kvp = t2List.at(j);
      const double score = OsmSchema::getInstance().score(t1Kvp, t2Kvp);
      if (score >= result)
      {
        if (!t1Kvp.isEmpty() && t1Kvp != "building=yes" && t1Kvp != "poi=yes")
        {
          _t1BestKvp = t1Kvp;
        }
        if (!t2Kvp.isEmpty() && t2Kvp != "building=yes" && t2Kvp != "poi=yes")
        {
          _t2BestKvp = t2Kvp;
        }
      }
      result = max(score, result);

      if (Log::getInstance().getLevel() == Log::Debug &&
          (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
      {
        LOG_VARD(t1List.at(i));
        LOG_VARD(t2List.at(j));
        LOG_VARD(result);
      }
    }
  }

  return result;
}

QStringList PoiPolygonMatch::_getRelatedTags(const Tags& tags) const
{
  QStringList tagsList;

  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
  {
    //TODO: hack - not sure the correct way to handle these concatenated values yet
    const QStringList values = it.value().split(";");
    for (int i = 0; i < values.size(); i++)
    {
      if ((OsmSchema::getInstance().getCategories(it.key(), it.value()) &
           (OsmSchemaCategory::building() | OsmSchemaCategory::use() | OsmSchemaCategory::poi()))
             != OsmSchemaCategory::Empty)
      {
        tagsList.append(it.key() + "=" + values.at(i));
      }
    }
  }

  return tagsList;
}

double PoiPolygonMatch::_getMatchDistanceForType(const QString /*typeKvp*/) const
{
  //dataset c
  /*if (typeKvp == "amenity=clinic")
  {
    return 10.0;
  }
  else if (typeKvp == "amenity=school" || typeKvp == "amenity=kindergarten")
  {
    return 20.0;
  }
  else if (typeKvp == "leisure=park")
  {
    return 10.0;
  }*/

  return _matchDistance;
}

double PoiPolygonMatch::_getReviewDistanceForType(const QString /*typeKvp*/) const
{
  /*if (typeKvp == "leisure=park")
  {
    return 20.0;
  }
  else if (typeKvp == "leisure=playground")
  {
    return 20.0;
  }
  else if (typeKvp == "sport=basketball")
  {
    return 50.0;
  }*/

  return _reviewDistance;
}

bool PoiPolygonMatch::_getAddressMatch(ConstElementPtr building, ConstElementPtr poi)
{
  Tags buildingTags = building->getTags();
  QString buildingHouseNum = buildingTags.get("addr:housenumber").trimmed();
  QString buildingStreet =
    Translator::getInstance().toEnglish(buildingTags.get("addr:street")).trimmed().toLower();
  QString buildingAddrComb;
  if (!buildingHouseNum.isEmpty() && !buildingStreet.isEmpty())
  {
    buildingAddrComb = buildingHouseNum + " " + buildingStreet;
  }
  QString buildingAddrTag =
    Translator::getInstance().toEnglish(buildingTags.get("address")).trimmed().toLower();
  if (buildingAddrComb.isEmpty() && buildingAddrTag.isEmpty())
  {
    //try to find the address from a building way node instead
    if (building->getElementType() == ElementType::Way)
    {
      ConstWayPtr wayBuilding = dynamic_pointer_cast<const Way>(building);
      const vector<long> wayNodeIds = wayBuilding->getNodeIds();
      for (size_t i = 0; i < wayNodeIds.size(); i++)
      {
        ConstElementPtr buildingWayNode = _map->getElement(ElementType::Node, wayNodeIds.at(i));
        buildingTags = buildingWayNode->getTags();
        buildingHouseNum = buildingTags.get("addr:housenumber").trimmed();
        buildingStreet =
          Translator::getInstance().toEnglish(buildingTags.get("addr:street")).trimmed().toLower();
        buildingAddrTag =
          Translator::getInstance().toEnglish(buildingTags.get("address")).trimmed().toLower();
        if (!buildingHouseNum.isEmpty() && !buildingStreet.isEmpty())
        {
          buildingAddrComb = buildingHouseNum + " " + buildingStreet;
        }
        if (!buildingAddrComb.isEmpty() || !buildingAddrTag.isEmpty())
        {
          break;
        }
      }
    }
    //haven't seen addresses yet in building relation node members
    /*else if (e2->getElementType() == ElementType::Relation)
      {
      }*/
  }
  if (buildingAddrComb.isEmpty() && buildingAddrTag.isEmpty())
  {
    return false;
  }

  const Tags poiTags = poi->getTags();
  const QString poiHouseNum = poiTags.get("addr:housenumber").trimmed();
  const QString poiStreet =
    Translator::getInstance().toEnglish(poiTags.get("addr:street")).trimmed().toLower();
  QString poiAddrComb;
  if (!poiHouseNum.isEmpty() && !poiStreet.isEmpty())
  {
    poiAddrComb = poiHouseNum + " " + poiStreet;
  }
  const QString poiAddrTag =
    Translator::getInstance().toEnglish(poiTags.get("address")).trimmed().toLower();
  if (poiAddrComb.isEmpty() && poiAddrTag.isEmpty())
  {
    return false;
  }

  if (Log::getInstance().getLevel() == Log::Debug &&
      (buildingTags.get("uuid") == _testUuid || poiTags.get("uuid") == _testUuid))
  {
    LOG_VARD(buildingAddrComb);
    LOG_VARD(poiAddrComb);
    LOG_VARD(buildingAddrTag);
    LOG_VARD(poiAddrTag);
  }

  ExactStringDistance addrComp;
  return
    (!buildingAddrTag.isEmpty() && !poiAddrTag.isEmpty() &&
       addrComp.compare(buildingAddrTag, poiAddrTag) == 1.0) ||
    (!buildingAddrComb.isEmpty() && !poiAddrTag.isEmpty() &&
       addrComp.compare(buildingAddrComb, poiAddrTag) == 1.0) ||
    (!poiAddrComb.isEmpty() && !buildingAddrTag.isEmpty() &&
       addrComp.compare(poiAddrComb, buildingAddrTag) == 1.0) ||
    (!buildingAddrComb.isEmpty() && !poiAddrComb.isEmpty() &&
       addrComp.compare(buildingAddrComb, poiAddrComb) == 1.0);
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
    arg(_polyEid.toString()).arg(_class.toString());
}

bool PoiPolygonMatch::_elementIsPark(ConstElementPtr element) const
{
  return !OsmSchema::getInstance().isBuilding(element) &&
         (element->getTags().get("leisure") == "park" /*||
          _containsPartial("park", element->getTags().getNames())*/);
}

bool PoiPolygonMatch::_elementIsParkish(ConstElementPtr element) const
{
  if (OsmSchema::getInstance().isBuilding(element))
  {
    return false;
  }
  const QString leisureVal = element->getTags().get("leisure").toLower();
  return
    leisureVal == "garden" || element->getTags().get("sport") == "tennis"/* ||
      element->getTags().get("name").toLower().contains("playground") ||
      element->getTags().get("name").toLower().contains("play area")*/;
}

bool PoiPolygonMatch::_containsPartial(const QString key, const QStringList strList) const
{
  for (QStringList::const_iterator it = strList.constBegin(); it != strList.constEnd(); it++)
  {
    QString val = *it;
    if (val.contains(key, Qt::CaseInsensitive))
    {
      return true;
    }
  }
  return false;
}

bool PoiPolygonMatch::_triggersParkRule(ConstElementPtr poi, ConstElementPtr poly,
                                        shared_ptr<Geometry> gpoly, shared_ptr<Geometry> gpoi)
{
  bool triggersParkRule = false;

  const QString poiName = poi->getTags().get("name").toLower();
  const QString polyName = poly->getTags().get("name").toLower();

  const bool poiHasType =
    OsmSchema::getInstance().getCategories(poi->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  const bool poiIsParkArea = _elementIsPark(poi);
  const bool poiIsParkish = _elementIsParkish(poi);
  const bool poiIsPlayground =
    poi->getTags().get("leisure") == "playground" || poiName.contains("play area") ||
    poiName.contains("playground");

  const bool polyHasType =
    OsmSchema::getInstance().getCategories(poly->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  const bool polyIsParkArea = _elementIsPark(poly);
  const bool polyIsPlayground =
    poly->getTags().get("leisure") == "playground" || polyName.contains("play area") ||
    polyName.contains("playground");
  const bool polyIsBuilding = OsmSchema::getInstance().isBuilding(poly);

  bool polyVeryCloseToAnotherParkPoly = false;
  double parkPolyAngleHistVal = -1.0;
  double parkPolyOverlapVal = -1.0;
  bool otherParkPolyNameMatch = false;
  double otherParkPolyNameScore = -1.0;
  double poiToPolyNodeDist = DBL_MAX;
  double poiToOtherParkPolyNodeDist = DBL_MAX;
  //double distToOtherParkPoly = -1.0;
  //bool otherParkPolyContainsPoly = false;
  bool otherParkPolyHasName = false;

  const double polyArea = gpoly->getArea();

  set<ElementId>::const_iterator it = _areaIds.begin();
  while (it != _areaIds.end() && !polyVeryCloseToAnotherParkPoly)
  {
    ConstElementPtr area = _map->getElement(*it);
    if (area->getElementId() != poly->getElementId())
    {
      if (_elementIsPark(area))
      {
        //otherParkPolyHasName = area->getTags().getNames().size() > 0;
        //not sure why the one above didn't work
        otherParkPolyHasName = !area->getTags().get("name").trimmed().isEmpty();
        otherParkPolyNameScore = _getNameScore(poi, area);
        otherParkPolyNameMatch = otherParkPolyNameScore >= _nameScoreThreshold;
        shared_ptr<Geometry> areaGeom = ElementConverter(_map).convertToGeometry(area);
        //distToOtherParkPoly = areaGeom->distance(gpoly.get());
        //otherParkPolyContainsPoly = areaGeom->contains(gpoly.get());

        if (areaGeom->intersects(gpoly.get()))
        {
          parkPolyAngleHistVal = AngleHistogramExtractor().extract(*_map, area, poly);
          parkPolyOverlapVal = OverlapExtractor().extract(*_map, area, poly);

          //When just using intersection as the criteria, only found one instance when something
          //was considered as "very close" to a park poly when I didn't want it to be...so these
          //values set very low to weed that instance out...overlap at least, not sure angle hist is
          //actually doing much here.  Maybe I need to pull the area to area search dist down even
          //more...or bring back in edge dist?
          if (parkPolyAngleHistVal >= 0.05 && parkPolyOverlapVal >= 0.02)
          {
            polyVeryCloseToAnotherParkPoly = true;

            if (poly->getElementType() == ElementType::Way &&
                area->getElementType() == ElementType::Way)
            {
              //Calc the distance from the poi to the poly line instead of the poly itself.  Calcing
              //distance to the poly itself will always return 0 when the poi is in the poly.
              ConstWayPtr polyWay = dynamic_pointer_cast<const Way>(poly);
              shared_ptr<const LineString> polyLineStr =
                dynamic_pointer_cast<const LineString>(
                  ElementConverter(_map).convertToLineString(polyWay));
              poiToPolyNodeDist = polyLineStr->distance(gpoi.get());
              ConstWayPtr areaWay = dynamic_pointer_cast<const Way>(area);;
              shared_ptr<const LineString> areaLineStr =
                dynamic_pointer_cast<const LineString>(
                  ElementConverter(_map).convertToLineString(areaWay));
              poiToOtherParkPolyNodeDist = areaLineStr->distance(gpoi.get());
            }

            if (Log::getInstance().getLevel() == Log::Debug &&
                (poly->getTags().get("uuid") == _testUuid ||
                 area->getTags().get("uuid") == _testUuid))
            {
              LOG_DEBUG("poly examined and found very close to a park: " << poly->toString());
              LOG_DEBUG("park it was very close to: " << area->toString());
            }
          }
        }
      }
    }
    it++;
  }

  //If the POI is inside a poly that is very close to another park poly, declare miss if
  //the distance to the outer ring of the other park poly is shorter than the distance to the outer
  //ring of this poly and the other park poly has a name.
  if ((poiIsParkArea || poiIsParkish) && polyVeryCloseToAnotherParkPoly && _distance == 0 &&
      poiToOtherParkPolyNodeDist < poiToPolyNodeDist && poiToPolyNodeDist != DBL_MAX &&
      poiToOtherParkPolyNodeDist != DBL_MAX && otherParkPolyHasName)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #1...");
    }
    _class.setMiss();
    triggersParkRule = true;
  }
  //If the poi is not a park and being compared to a park polygon or a polygon that is "very close"
  //to another park poly, we want to be more restrictive on type matching, but only if the poi has
  //any type at all.  If the poi has no type, then behave as normal.  Also, let an exact name match
  //cause a review here, rather than a miss.
  else if ((polyIsParkArea || (polyVeryCloseToAnotherParkPoly && !polyHasType)) && !poiIsParkArea &&
           !poiIsParkish && poiHasType)
  {
    if (_exactNameMatch)
    {
      if (Log::getInstance().getLevel() == Log::Debug &&
          (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
      {
        LOG_DEBUG("Returning review per park rule #2...");
      }
      _class.setReview();
    }
    else
    {
      if (Log::getInstance().getLevel() == Log::Debug &&
          (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
      {
        LOG_DEBUG("Returning miss per park rule #2...");
      }
      _class.setMiss();
    }
    triggersParkRule = true;
  }
  //If the poi is a park, the poly it is being compared to is not a park or building, and that poly
  //is "very close" to another park poly that has a name match with the poi, then declare a miss
  //(exclude poly playgrounds from this).
  else if (poiIsParkArea && !polyIsParkArea && !polyIsBuilding && polyVeryCloseToAnotherParkPoly &&
           otherParkPolyNameMatch && !polyIsPlayground)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #3...");
    }
    _class.setMiss();
    triggersParkRule = true;
  }
  //Play areas areas can get matched to the larger parks they reside within.  We're setting a max
  //size for play area polys here to prevent that.  Unfortunately, most play areas are tagged as
  //parked, if tagged at all, so we're scanning the name tag here.
  else if (poiName.contains("play area") && polyArea > 25000) //TODO: move to config?
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #4...");
    }
    _class.setMiss();
    triggersParkRule = true;
  }


  if (Log::getInstance().getLevel() == Log::Debug &&
      (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(poiHasType);
    LOG_VARD(polyHasType);
    LOG_VARD(polyIsParkArea);
    LOG_VARD(poiIsParkish);
    LOG_VARD(poiIsParkArea);
    LOG_VARD(polyIsPlayground);
    LOG_VARD(polyIsBuilding);
    LOG_VARD(polyVeryCloseToAnotherParkPoly);
    LOG_VARD(parkPolyAngleHistVal);
    LOG_VARD(parkPolyOverlapVal);
    LOG_VARD(otherParkPolyNameMatch);
    LOG_VARD(otherParkPolyNameScore);
    LOG_VARD(poiToPolyNodeDist);
    LOG_VARD(poiToOtherParkPolyNodeDist);
    LOG_VARD(otherParkPolyHasName);
    LOG_VARD(polyArea);
    //LOG_VARD(distToOtherParkPoly);
    //LOG_VARD(otherParkPolyContainsPoly);
  }

  return triggersParkRule;
}

}

