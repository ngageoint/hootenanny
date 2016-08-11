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
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/rnd/conflate/poi-polygon/extractors/PoiPolygonNameExtractor.h>
#include <hoot/core/algorithms/string/MinSumWordSetDistance.h>
#include <hoot/core/algorithms/string/WeightedWordDistance.h>
//#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/algorithms/string/SqliteWordWeightDictionary.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/util/ConfPath.h>

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

QString PoiPolygonMatch::_testUuid = "{08cf2389-216b-5a49-afcd-5ce30cef9436}";

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold) :
Match(threshold),
_ancestorTypeMatch(false),
_ancestorDistance(-1.0),
_addressMatch(false),
_exactNameMatch(false)
{
  ConstElementPtr e1 = map->getElement(eid1);
  ConstElementPtr e2 = map->getElement(eid2);

  if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    LOG_DEBUG("Conflating:" << _testUuid);
  }

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

  bool typeMatch = _calculateTypeMatch(poi, poly);
  bool ancestorTypeMatch = false;
  if (ConfigOptions().getPoiPolygonUseTagAncestorTypeMatching())
  {
    ancestorTypeMatch = _calculateAncestorTypeMatch(map, poi, poly);
  }
  //bool buildingCategoryTypeMatch = _getTagDistance("category", "building", map, e1, e2) == 0;
  //LOG_VARD(buildingCategoryTypeMatch);
  //bool poiCategoryTypeMatch = _getTagDistance("category", "poi", map, e1, e2) == 0;
  //LOG_VARD(poiCategoryTypeMatch);
  //if (poiCategoryTypeMatch)
  //{
    //typeMatch = true;
  //}

  double nameScore = _calculateNameScore(poi, poly);
  bool nameMatch = nameScore >= ConfigOptions().getPoiPolygonMatchNameThreshold();
  bool exactNameMatch = false;
  //poi.polygon.promote.exact.name.matches
  if (ConfigOptions().getPoiPolygonPromoteExactNameMatches())
  {
    exactNameMatch = nameScore == 1.0;
    _exactNameMatch = exactNameMatch;
  }

  bool addressMatch = false;
  if (ConfigOptions().getPoiPolygonUseAddressNameMatching())
  {
    addressMatch = _getAddressMatch(e1, e2);
    _addressMatch = addressMatch;
  }

  double distance = gpoly->distance(gpoi.get());

  // calculate the 2 sigma for the distance between the two objects
  double sigma1 = e1->getCircularError() / 2.0;
  double sigma2 = e1->getCircularError() / 2.0;
  double ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;

  double matchDistance = -1.0;
  if (!ConfigOptions().getPoiPolygonUseTypeCustomizedMatchDistance())
  {
    matchDistance = ConfigOptions().getPoiPolygonMatchDistance();
  }
  else
  {
    if (_oneGeneric(e1, e2) && _ancestorDistance == -1.0)
    {
      matchDistance = max(_getMatchDistance(e1), _getMatchDistance(e2));
    }
    else
    {
      matchDistance = min(_getMatchDistance(e1), _getMatchDistance(e2));
    }
  }

  double reviewDistance = -1.0;
  if (ConfigOptions().getPoiPolygonUseTypeCustomizedReviewDistance())
  {
    if (_oneGeneric(e1, e2) && _ancestorDistance == -1.0)
    {
      reviewDistance = max(_getReviewDistance(e1), _getReviewDistance(e2));
    }
    else
    {
      reviewDistance = min(_getReviewDistance(e1), _getReviewDistance(e2));
    }
  }
  else
  {
    reviewDistance = ConfigOptions().getPoiPolygonMatchReviewDistance();
  }

  if (ConfigOptions().getPoiPolygonAddCircularErrorToReviewDistance())
  {
    reviewDistance += ce;
  }

  //custom rule
  if (ConfigOptions().getPoiPolygonUseCustomTowerRules() &&
      (e1->getTags().get("man_made") == "tower" || e2->getTags().get("man_made") == "tower"))
  {
    matchDistance = 0.0;
    reviewDistance = 0.0;
  }

  /*if (ancestorTypeMatch && !typeMatch && !nameMatch)
  {
    reviewDistance *= ConfigOptions().getPoiPolygonAncestorOnlyReviewPenalty();
    //matchDistance = 0.0;
  }*/

  bool closeMatch = distance <= reviewDistance;

  int evidence = 0;
  evidence += typeMatch ? 1 : 0;
  evidence += ancestorTypeMatch ? 1 : 0;
  evidence += nameMatch ? 1 : 0;
  evidence += exactNameMatch ? 1 : 0;
  if (ConfigOptions().getPoiPolygonAddressOnlyEvidenceBoost() != 0 && !typeMatch && !nameMatch)
  {
    evidence += addressMatch ? ConfigOptions().getPoiPolygonAddressOnlyEvidenceBoost() : 0;
  }
  else
  {
    evidence += addressMatch ? 1 : 0;
  }
  evidence += distance <= matchDistance ? 2 : 0;

  //custom rule
  if (ConfigOptions().getPoiPolygonUseCustomTowerRules() &&
      (((e1->getTags().get("building") == "terminal" &&
        e2->getTags().get("man_made") == "control_tower") ||
      (e2->getTags().get("building") == "terminal" &&
       e1->getTags().get("man_made") == "control_tower")) && closeMatch))
  {
    evidence += 1;
  }

  if (!closeMatch)
  {
    _c.setMiss();
    //LOG_DEBUG("poipoly miss");
  }
  else if (evidence >= 3)
  {
    _c.setMatch();
    //LOG_DEBUG("poipoly match");
  }
  else if (evidence >= 1)
  {
    _c.setReview();
    //LOG_DEBUG("poipoly review");
  }
  else
  {
    _c.setMiss();
    //LOG_DEBUG("poipoly miss");
  }

  _uuid1 = e1->getTags().get("uuid");
  _uuid2 = e2->getTags().get("uuid");
  _typeMatch = typeMatch;
  _nameMatch = nameMatch;
  _nameScore = nameScore;
  QStringList names1 = e1->getTags().getNames();
  names1.append(e1->getTags().getPseudoNames());
  _names1 = names1.join(",");
  QStringList names2 = e2->getTags().getNames();
  names2.append(e2->getTags().getPseudoNames());
  _names2 = names2.join(",");
  //_addressMatch = addressMatch;
  //_addrTag1 = e1AddrTag;
  //_combAddr1 = e1AddrComb;
  //_addrTag2 = e2AddrTag;
  //_combAddr2 = e2AddrComb;
  _closeMatch = closeMatch;
  _distance = distance;
  _reviewDistance = reviewDistance;
  _ce = ce;
  _circularError1 = e1->getCircularError();
  _circularError2 = e2->getCircularError();
  _evidence = evidence;

  if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    LOG_VARD(eid1);\
    LOG_VARD(e1->getTags().get("uuid"));
    LOG_VARD(e1->getTags());
    LOG_VARD(eid2);
    LOG_VARD(e2->getTags().get("uuid"));
    LOG_VARD(e2->getTags());
    LOG_VARD(typeMatch);
    LOG_VARD(_typeMatchAttributeKey);
    LOG_VARD(_typeMatchAttributeValue);
    LOG_VARD(ancestorTypeMatch);
    LOG_VARD(_ancestorDistance);
    LOG_VARD(nameMatch);
    LOG_VARD(nameScore);
    LOG_VARD(names1);
    LOG_VARD(names2);
    LOG_VARD(addressMatch);
    LOG_VARD(closeMatch);
    LOG_VARD(distance);
    LOG_VARD(reviewDistance);
    LOG_VARD(ce);
    LOG_VARD(e1->getCircularError());
    LOG_VARD(e2->getCircularError());
    LOG_VARD(evidence);
    LOG_DEBUG("**************************");
  }
}

