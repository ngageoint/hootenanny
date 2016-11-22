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
#include "PoiPolygonTypeScoreExtractor.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

#include "../PoiPolygonDistanceTruthRecorder.h"

// Qt
#include <QSet>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonTypeScoreExtractor)

QString PoiPolygonTypeScoreExtractor::poiBestKvp;
QString PoiPolygonTypeScoreExtractor::polyBestKvp;
QSet<QString> PoiPolygonTypeScoreExtractor::_allTagKeys;

PoiPolygonTypeScoreExtractor::PoiPolygonTypeScoreExtractor()
{
}

void PoiPolygonTypeScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setTypeScoreThreshold(config.getPoiPolygonTypeScoreThreshold());
}

bool PoiPolygonTypeScoreExtractor::_failsCuisineMatch(const Tags& t1, const Tags& t2) const
{
  //be a little more restrictive with restaurants
  if (t1.get("amenity").toLower() == "restaurant" &&
      t2.get("amenity").toLower() == "restaurant" &&
      t1.contains("cuisine") && t2.contains("cuisine"))
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
  if (t1.get("leisure").toLower() == "pitch" &&
      t2.get("leisure").toLower() == "pitch" &&
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

double PoiPolygonTypeScoreExtractor::extract(const OsmMap& /*map*/,
                                             const ConstElementPtr& poi,
                                             const ConstElementPtr& poly) const
{
  const Tags& t1 = poi->getTags();
  const Tags& t2 = poly->getTags();

  if (_failsCuisineMatch(t1, t2) || _failsSportMatch(t1, t2))
  {
    return 0.0;
  }

  const double typeScore = _getTagScore(poi, poly);
  LOG_VART(typeScore);
  return typeScore;
}

double PoiPolygonTypeScoreExtractor::_getTagScore(ConstElementPtr poi,
                                                  ConstElementPtr poly) const
{
  double result = 0.0;

  const QStringList poiTagList = _getRelatedTags(poi->getTags());
  const QStringList polyTagList = _getRelatedTags(poly->getTags());

  QStringList excludeKvps;
  excludeKvps.append("building=yes");
  excludeKvps.append("poi=yes");

  for (int i = 0; i < poiTagList.size(); i++)
  {
    for (int j = 0; j < polyTagList.size(); j++)
    {
      const QString poiKvp = poiTagList.at(i).toLower();
      const QString polyKvp = polyTagList.at(j).toLower();
      const double score = OsmSchema::getInstance().score(poiKvp, polyKvp);
      if (score >= result)
      {
        if (!poiKvp.isEmpty() && !excludeKvps.contains(poiKvp))
        {
          poiBestKvp = poiKvp;
        }
        if (!polyKvp.isEmpty() && !excludeKvps.contains(polyKvp))
        {
          polyBestKvp = polyKvp;
        }
      }
      result = max(score, result);

      LOG_VART(poiKvp);
      LOG_VART(polyKvp);
      LOG_VART(result);

      if (result == 1.0)
      {
        if (ConfigOptions().getPoiPolygonPrintMatchDistanceTruth())
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

  if (ConfigOptions().getPoiPolygonPrintMatchDistanceTruth())
  {
    PoiPolygonDistanceTruthRecorder::recordDistanceTruth(
      poi, poly, poiBestKvp, polyBestKvp, _featureDistance);
  }

  return result;
}

QStringList PoiPolygonTypeScoreExtractor::_getRelatedTags(const Tags& tags) const
{
  QStringList tagsList;

  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
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

//bool PoiPolygonTypeScoreExtractor::isRecCenter(/*ConstElementPtr element*/const QString elementName)
//{
//  //const QString elementName =
//    //Translator::getInstance().toEnglish(element->getTags().get("name").toLower());
//  return elementName.contains("recreation center") || elementName.contains("rec center") ||
//    elementName.contains("rec ctr") || elementName.contains("clubhouse") ||
//    elementName.contains("fieldhouse");
//}

bool PoiPolygonTypeScoreExtractor::isPark(ConstElementPtr element)
{
  return !OsmSchema::getInstance().isBuilding(element) &&
         (element->getTags().get("leisure") == "park");
}

//bool PoiPolygonTypeScoreExtractor::isBuildingIsh(ConstElementPtr element, const QString elementName)
//{
//  //const QString elementName =
//    //Translator::getInstance().toEnglish(element->getTags().get("name").toLower());
//  return OsmSchema::getInstance().isBuilding(element) || elementName.contains("building") ||
//    elementName.contains("bldg");
//}

bool PoiPolygonTypeScoreExtractor::isParkish(ConstElementPtr element)
{
  if (OsmSchema::getInstance().isBuilding(element))
  {
    return false;
  }
  const QString leisureVal = element->getTags().get("leisure").toLower();
  return leisureVal == "garden" || leisureVal == "dog_park";
}

//bool PoiPolygonTypeScoreExtractor::isPlayArea(/*ConstElementPtr element*/const QString elementName)
//{
//  //const QString elementName =
//    //Translator::getInstance().toEnglish(element->getTags().get("name").toLower());
//  return elementName.contains("play area");
//}

bool PoiPolygonTypeScoreExtractor::isPlayground(ConstElementPtr element)
{
  const Tags& tags = element->getTags();
  //const QString elementName = Translator::getInstance().toEnglish(tags.get("name").toLower());
  return tags.get("leisure") == "playground" /*|| elementName.contains("playground")*/;
}

bool PoiPolygonTypeScoreExtractor::isSport(ConstElementPtr element)
{
  return element->getTags().contains("sport");
}

bool PoiPolygonTypeScoreExtractor::isSchool(ConstElementPtr element)
{
  return element->getTags().get("amenity").toLower() == "school";
}

bool PoiPolygonTypeScoreExtractor::isRestroom(ConstElementPtr element)
{
  return element->getTags().get("amenity").toLower() == "toilets";
}

bool PoiPolygonTypeScoreExtractor::isParking(ConstElementPtr element)
{
  return element->getTags().get("amenity") == "parking";
}

bool PoiPolygonTypeScoreExtractor::hasMoreThanOneType(ConstElementPtr element)
{
  int typeCount = 0;
  QStringList typesParsed;
  if (_allTagKeys.size() == 0)
  {
    _allTagKeys = OsmSchema::getInstance().getAllTagKeys();
    _allTagKeys.remove("REF1");
    _allTagKeys.remove("REF2");
    _allTagKeys.remove("uuid");
    _allTagKeys.remove("name");
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

}
