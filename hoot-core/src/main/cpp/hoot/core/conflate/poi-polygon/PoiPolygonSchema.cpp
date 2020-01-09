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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonSchema.h"

// hoot
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QSet>
#include <QStringBuilder>

namespace hoot
{

QMultiHash<QString, QString> PoiPolygonSchema::_typeToNames;
QSet<QString> PoiPolygonSchema::_allTagKeys;

PoiPolygonSchema::PoiPolygonSchema()
{
  _readTypeToNames();
}

void PoiPolygonSchema::_readTypeToNames()
{
  // see related note in ImplicitTagUtils::_modifyUndesirableTokens
  if (_typeToNames.isEmpty())
  {
    const QStringList typeToNamesRaw =
      FileUtils::readFileToList(ConfigOptions().getPoiPolygonTypeToNamesFile());
    for (int i = 0; i < typeToNamesRaw.size(); i++)
    {
      const QString typeToNamesRawEntry = typeToNamesRaw.at(i);
      const QStringList typeToNamesRawEntryParts = typeToNamesRawEntry.split(";");
      if (typeToNamesRawEntryParts.size() != 2)
      {
        throw HootException("Invalid POI/Polygon type to names entry: " + typeToNamesRawEntry);
      }
      const QString kvp = typeToNamesRawEntryParts.at(0);
      const QStringList names = typeToNamesRawEntryParts.at(1).split(",");
      for (int j = 0; j < names.size(); j++)
      {
        _typeToNames.insert(kvp, names.at(j));
      }
    }
  }
}

bool PoiPolygonSchema::_typeHasName(const QString& kvp, const QString& name)
{
  const QStringList typeNames =_typeToNames.values(kvp);
  for (int i = 0; i < typeNames.size(); i++)
  {
    if (name.contains(typeNames.at(i)))
    {
      return true;
    }
  }
  return false;
}

QString PoiPolygonSchema::_getMatchingTypeName(const QString& kvp, const QString& name)
{
  const QStringList typeNames =_typeToNames.values(kvp);
  for (int i = 0; i < typeNames.size(); i++)
  {
    const QString typeName = typeNames.at(i);
    if (name.contains(typeName))
    {
      return typeName;
    }
  }
  return "";
}

bool PoiPolygonSchema::_haveMatchingTypeNames(const QString& kvp, const QString& name1,
                                                          const QString& name2)
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

// Schools are the only example of the concept of trying to reduce reviews between features of the
// same type when their names indicate they are actually different types.  If this concept proves
// useful with other types, the code could be abstracted to handle them.

bool PoiPolygonSchema::isSpecificSchool(const ConstElementPtr& element)
{
  if (!isSchool(element))
  {
    return false;
  }
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
    {
      return true;
    }
  }
  return false;
}

bool PoiPolygonSchema::isPark(const ConstElementPtr& element)
{
  return
    element->getTags().get("leisure").toLower() == QLatin1String("park") &&
    !BuildingCriterion().isSatisfied(element);
}

bool PoiPolygonSchema::isParkish(const ConstElementPtr& element)
{
  const QString leisureVal = element->getTags().get("leisure").toLower();
  return
    (leisureVal == QLatin1String("garden") || leisureVal == QLatin1String("dog_park")) &&
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
  {
    return true;
  }
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
  {
    return true;
  }
  const QString amenityVal = tags.get("amenity").toLower();
  return amenityVal == QLatin1String("parking") || amenityVal == QLatin1String("bicycle_parking");
}

bool PoiPolygonSchema::isReligion(const ConstElementPtr& element)
{
  const Tags& tags = element->getTags();
  const QString amenityVal = tags.get("amenity").toLower();
  const QString buildingVal = tags.get("building").toLower();
  return amenityVal == QLatin1String("place_of_worship") ||
         buildingVal == QLatin1String("church") ||
         buildingVal == QLatin1String("mosque") ||
         // TODO: this one is an alias of building=mosque, so we should be getting it from there
         //instead
         amenityVal == QLatin1String("mosque") ||
         buildingVal == QLatin1String("synagogue");
}

bool PoiPolygonSchema::hasMoreThanOneType(const ConstElementPtr& element)
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

bool PoiPolygonSchema::hasRelatedType(const ConstElementPtr& element)
{
  return
    OsmSchema::getInstance().getCategories(element->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
}

bool PoiPolygonSchema::hasSpecificType(const ConstElementPtr& element)
{
  const Tags& tags = element->getTags();
  return
    !tags.contains("poi") && tags.get("building").toLower() != QLatin1String("yes") &&
    tags.get("office").toLower() != QLatin1String("yes") &&
    tags.get("area").toLower() != QLatin1String("yes") && hasRelatedType(element);
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

