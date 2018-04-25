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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonTypeScoreExtractor.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>

#include "PoiPolygonNameScoreExtractor.h"
#include "../PoiPolygonDistanceTruthRecorder.h"

// Qt
#include <QSet>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonTypeScoreExtractor)

QString PoiPolygonTypeScoreExtractor::poiBestKvp;
QString PoiPolygonTypeScoreExtractor::polyBestKvp;
QSet<QString> PoiPolygonTypeScoreExtractor::_allTagKeys;
QStringList PoiPolygonTypeScoreExtractor::failedMatchRequirements;

PoiPolygonTypeScoreExtractor::PoiPolygonTypeScoreExtractor()
{
}

void PoiPolygonTypeScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setTypeScoreThreshold(config.getPoiPolygonTypeScoreThreshold());
  setPrintMatchDistanceTruth(config.getPoiPolygonPrintMatchDistanceTruth());
}

double PoiPolygonTypeScoreExtractor::extract(const OsmMap& /*map*/,
                                             const ConstElementPtr& poi,
                                             const ConstElementPtr& poly) const
{
  failedMatchRequirements.clear();

  const Tags& t1 = poi->getTags();
  const Tags& t2 = poly->getTags();

  //TODO: make this failing match technique more extensible

  if (_failsCuisineMatch(t1, t2))
  {
    if (!failedMatchRequirements.contains("cuisine"))
    {
      failedMatchRequirements.append("cusine");
    }
    return 0.0;
  }
  else if (_failsSportMatch(t1, t2))
  {
    if (!failedMatchRequirements.contains("sport"))
    {
      failedMatchRequirements.append("sport");
    }
    return 0.0;
  }
  else if (_failsReligionMatch(t1, t2))
  {
    if (!failedMatchRequirements.contains("religion"))
    {
      failedMatchRequirements.append("religion");
    }
    return 0.0;
  }

  double typeScore = _getTagScore(poi, poly);
  if (typeScore < 0.001)
  {
    typeScore = 0.0;
  }
  LOG_VART(typeScore);
  return typeScore;
}

double PoiPolygonTypeScoreExtractor::_getTagScore(ConstElementPtr poi,
                                                  ConstElementPtr poly) const
{
  double result = 0.0;

  QStringList poiTagList = _getRelatedTags(poi->getTags());
  QStringList polyTagList = _getRelatedTags(poly->getTags());
  LOG_VART(poiTagList);
  LOG_VART(polyTagList);

  //If a feature has a specific type, we only want to look at that type and ignore any generic
  //types.  Otherwise, we'll allow a type match with just a generic tag.
  QStringList excludeKvps;
  const bool poiIsGenericPoi = poiTagList.size() == 1 && poiTagList.contains("poi=yes");
  const bool poiIsGenericBuilding = poiTagList.size() == 1 && poiTagList.contains("building=yes");
  const bool polyIsGenericPoi = polyTagList.size() == 1 && polyTagList.contains("poi=yes");
  const bool polyIsGenericBuilding = polyTagList.size() == 1 && polyTagList.contains("building=yes");
  if (!poiIsGenericPoi && !polyIsGenericPoi)
  {
    excludeKvps.append("poi=yes");
  }
  if (!poiIsGenericBuilding && !polyIsGenericBuilding)
  {
    excludeKvps.append("building=yes");
  }
  LOG_VART(poiIsGenericPoi);
  LOG_VART(poiIsGenericBuilding);
  LOG_VART(polyIsGenericPoi);
  LOG_VART(polyIsGenericBuilding);

  LOG_VART(excludeKvps);
  for (int i = 0; i < excludeKvps.size(); i++)
  {
    const QString excludeKvp = excludeKvps.at(i);
    poiTagList.removeAll(excludeKvp);
    polyTagList.removeAll(excludeKvp);
  }

  for (int i = 0; i < poiTagList.size(); i++)
  {
    for (int j = 0; j < polyTagList.size(); j++)
    {
      const QString poiKvp = poiTagList.at(i).toLower();
      const QString polyKvp = polyTagList.at(j).toLower();
      LOG_VART(poiKvp);
      LOG_VART(polyKvp);

      const double score = OsmSchema::getInstance().score(poiKvp, polyKvp);
      LOG_VART(score);
      if (score >= result)
      {
        if (!poiKvp.isEmpty() && !excludeKvps.contains(poiKvp))
        {
          poiBestKvp = poiKvp;
          LOG_VART(poiBestKvp);
        }
        if (!polyKvp.isEmpty() && !excludeKvps.contains(polyKvp))
        {
          polyBestKvp = polyKvp;
          LOG_VART(polyBestKvp);
        } 
      }
      result = max(score, result);
      LOG_VART(result);

      if (result == 1.0)
      {
        if (_printMatchDistanceTruth)
        {
          LOG_VART(poiBestKvp);
          LOG_VART(polyBestKvp);
          PoiPolygonDistanceTruthRecorder::recordDistanceTruth(
            poi, poly, poiBestKvp, polyBestKvp, _featureDistance);
        }
        return result;
      }
    }
  }
  LOG_VART(poiBestKvp);
  LOG_VART(polyBestKvp);

  if (_printMatchDistanceTruth)
  {
    PoiPolygonDistanceTruthRecorder::recordDistanceTruth(
      poi, poly, poiBestKvp, polyBestKvp, _featureDistance);
  }

  return result;
}