double PoiPolygonMatch::_calculateNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  if (ConfigOptions().getPoiPolygonUseWeightedWordDistance())
  {
    SqliteWordWeightDictionary* dict =
      new SqliteWordWeightDictionary(
        ConfPath::search(ConfigOptions().getWeightedWordDistanceDictionary()));
    return
      PoiPolygonNameExtractor(
        new TranslateStringDistance(
          new WeightedWordDistance(
            new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()), dict)))
        .extract(e1, e2);
  }
  else
  {
    if (ConfigOptions().getPoiPolygonUseMeanWordDistanceNameComparison())
    {
      return
        PoiPolygonNameExtractor(
          new TranslateStringDistance(
            new MeanWordSetDistance(
              new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))))
          .extract(e1, e2);
    }
    else
    {
      return
        PoiPolygonNameExtractor(
          new TranslateStringDistance(
            new MinSumWordSetDistance(
              new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))))
          .extract(e1, e2);
    }
  }
}

bool PoiPolygonMatch::_calculateTypeMatch(ConstElementPtr e1, ConstElementPtr e2) //const
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();

  if (ConfigOptions().getPoiPolygonUseCustomRestaurantRules())
  {
    if (t1.get("amenity").toLower() == "restaurant" &&
        t2.get("amenity").toLower() == "restaurant")
    {
      if (t1.contains("cuisine") && t2.contains("cuisine"))
      {
        if (t1.get("cuisine").toLower() == "german" && t2.get("cuisine").toLower() == "bavarian")
        {
          return true;
        }
        else if (t2.get("cuisine").toLower() == "german" && t1.get("cuisine").toLower() == "bavarian")
        {
          return true;
        }
        else if (t1.get("cuisine").toLower() != t2.get("cuisine").toLower())
        {
          return false;
        }
      }
    }
  }

  for (Tags::const_iterator it = t1.begin(); it != t1.end(); it++)
  {
    // if it is a use or POI category
    if ((OsmSchema::getInstance().getCategories(it.key(), it.value()) &
         (OsmSchemaCategory::building() | OsmSchemaCategory::use() | OsmSchemaCategory::poi()))
          != OsmSchemaCategory::Empty)
    {
      bool result = t2.get(it.key()) == it.value();
      if (result &&
          ((it.key().toLower() != "building" && it.value().toLower() != "yes") ||
            ConfigOptions().getPoiPolygonAllowGenericBuildingMatches()))
      {
        _typeMatchAttributeKey = it.key();
        _typeMatchAttributeValue = it.value();

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
         t1.get("shop").toLower() == "car_repair")) //very questionable
    {
      return true;
    }
    //TODO: haven't tested this yet
    if (t1.get("shop").toLower().contains("car") &&
        t2.get("shop").toLower().contains("car")) //very questionable
    {
      return true;
    }
    if ((t1.get("leisure").toLower() == "sports_centre" &&
         t2.get("leisure").toLower() == "water_park") ||
        (t2.get("leisure").toLower() == "sports_centre" &&
         t1.get("leisure").toLower() == "water_park")) //very questionable
    {
      return true;
    }
    if ((t1.get("leisure").toLower() == "sports_centre" &&
         t2.get("leisure").toLower() == "swimming_pool") ||
        (t2.get("leisure").toLower() == "sports_centre" &&
         t1.get("leisure").toLower() == "swimming_pool")) //very questionable
    {
      return true;
    }
    if ((t1.get("leisure").toLower() == "sports_centre" &&
         t2.get("sport").toLower() == "swimming") ||
        (t2.get("leisure").toLower() == "sports_centre" &&
         t1.get("sport").toLower() == "swimming")) //very questionable
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
    if ((t1.get("amenity").toLower() == "arts_centre" &&
         t2.get("amenity").toLower() == "theatre") ||
        (t2.get("amenity").toLower() == "arts_centre" &&
         t1.get("amenity").toLower() == "theatre"))
    {
      return true;
    }
  }

  return false;
}

