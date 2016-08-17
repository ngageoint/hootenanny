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

//QString PoiPolygonMatch::_testUuid = "{08cf2389-216b-5a49-afcd-5ce30cef9436}";
bool PoiPolygonMatch::_test = false;

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf)
{
  _calculateMatch(map, eid1, eid2);
}

void PoiPolygonMatch::_calculateMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                      const ElementId& eid2)
{
  if (!_test)
  {
    LOG_VARD(OsmSchema::getInstance().score("amenity=hospital", "use=healthcare"));
    LOG_VARD(OsmSchema::getInstance().score("building=school", "amenity=school"));
    LOG_VARD(OsmSchema::getInstance().score("amenity=hospital", "building=hospital"));
    LOG_VARD(OsmSchema::getInstance().score("building=transportation", "station=*"));
    LOG_VARD(OsmSchema::getInstance().score("shop=mall", "shop=*"));
    LOG_VARD(OsmSchema::getInstance().score("leisure=sports_centre", "sport=*"));
    LOG_VARD(OsmSchema::getInstance().score("leisure=sports_complex", "sport=*"));
    LOG_VARD(OsmSchema::getInstance().score("shop=car", "shop=car_repair"));
    LOG_VARD(OsmSchema::getInstance().score("leisure=sports_centre", "leisure=water_park"));
    LOG_VARD(OsmSchema::getInstance().score("leisure=sports_centre", "leisure=swimming_pool"));
    LOG_VARD(OsmSchema::getInstance().score("leisure=sports_centre", "leisure=swimming"));
    LOG_VARD(OsmSchema::getInstance().score("tourism=attraction", "tourism=zoo"));
    LOG_VARD(OsmSchema::getInstance().score("amenity=arts_centre", "amenity=theatre"));
    LOG_VARD(OsmSchema::getInstance().score("amenity=clinic", "amenity=hospital"));
    LOG_VARD(OsmSchema::getInstance().score("station=light_rail", "building=train_station"));
    LOG_VARD(OsmSchema::getInstance().score("historic=building", "historic=monument"));
    _test = true;
  }

  ConstElementPtr e1 = map->getElement(eid1);
  ConstElementPtr e2 = map->getElement(eid2);

  ConstElementPtr poi, poly;
  if (isPoiIsh(e1) && isBuildingIsh(e2))
  {
    _poiEid = eid1;
    _polyEid = eid2;
    poi = e1;
    poly = e2;
  }
  else if (isPoiIsh(e2) && isBuildingIsh(e1))
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
  shared_ptr<Geometry> gpoi = ElementConverter(map).convertToGeometry(poi);

  const bool typeMatch = _calculateTypeMatch(poi, poly);
  bool ancestorTypeMatch = false;
  if (ConfigOptions().getPoiPolygonUseTagAncestorTypeMatching())
  {
    ancestorTypeMatch = _calculateAncestorTypeMatch(map, poi, poly);
  }

  const double nameScore = _calculateNameScore(poi, poly);
  const bool nameMatch = nameScore >= ConfigOptions().getPoiPolygonMatchNameThreshold();

  // calculate the 2 sigma for the distance between the two objects
  const double sigma1 = e1->getCircularError() / 2.0;
  const double sigma2 = e1->getCircularError() / 2.0;
  const double ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;

  const double distance = gpoly->distance(gpoi.get());
  const double matchDistance = ConfigOptions().getPoiPolygonMatchDistance();
  const double reviewDistance = ConfigOptions().getPoiPolygonMatchReviewDistance() + ce;
  const bool closeMatch = distance <= reviewDistance;

  int evidence = 0;
  //also allowing common ancestors to be an additional piece of evidence seems to create
  //some extra reviews for things that would be missed otherwise
  evidence += typeMatch || ancestorMatch ? 1 : 0;
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

  /*if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    _uuid1 = e1->getTags().get("uuid");
    _uuid2 = e2->getTags().get("uuid");
    QStringList names1 = e1->getTags().getNames();
    names1.append(e1->getTags().getPseudoNames());
    _names1 = names1.join(",");
    QStringList names2 = e2->getTags().getNames();
    names2.append(e2->getTags().getPseudoNames());
    _names2 = names2.join(",");
    _circularError1 = e1->getCircularError();
    _circularError2 = e2->getCircularError();

    LOG_VARD(_eid1);
    LOG_VARD(e1->getTags().get("uuid"));
    LOG_VARD(e1->getTags());
    LOG_VARD(_eid2);
    LOG_VARD(e2->getTags().get("uuid"));
    LOG_VARD(e2->getTags());
    LOG_VARD(_typeMatch);
    LOG_VARD(_typeMatchAttributeKey);
    LOG_VARD(_typeMatchAttributeValue);
    LOG_VARD(_ancestorTypeMatch);
    LOG_VARD(_ancestorDistance);
    LOG_VARD(_nameMatch);
    LOG_VARD(_nameScore);
    LOG_VARD(names1);
    LOG_VARD(names2);
    LOG_VARD(_closeMatch);
    LOG_VARD(_distance);
    LOG_VARD(_reviewDistance);
    LOG_VARD(_ce);
    LOG_VARD(e1->getCircularError());
    LOG_VARD(e2->getCircularError());
    LOG_VARD(_evidence);
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

bool PoiPolygonMatch::_calculateTypeMatch(ConstElementPtr e1, ConstElementPtr e2)
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

  for (Tags::const_iterator it = t1.begin(); it != t1.end(); it++)
  {
    // if it is a use, POI, or building category
    if ((OsmSchema::getInstance().getCategories(it.key(), it.value()) &
         (OsmSchemaCategory::building() | OsmSchemaCategory::use() | OsmSchemaCategory::poi()))
          != OsmSchemaCategory::Empty)
    {
      //and any tag matches exactly
      bool result = t2.get(it.key()) == it.value();
      if (result)
      {
        //_typeMatchAttributeKey = it.key();
        //_typeMatchAttributeValue = it.value();
        return true;
      }
    }
  }

  if (ConfigOptions().getPoiPolygonUseSchemaMods())
  {
    //TODO: hacks - having trouble figuring out how to do this the right way...will fix in schema
    if ((t1.get("amenity").toLower() == "hospital" &&
         t2.get("use").toLower() == "healthcare") ||
        (t2.get("amenity").toLower() == "hospital" &&
         t1.get("use").toLower() == "healthcare"))
    {
      return true;
    }
    if (((t1.get("building") == "school" &&
         t2.get("amenity") == "school") ||
        (t2.get("building") == "school" &&
         t1.get("amenity") == "school")))
    {
      return true;
    }
    if ((t1.get("amenity").toLower() == "hospital" &&
         t2.get("building").toLower() == "hospital") ||
        (t2.get("amenity").toLower() == "hospital" &&
         t1.get("building").toLower() == "hospital"))
    {
      return true;
    }
    //dataset D
    if ((t1.get("building").toLower() == "transportation" &&
         t2.contains("station")) ||
        (t2.get("building").toLower() == "transportation" &&
         t1.contains("station")))
    {
      return true;
    }
    if ((t1.contains("shop") &&
         t2.get("shop").toLower() == "mall") ||
        (t2.contains("shop") &&
         t1.get("shop").toLower() == "mall"))
    {
      return true;
    }
    if ((t1.get("leisure").toLower() == "sports_centre" &&
         t2.contains("sport")) ||
        (t2.get("leisure").toLower() == "sports_centre" &&
         t1.contains("sport")))
    {
      return true;
    }
    if ((t1.get("leisure").toLower() == "sports_complex" &&
         t2.contains("sport")) ||
        (t2.get("leisure").toLower() == "sports_complex" &&
         t1.contains("sport")))
    {
      return true;
    }
    if ((t1.get("shop").toLower() == "car" &&
         t2.get("shop").toLower() == "car_repair") ||
        (t2.get("shop").toLower() == "car" &&
         t1.get("shop").toLower() == "car_repair"))
    {
      return true;
    }
    //TODO: this shouldn't be needed regardless
    /*if (t1.get("shop").toLower().contains("car") &&
        t2.get("shop").toLower().contains("car"))
    {
      LOG_VARD(OsmSchema::score("shop=car", "shop=car"));
      return true;
    }*/
    if ((t1.get("leisure").toLower() == "sports_centre" &&
         t2.get("leisure").toLower() == "water_park") ||
        (t2.get("leisure").toLower() == "sports_centre" &&
         t1.get("leisure").toLower() == "water_park"))
    {
      return true;
    }
    if ((t1.get("leisure").toLower() == "sports_centre" &&
         t2.get("leisure").toLower() == "swimming_pool") ||
        (t2.get("leisure").toLower() == "sports_centre" &&
         t1.get("leisure").toLower() == "swimming_pool"))
    {
      return true;
    }
    if ((t1.get("leisure").toLower() == "sports_centre" &&
         t2.get("sport").toLower() == "swimming") ||
        (t2.get("leisure").toLower() == "sports_centre" &&
         t1.get("sport").toLower() == "swimming"))
    {
      return true;
    }
    if ((t1.get("tourism").toLower() == "attraction" &&
         t2.get("tourism").toLower() == "zoo") ||
        (t2.get("tourism").toLower() == "attraction" &&
         t1.get("tourism").toLower() == "zoo"))
    {
      return true;
    }
    if ((t1.get("amenity").toLower() == "arts_centre" &&
         t2.get("amenity").toLower() == "theatre") ||
        (t2.get("amenity").toLower() == "arts_centre" &&
         t1.get("amenity").toLower() == "theatre"))
    {
      return true;
    }
    if ((t1.get("amenity").toLower() == "clinic" &&
         t2.get("amenity").toLower() == "hospital") ||
        (t2.get("amenity").toLower() == "clinic" &&
         t1.get("amenity").toLower() == "hospital"))
    {
      return true;
    }
    if ((t1.get("station").toLower() == "light_rail" &&
         t2.get("building").toLower() == "train_station") ||
        (t2.get("station").toLower() == "light_rail" &&
         t1.get("building").toLower() == "train_station"))
    {
      return true;
    }
    if ((t1.get("historic").toLower() == "building" &&
         t2.get("historic").toLower() == "monument") ||
        (t2.get("historic").toLower() == "building" &&
         t1.get("historic").toLower() == "monument"))
    {
      return true;
    }
  }

  return false;
}

