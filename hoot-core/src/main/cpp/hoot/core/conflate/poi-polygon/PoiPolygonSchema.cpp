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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonSchema.h"

// hoot
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

QMultiHash<QString, QString> PoiPolygonSchema::_typeToNames;
QSet<QString> PoiPolygonSchema::_allTypeKeys;

QMultiHash<QString, QString> PoiPolygonSchema::_getTypeToNames()
{
  if (_typeToNames.isEmpty())
    _typeToNames = _readTypeToNames();
  return _typeToNames;
}

QMultiHash<QString, QString> PoiPolygonSchema::_readTypeToNames()
{
  /* poi.polygon.type.to.names allows for more granularity when comparing elements with the same OSM
   * type. This is different than what is contained in the Hootenanny schema files, as this deals
   * with name text and not just feature types. Possibly, a more robust framework could eventually
   * be created for this capability if its proven more widely useful (more than just POI/Polygon
   * conflation).

    example 1; two features with tags: 1) name=Bristow High School, amenity=school,
    2) name=Bristow Elementary School, amenity=school. These two have matching types, and depending
    upon how name matching is configured, you could end up with a match/review between these two
    features when you should (most of the time) end up with a miss.

    example 2; two features with tags: 1) name=Bristow High School, amenity=school,
    2) name=Bristow College, amenity=school. In this case the college was mistakenly tagged as
    amenity=school, when it should have been tagged as amenity=university. These two should not
    match either.*/

  QMultiHash<QString, QString> typeToNames;
  const QStringList typeToNamesRaw = ConfigOptions().getPoiPolygonTypeToNames();
  for (const auto& typeToNamesRawEntry : qAsConst(typeToNamesRaw))
  {
    const QStringList typeToNamesRawEntryParts = typeToNamesRawEntry.split("|");
    if (typeToNamesRawEntryParts.size() != 2)
      throw HootException("Invalid POI/Polygon type to names entry: " + typeToNamesRawEntry);
    const QString kvp = typeToNamesRawEntryParts.at(0);
    const QStringList names = typeToNamesRawEntryParts.at(1).split(",");
    for (const auto& name : qAsConst(names))
      typeToNames.insert(kvp, name);
  }
  return typeToNames;
}

bool PoiPolygonSchema::_typeHasName(const QString& kvp, const QString& name)
{
  const QStringList typeNames = _getTypeToNames().values(kvp);
  for (const auto& typeName : qAsConst(typeNames))
  {
    if (name.contains(typeName))
      return true;
  }
  return false;
}

QString PoiPolygonSchema::_getMatchingTypeName(const QString& kvp, const QString& name)
{
  const QStringList typeNames = _getTypeToNames().values(kvp);
  for (const auto& typeName : qAsConst(typeNames))
  {
    if (name.contains(typeName))
      return typeName;
  }
  return "";
}

bool PoiPolygonSchema::_haveMatchingTypeNames(const QString& kvp, const QString& name1, const QString& name2)
{
  const QString typeName1 = _getMatchingTypeName(kvp, name1);
  const QString typeName2 = _getMatchingTypeName(kvp, name2);
  return typeName1 == typeName2 && !typeName1.isEmpty();
}

bool PoiPolygonSchema::isSchool(const ConstElementPtr& element)
{
  const QString amenityStr = element->getTags().get("amenity").toLower();
  return amenityStr == QLatin1String("school") || amenityStr == QLatin1String("university");
}

bool PoiPolygonSchema::isSpecificSchool(const ConstElementPtr& element)
{
  // Schools are the only example of the concept of trying to reduce reviews between features of the
  // same type when their names indicate they are actually different types.  If this concept proves
  // useful with other types, the code could be abstracted to handle them.
  if (!isSchool(element))
    return false;
  return _typeHasName("amenity=school", element->getTags().getName().toLower());
}

bool PoiPolygonSchema::specificSchoolMatch(const ConstElementPtr& element1,
                                         const ConstElementPtr& element2)
{
  if (isSpecificSchool(element1) && isSpecificSchool(element2))
  {
    const QString name1 = element1->getTags().getName().toLower();
    const QString name2 = element2->getTags().getName().toLower();
    if (_haveMatchingTypeNames("amenity=school", name1, name2))
      return true;
  }
  return false;
}