bool PoiPolygonMatch::_calculateAncestorTypeMatch(const ConstOsmMapPtr& map, ConstElementPtr e1,
                                                  ConstElementPtr e2) //const
{
  QStringList types;

  types.append("tourism");
  types.append("amenity");

  //TODO: Remove these if they don't yield any improvements after testing all of dataset D?
  types.append("leisure");
  types.append("historic");
  types.append("landuse");
  types.append("man_made");
  types.append("natural");
  types.append("place");
  types.append("power");
  types.append("railway");
  types.append("shop");
  types.append("sport");
  types.append("station");
  types.append("transport");
  types.append("barrier");
  types.append("use");
  types.append("industrial");

  types.append("building");

  bool hasMatchingCategory = false;
  for (int i = 0; i < types.length(); i++)
  {
    const QString type = types.at(i);
    if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
    {
      LOG_VARD(type);
    }

    if (!ConfigOptions().getPoiPolygonAllowGenericBuildingMatches() && type == "building" &&
        !hasMatchingCategory && _oneGeneric(e1, e2))
    {
      if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
      {
        LOG_DEBUG("generic building only");
      }
    }
    else if (e1->getTags().contains(type) && e2->getTags().contains(type))
    {
      hasMatchingCategory = true;
      const double ancestorDistance = _getTagDistance("ancestor", type, map, e1, e2);
      _ancestorDistance = ancestorDistance;
      if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
      {
        LOG_VARD(ancestorDistance);
      }
      if (ancestorDistance == 0.0)
      {
        _ancestorTypeMatch = true;
        if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
        {
          LOG_VARD(_ancestorTypeMatch);
        }
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
  else if (type == "category")
  {
    if (!_tagCategoryDifferencers.contains(kvp))
    {
      differencer.reset(new TagCategoryDifferencer(OsmSchemaCategory::fromString(kvp)));
      _tagCategoryDifferencers[kvp] = dynamic_pointer_cast<TagCategoryDifferencer>(differencer);
    }
    else
    {
      differencer = _tagCategoryDifferencers[kvp];
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

QStringList PoiPolygonMatch::_getTagsByCategory(const QString category, const Tags& tags) const
{
  QStringList result;
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
  {
    QString kvp = it.key() + "=" + it.value();
    if (kvp != "poi=yes" && kvp != "place=locality" && kvp != "building=yes")
    {
      if (OsmSchema::getInstance().getCategories(kvp).toStringList().indexOf(category) >= 0)
      {
        result.append(kvp);
      }
    }
  }
  return result;
}

bool PoiPolygonMatch::_oneGeneric(ConstElementPtr e1, ConstElementPtr e2) const
{
  if (isPoiIsh(e1) && isBuildingIsh(e2))
  {
    return _getTagsByCategory("poi", e1->getTags()).length() == 0 ||
           _getTagsByCategory("building", e2->getTags()).length() == 0;
  }
  else if (isPoiIsh(e2) && isBuildingIsh(e1))
  {
    return _getTagsByCategory("building", e1->getTags()).length() == 0 ||
           _getTagsByCategory("poi", e2->getTags()).length() == 0;
  }
  else
  {
    throw HootException();
  }
}

double PoiPolygonMatch::_getMatchDistance(ConstElementPtr element)
{
  Tags tags = element->getTags();
  if (tags.get("building") == "school")
  {
    return 10.0;
  }
  else if (tags.get("industrial") == "manufacturing")
  {
    return 20.0;
  }
  else if (tags.get("amenity") == "hospital")
  {
    return 7.0;
  }
  else if (tags.get("amenity") == "prison")
  {
    return 21.0;
  }
  else if (tags.get("amenity") == "courthouse" || tags.get("amenity") == "judicial_activities")
  {
    return 8.0;
  }
  else
  {
    return ConfigOptions().getPoiPolygonMatchDistance();
  }
}

double PoiPolygonMatch::_getReviewDistance(ConstElementPtr element)
{
  Tags tags = element->getTags();
  if (tags.get("amenity") == "prison")
  {
    return 27.0;
  }
  else if (tags.get("amenity") == "place_of_worship")
  {
    return 58.0;
  }
  else if (tags.get("amenity") == "government_administration")
  {
    return 10.0;
  }
  else if (tags.get("building") == "terminal")
  {
    return 32.0;
  }
  else if (tags.get("man_made") == "storage_tank")
  {
    return 8.0;
  }
  else if (tags.get("amenity") == "school")
  {
    return 16.0;
  }
  else if (tags.get("amenity") == "toilets")
  {
    return 12.0;
  }
  else if (tags.get("amenity") == "police")
  {
    return 19.0;
  }
  //TODO: verify this one is actually doing anything
  else if (tags.get("shop") == "department_store")
  {
    return 40.0;
  }
  else if (tags.get("building") == "apartments")
  {
    return 84.0;
  }
  else if (tags.get("amenity") == "library")
  {
    return 31.0;
  }
  else if (tags.get("building") == "station")
  {
    return 31.0;
  }
  else if (tags.get("tourism") == "attraction")
  {
    return 51.0;
  }
  else if (tags.get("amenity") == "restaurant")
  {
    return 8.0;
  }
  else
  {
    return ConfigOptions().getPoiPolygonMatchReviewDistance();
  }
}

bool PoiPolygonMatch::_getAddressMatch(ConstElementPtr e1, ConstElementPtr e2)
{
  Tags e1Tags = e1->getTags();
  Tags e2Tags = e2->getTags();

  //TODO: hack - this should be able to be eliminated by using the translated name comparison
  //logic
  QChar eszett(0x00DF);

  const QString e1HouseNum = e1Tags.get("addr:housenumber").trimmed();
  QString e1Street = e1Tags.get("addr:street").trimmed().toLower();
  if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  {
    e1Street = e1Street.replace(eszett, "ss");
  }
  QString e1AddrComb;
  if (!e1HouseNum.isEmpty() && !e1Street.isEmpty())
  {
    e1AddrComb = e1HouseNum + " " + e1Street;
  }
  QString e1AddrTag = e1Tags.get("address").trimmed().toLower();
  if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  {
    e1AddrTag = e1AddrTag.replace(eszett, "ss");
  }
  const QString e2HouseNum = e2Tags.get("addr:housenumber").trimmed();
  QString e2Street = e2Tags.get("addr:street").trimmed().toLower();
  if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  {
    e2Street = e2Street.replace(eszett, "ss");
  }
  QString e2AddrComb;
  if (!e2HouseNum.isEmpty() && !e2Street.isEmpty())
  {
    e2AddrComb = e2HouseNum + " " + e2Street;
  }
  QString e2AddrTag = e2Tags.get("address").trimmed().toLower();
  if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  {
    e2AddrTag = e2AddrTag.replace(eszett, "ss");
  }

  if (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid)
  {
    LOG_VARD(e1AddrComb);
    LOG_VARD(e2AddrComb);
    LOG_VARD(e1AddrTag);
    LOG_VARD(e2AddrTag);
  }

  ExactStringDistance addrComp;
  const double addrMatchThresh = 0.99;
  if (!e1AddrComb.isEmpty() && !e2AddrComb.isEmpty() && e1HouseNum == e2HouseNum &&
        addrComp.compare(e1AddrComb, e2AddrComb) >= addrMatchThresh)
  {
    return true;
  }
  else if (!e1AddrComb.isEmpty() && !e2AddrTag.isEmpty() && e2AddrTag.startsWith(e1HouseNum) &&
           addrComp.compare(e1AddrComb, e2AddrTag) >= addrMatchThresh)
  {
    return true;
  }
  else if (!e2AddrComb.isEmpty() && !e1AddrTag.isEmpty() && e1AddrTag.startsWith(e2HouseNum) &&
           addrComp.compare(e2AddrComb, e1AddrTag) >= addrMatchThresh)
  {
    return true;
  }
  else if (!e1AddrTag.isEmpty() && !e2AddrTag.isEmpty() && //e1HouseNum == e2HouseNum &&
           addrComp.compare(e1AddrTag, e2AddrTag) >= addrMatchThresh)
  {
    return true;
  }
  return false;
}

QString PoiPolygonMatch::toString() const
{
  //return QString("PoiPolygonMatch %1 %2 P: %3").arg(_poiEid.toString()).
    //arg(_polyEid.toString()).arg(_c.toString());

  QString str =
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
  str += "exact name match: " + QString::number(_exactNameMatch) + "\n";
  str += "name score: " + QString::number(_nameScore) + "\n";
  str += "names 1: " + _names1 + "\n";
  str += "names 2: " + _names2 + "\n";
  str += "address match: " + QString::number(_addressMatch) + "\n";
  str += "close match: " + QString::number(_closeMatch) + "\n";
  str += "distance: " + QString::number(_distance) + "\n";
  str += "review distance: " + QString::number(_reviewDistance) + "\n";
  str += "overall circular error: " + QString::number(_ce) + "\n";
  str += "circular error 1: " + QString::number(_circularError1) + "\n";
  str += "circular error 2: " + QString::number(_circularError2) + "\n";
  str += "evidence: " + QString::number(_evidence);
  return str;
}

}