bool PoiPolygonMatch::_calculateAncestorTypeMatch(const ConstOsmMapPtr& map, ConstElementPtr e1,
                                                  ConstElementPtr e2)
{
  QStringList types;

  types.append("tourism");
  types.append("amenity");
  types.append("building");

  for (int i = 0; i < types.length(); i++)
  {
    const QString type = types.at(i);
    /*if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
    {
      LOG_VARD(type);
    }*/

    if (e1->getTags().contains(type) && e2->getTags().contains(type))
    {
      const double ancestorDistance = _getTagDistance("ancestor", type, map, e1, e2);
      //_ancestorDistance = ancestorDistance;
      /*if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
      {
        LOG_VARD(ancestorDistance);
      }*/

      if (ancestorDistance == 0.0)
      {
        //_ancestorTypeMatch = true;
        /*if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
        {
          LOG_VARD(_ancestorTypeMatch);
        }*/

        return true;
      }
    }
  }

  return false;
}

set< pair<ElementId, ElementId> > PoiPolygonMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_poiEid, _polyEid));
  return result;
}

bool PoiPolygonMatch::isBuildingIsh(ConstElementPtr e)
{
  return OsmSchema::getInstance().isArea(e->getTags(), e->getElementType()) &&
    OsmSchema::getInstance().getCategories(e->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
}

bool PoiPolygonMatch::isPoiIsh(ConstElementPtr e)
{
  return e->getElementType() == ElementType::Node &&
    (OsmSchema::getInstance().getCategories(e->getTags()).intersects(
        OsmSchemaCategory::building() | OsmSchemaCategory::poi()) ||
     e->getTags().getNames().size() > 0);
}

double PoiPolygonMatch::_getTagDistance(const QString type, const QString kvp, ConstOsmMapPtr map,
                                       ConstElementPtr e1, ConstElementPtr e2)
{
  shared_ptr<TagFilteredDifferencer> differencer;
  if (type == "ancestor")
  {
    if (!_tagAncestorDifferencers.contains(kvp))
    {
      differencer.reset(new TagAncestorDifferencer(kvp));
      _tagAncestorDifferencers[kvp] = dynamic_pointer_cast<TagAncestorDifferencer>(differencer);
    }
    else
    {
      differencer = _tagAncestorDifferencers[kvp];
    }
  }
  else
  {
    throw HootException();
  }
  return differencer->diff(map, e1, e2);
}

double PoiPolygonMatch::_getTagDistance(const QString kvp, ConstElementPtr e1,
                                        ConstElementPtr e2) const
{
  double result = 1.0;

  const QStringList relatedTags1 = _getRelatedTags(kvp, e1->getTags());
  const QStringList relatedTags2 = _getRelatedTags(kvp, e2->getTags());
  if (relatedTags1.size() == 0 || relatedTags2.size() == 0)
  {
    return FeatureExtractor::nullValue();
  }

  for (int i = 0; i < relatedTags1.length(); i++)
  {
    for (int j = 0; j < relatedTags2.length(); j++)
    {
      result =
        min(1.0 - OsmSchema::getInstance().score(relatedTags1.at(i), relatedTags2.at(j)), result);
    }
  }

  return result;
}

QStringList PoiPolygonMatch::_getRelatedTags(const QString relateToKvp, const Tags& tags) const
{
  QStringList result;
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
  {
    QString kvp = it.key() + "=" + it.value();
    if (kvp != "poi=yes" && kvp != "place=locality" && kvp != "building=yes")
    {
      if (OsmSchema::getInstance().score(relateToKvp, kvp) > 0)
      {
        result.append(kvp);
      }
    }
  }
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
  str += "type match attribute key: " + _typeMatchAttributeKey + "\n";
  str += "type match attribute value: " + _typeMatchAttributeValue + "\n";
  str += "ancestor type match: " + QString::number(_ancestorTypeMatch) + "\n";
  str += "ancestor distance score: " + QString::number(_ancestorDistance) + "\n";
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