QStringList PoiPolygonTypeScoreExtractor::_getRelatedTags(const Tags& tags) const
{
  QStringList tagsList;
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
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

bool PoiPolygonTypeScoreExtractor::isSchool(ConstElementPtr element)
{
  const QString amenityStr = element->getTags().get("amenity").toLower();
  return amenityStr == "school" || amenityStr == "university";
}

//TODO: this specific school logic should be handled in the schema instead

bool PoiPolygonTypeScoreExtractor::isSpecificSchool(ConstElementPtr element)
{
  const QString name = PoiPolygonNameScoreExtractor::getElementName(element).toLower();
  return
    isSchool(element) &&
    //TODO: these endsWiths can maybe be contains instead
    (name.toLower().endsWith("high school") || name.toLower().endsWith("middle school") ||
     name.toLower().endsWith("elementary school") ||
     name.toLower().contains("college") || name.toLower().contains("university") );
}

bool PoiPolygonTypeScoreExtractor::specificSchoolMatch(ConstElementPtr element1,
                                                       ConstElementPtr element2)
{
  if (isSpecificSchool(element1) && isSpecificSchool(element2))
  {
    const QString name1 = PoiPolygonNameScoreExtractor::getElementName(element1).toLower();
    const QString name2 = PoiPolygonNameScoreExtractor::getElementName(element2).toLower();
    if ((name1.endsWith("high school") && name2.endsWith("high school")) ||
        (name1.endsWith("middle school") && name2.endsWith("middle school")) ||
        (name1.endsWith("elementary school") && name2.endsWith("elementary school")) ||
        (name1.contains("college") && name2.contains("college")) ||
        (name1.contains("college") && name2.contains("university")) ||
        (name1.contains("university") && name2.contains("college")) ||
        (name1.contains("university") && name2.contains("university")))
    {
      return true;
    }
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::isPark(ConstElementPtr element)
{
  return !OsmSchema::getInstance().isBuilding(element) &&
         (element->getTags().get("leisure") == "park");
}

bool PoiPolygonTypeScoreExtractor::isParkish(ConstElementPtr element)
{
  if (OsmSchema::getInstance().isBuilding(element))
  {
    return false;
  }
  const QString leisureVal = element->getTags().get("leisure").toLower();
  return leisureVal == "garden" || leisureVal == "dog_park";
}

bool PoiPolygonTypeScoreExtractor::isPlayground(ConstElementPtr element)
{
  return element->getTags().get("leisure") == "playground";
}

bool PoiPolygonTypeScoreExtractor::isSport(ConstElementPtr element)
{
  const Tags& tags = element->getTags();
  return tags.contains("sport") || tags.get("leisure").contains("sport");
}

bool PoiPolygonTypeScoreExtractor::isRestroom(ConstElementPtr element)
{
  return element->getTags().get("amenity").toLower() == "toilets";
}

bool PoiPolygonTypeScoreExtractor::isParking(ConstElementPtr element)
{
  const Tags& tags = element->getTags();
  return
    tags.get("amenity") == "parking" || tags.contains("parking") ||
    tags.get("amenity") == "bicycle_parking";
}

bool PoiPolygonTypeScoreExtractor::isReligion(ConstElementPtr element)
{
  return isReligion(element->getTags());
}

bool PoiPolygonTypeScoreExtractor::isReligion(const Tags& tags)
{
  return tags.get("amenity").toLower() == "place_of_worship" ||
         tags.get("building").toLower() == "church" ||
         tags.get("building").toLower() == "mosque" ||
         //TODO: this one is an alias of building=mosque, so we should be getting it from there instead
         tags.get("amenity").toLower() == "mosque" ||
         tags.get("building").toLower() == "synagogue";
}

bool PoiPolygonTypeScoreExtractor::hasMoreThanOneType(ConstElementPtr element)
{
  int typeCount = 0;
  QStringList typesParsed;
  if (_allTagKeys.size() == 0)
  {
    QSet<QString> allTagKeysTemp = OsmSchema::getInstance().getAllTagKeys();
    allTagKeysTemp.remove(MetadataTags::Ref1());
    allTagKeysTemp.remove(MetadataTags::Ref2());
    allTagKeysTemp.remove("uuid");
    allTagKeysTemp.remove("name");
    allTagKeysTemp.remove("ele");
    for (QSet<QString>::const_iterator it = allTagKeysTemp.begin(); it != allTagKeysTemp.end(); ++it)
    {
      const QString tagKey = *it;
      //address tags aren't really type tags
      if (!tagKey.startsWith("addr:"))
      {
        _allTagKeys.insert(tagKey);
      }
    }
  }

  const Tags elementTags = element->getTags();
  for (Tags::const_iterator it = elementTags.begin(); it != elementTags.end(); ++it)
  {
    const QString elementTagKey = it.key();
    //there may be duplicate keys in allTags
    if (_allTagKeys.contains(elementTagKey) && !typesParsed.contains(elementTagKey))
    {
      LOG_TRACE("Has key: " << elementTagKey);
      typeCount++;
      if (typeCount > 1)
      {
        return true;
      }
    }

    typesParsed.append(elementTagKey);
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::hasType(ConstElementPtr element)
{
  return
    OsmSchema::getInstance().getCategories(element->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
}

bool PoiPolygonTypeScoreExtractor::hasSpecificType(ConstElementPtr element)
{
  return
    hasType(element) && !element->getTags().contains("poi") &&
          element->getTags().get("building") != QLatin1String("yes") &&
          element->getTags().get("office") != QLatin1String("yes") &&
          element->getTags().get("area") != QLatin1String("yes");
}

bool PoiPolygonTypeScoreExtractor::isRestaurant(ConstElementPtr element)
{
  return isRestaurant(element->getTags());
}

bool PoiPolygonTypeScoreExtractor::isRestaurant(const Tags& tags)
{
  return tags.get("amenity") == "restaurant" || tags.get("amenity") == "fast_food";
}

//TODO: abstract these three type fail methods into one; also, this should be able to be done
//more intelligently using the schema vs custom code

bool PoiPolygonTypeScoreExtractor::_failsCuisineMatch(const Tags& t1, const Tags& t2) const
{
  //be a little more restrictive with restaurants
  if (isRestaurant(t1) && isRestaurant(t2) && t1.contains("cuisine") && t2.contains("cuisine"))
  {
    const QString t1Cuisine = t1.get("cuisine").toLower();
    const QString t2Cuisine = t2.get("cuisine").toLower();
    if (OsmSchema::getInstance().score("cuisine=" + t1Cuisine, "cuisine=" + t2Cuisine) != 1.0 &&
        //Don't return false on regional, since its location dependent and we don't take that into
        //account.
        t1Cuisine != "regional" && t2Cuisine != "regional" &&
        //Don't fail on "other", since that's not very descriptive.
        t1Cuisine != "other" && t2Cuisine != "other")
    {
      LOG_TRACE("Failed type match on different cuisines.");
      return true;
    }
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::_failsSportMatch(const Tags& t1, const Tags& t2) const
{
  //be a little more restrictive with sport areas
  //TODO: the sports center part of this may go away if the 0.8 similarity match between
  //sports_centre and sport=tennis is removed
  //TODO: use isSport here instead
  if ((t1.get("leisure").toLower() == "pitch" || t1.get("leisure").toLower() == "sports_centre") &&
      (t2.get("leisure").toLower() == "pitch" || t2.get("leisure").toLower() == "sports_centre") &&
      t1.contains("sport") && t2.contains("sport"))
  {
    const QString t1Sport = t1.get("sport").toLower();
    const QString t2Sport = t2.get("sport").toLower();
    if (OsmSchema::getInstance().score("sport=" + t1Sport, "sport=" + t2Sport) != 1.0)
    {
      LOG_TRACE("Failed type match on different sports.");
      return true;
    }
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::_failsReligionMatch(const Tags& t1, const Tags& t2) const
{
  //be a little more restrictive with religions
  if (isReligion(t1) && isReligion(t2))
  {
    if (t1.contains("denomination") && t2.contains("denomination"))
    {
      const QString t1Denom = t1.get("denomination").toLower().trimmed();
      const QString t2Denom = t2.get("denomination").toLower().trimmed();
      if (!t1Denom.isEmpty() && !t2Denom.isEmpty() &&
          OsmSchema::getInstance().score("denomination=" + t1Denom, "denomination=" + t2Denom) != 1.0)
      {
        LOG_TRACE("Failed type match on different religious denomination.");
        return true;
      }
    }
    else if (t1.contains("religion") && t2.contains("religion"))
    {
      const QString t1Denom = t1.get("religion").toLower().trimmed();
      const QString t2Denom = t2.get("religion").toLower().trimmed();
      if (!t1Denom.isEmpty() && !t2Denom.isEmpty() &&
          OsmSchema::getInstance().score("religion=" + t1Denom, "religion=" + t2Denom) != 1.0)
      {
        LOG_TRACE("Failed type match on different religions.");
        return true;
      }
    }
  }

  return false;
}

}