bool PoiPolygonSchema::isPark(const ConstElementPtr& element)
{
  return element->getTags().get("leisure").toLower() == QLatin1String("park") &&
          !BuildingCriterion().isSatisfied(element);
}

bool PoiPolygonSchema::isParkish(const ConstElementPtr& element)
{
  const QString leisureVal = element->getTags().get("leisure").toLower();
  return (leisureVal == QLatin1String("garden") || leisureVal == QLatin1String("dog_park")) &&
          !BuildingCriterion().isSatisfied(element);
}

bool PoiPolygonSchema::isPlayground(const ConstElementPtr& element)
{
  return element->getTags().get("leisure").toLower() == QLatin1String("playground");
}

bool PoiPolygonSchema::isSport(const ConstElementPtr& element)
{
  const Tags& tags = element->getTags();
  if (tags.contains("sport"))
    return true;

  const QString leisureVal = tags.get("leisure").toLower();
  return leisureVal.contains("sport") || leisureVal == QLatin1String("pitch");
}

bool PoiPolygonSchema::isRestroom(const ConstElementPtr& element)
{
  return element->getTags().get("amenity").toLower() == QLatin1String("toilets");
}

bool PoiPolygonSchema::isParking(const ConstElementPtr& element)
{
  const Tags& tags = element->getTags();
  if (tags.contains("parking"))
    return true;

  const QString amenityVal = tags.get("amenity").toLower();
  return amenityVal == QLatin1String("parking") || amenityVal == QLatin1String("bicycle_parking");
}

bool PoiPolygonSchema::isReligion(const ConstElementPtr& element)
{
  const Tags& tags = element->getTags();
  const QString amenityVal = tags.get("amenity").toLower();
  const QString buildingVal = tags.get(MetadataTags::Building()).toLower();
  return amenityVal == QLatin1String("place_of_worship") ||
         buildingVal == QLatin1String("church") ||
         buildingVal == QLatin1String("mosque") ||
         // TODO: this one is an alias of building=mosque, so we should be getting it from there
         // instead.
         amenityVal == QLatin1String("mosque") ||
         buildingVal == QLatin1String("synagogue");
}

bool PoiPolygonSchema::hasMoreThanOneType(const ConstElementPtr& element)
{
  int typeCount = 0;
  QStringList typesParsed;
  if (_allTypeKeys.empty())
    _allTypeKeys = OsmSchema::getInstance().getAllTypeKeys();

  const Tags elementTags = element->getTags();
  for (auto it = elementTags.begin(); it != elementTags.end(); ++it)
  {
    const QString elementTagKey = it.key();
    //  there may be duplicate keys in allTags
    if (_allTypeKeys.contains(elementTagKey) && !typesParsed.contains(elementTagKey))
    {
      LOG_TRACE("Has key: " << elementTagKey);
      typeCount++;
      if (typeCount > 1)
        return true;
    }
    typesParsed.append(elementTagKey);
  }
  return false;
}

bool PoiPolygonSchema::hasRelatedType(const ConstElementPtr& element)
{
  return OsmSchema::getInstance().getCategories(element->getTags()).intersects(
          OsmSchemaCategory::building() | OsmSchemaCategory::poi());
}

bool PoiPolygonSchema::hasSpecificType(const ConstElementPtr& element)
{
  const Tags& tags = element->getTags();
  return !tags.contains("poi") && tags.get(MetadataTags::Building()).toLower() != QLatin1String("yes") &&
          tags.get("office").toLower() != QLatin1String("yes") &&
          tags.get(MetadataTags::Area()).toLower() != QLatin1String("yes") && hasRelatedType(element);
}

bool PoiPolygonSchema::isRestaurant(const ConstElementPtr& element)
{
  const Tags& tags = element->getTags();
  const QString amenityVal = tags.get("amenity").toLower();
  return amenityVal == QLatin1String("restaurant") || amenityVal == QLatin1String("fast_food");
}

bool PoiPolygonSchema::isNatural(const ConstElementPtr& element)
{
  return element->getTags().contains("natural");
}

}
