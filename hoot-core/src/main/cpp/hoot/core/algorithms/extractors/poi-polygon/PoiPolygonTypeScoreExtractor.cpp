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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonTypeScoreExtractor.h"

// hoot
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonDistanceTruthRecorder.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
// Qt
#include <QSet>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonTypeScoreExtractor)

std::shared_ptr<ToEnglishTranslator> PoiPolygonTypeScoreExtractor::_translator;
QSet<QString> PoiPolygonTypeScoreExtractor::_allTagKeys;
QMap<QString, QSet<QString>> PoiPolygonTypeScoreExtractor::_categoriesToSchemaTagValues;
QMultiHash<QString, QString> PoiPolygonTypeScoreExtractor::_typeToNames;

PoiPolygonTypeScoreExtractor::PoiPolygonTypeScoreExtractor() :
_typeScoreThreshold(-1.0),
_featureDistance(-1.0),
_printMatchDistanceTruth(false),
_translateTagValuesToEnglish(false),
_noTypeFound(false)
{
  _readTypeToNames();
}

void PoiPolygonTypeScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  setTypeScoreThreshold(config.getPoiPolygonTypeScoreThreshold());
  setPrintMatchDistanceTruth(config.getPoiPolygonPrintMatchDistanceTruth());

  _translateTagValuesToEnglish = config.getPoiPolygonTypeTranslateToEnglish();
  if (_translateTagValuesToEnglish && !_translator)
  {
    _translator.reset(
      Factory::getInstance().constructObject<ToEnglishTranslator>(
        config.getLanguageTranslationTranslator()));
    _translator->setConfiguration(conf);
    _translator->setSourceLanguages(config.getLanguageTranslationSourceLanguages());
    _translator->setId(QString::fromStdString(className()));
  }
}

void PoiPolygonTypeScoreExtractor::_readTypeToNames()
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

double PoiPolygonTypeScoreExtractor::extract(const OsmMap& /*map*/,
                                             const ConstElementPtr& poi,
                                             const ConstElementPtr& poly) const
{
  LOG_VART(_translateTagValuesToEnglish);

  const Tags& t1 = poi->getTags();
  const Tags& t2 = poly->getTags();

  //be a little more restrictive with each of these
  if (_failsCuisineMatch(t1, t2))
  {
    _failedMatchRequirements.append("cusine");
    return 0.0;
  }
  else if (_failsSportMatch(t1, t2))
  {
    _failedMatchRequirements.append("sport");
    return 0.0;
  }
  else if (_failsReligionMatch(t1, t2))
  {
    _failedMatchRequirements.append("religion");
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

QSet<QString> PoiPolygonTypeScoreExtractor::_getTagValueTokens(const QString& category)
{
  if (_categoriesToSchemaTagValues[category].isEmpty())
  {
    const std::vector<SchemaVertex> tags =
      OsmSchema::getInstance().getTagByCategory(OsmSchemaCategory::fromString(category));
    for (std::vector<SchemaVertex>::const_iterator tagItr = tags.begin();
         tagItr != tags.end(); ++tagItr)
    {
      SchemaVertex tag = *tagItr;
      const QString tagVal = tag.value.toLower();
      if (!tagVal.contains("*"))  //skip wildcards
      {
        _categoriesToSchemaTagValues[category].insert(tagVal);
        LOG_TRACE("Appended " << tagVal << " to schema tag values.");
      }
    }
    LOG_VART(_categoriesToSchemaTagValues.size());
  }
  return _categoriesToSchemaTagValues[category];
}

void PoiPolygonTypeScoreExtractor::_translateTagValue(const QString& tagKey, QString& tagValue) const
{
  LOG_VART(tagKey);
  LOG_VART(tagValue);

  //don't care about urls
  if (tagValue.toLower().startsWith("http://"))
  {
    return;
  }

  //If the tag key is not in the categories we're interested in, pass.
  const bool inABuildingOrPoiCategory =
    OsmSchema::getInstance()
      .getCategories(tagKey, tagValue)
      .intersects(OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  LOG_VART(inABuildingOrPoiCategory);
  if (!inABuildingOrPoiCategory)
  {
    LOG_TRACE(
      "Input tag to translate: " << tagKey << "=" << tagValue << " is not a building/poi tag.");
    return;
  }

  //If the tag key is already OSM, then no need to translate it.
  // TODO: Should this also have use and/or building categories be added here?
  if (_getTagValueTokens("poi").contains(tagValue))
  {
    LOG_TRACE("Input tag value to translate: " << tagValue << " is already a poi tag value.");
    return;
  }

  //Translate the whole phrase, as the translator may be able to derive context from more than
  //one word.
  const QString tagValueTemp = tagValue.toLower().simplified().replace("_", " ");
  LOG_VART(tagValueTemp);
  QString translatedTagValue = _translator->translate(tagValueTemp).toLower();
  LOG_VART(translatedTagValue);
  if (translatedTagValue.isEmpty())
  {
    LOG_TRACE("To English translation for: " << tagValue << " same as original text.");
    return;
  }

  translatedTagValue = translatedTagValue.simplified().replace(" ", "_");
  LOG_TRACE(
    "Translated tag value: " << tagValue << " for key: " << tagKey << " to value: " <<
    translatedTagValue);
  tagValue = translatedTagValue;
}

double PoiPolygonTypeScoreExtractor::_getTagScore(ConstElementPtr poi,
                                                  ConstElementPtr poly) const
{
  double result = 0.0;

  QStringList poiTagList = _getRelatedTags(poi->getTags());
  QStringList polyTagList = _getRelatedTags(poly->getTags());
  LOG_VART(poiTagList);
  LOG_VART(polyTagList);
  if (poiTagList.size() == 0 || polyTagList.size() == 0)
  {
    _noTypeFound = true;
    LOG_TRACE("No valid type found when comparing: " << poi << " to: " << poly);
    return 0.0;
  }

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
    const QString poiKvp = poiTagList.at(i).toLower();
    for (int j = 0; j < polyTagList.size(); j++)
    {
      const QString polyKvp = polyTagList.at(j).toLower();
      LOG_VART(poiKvp);
      LOG_VART(polyKvp);

      const double score = OsmSchema::getInstance().score(poiKvp, polyKvp);
      LOG_VART(score);
      if (score >= result)
      {
        if (!poiKvp.isEmpty() && !excludeKvps.contains(poiKvp))
        {
          _poiBestKvp = poiKvp;
          LOG_VART(_poiBestKvp);
        }
        if (!polyKvp.isEmpty() && !excludeKvps.contains(polyKvp))
        {
          _polyBestKvp = polyKvp;
          LOG_VART(_polyBestKvp);
        }
      }
      result = max(score, result);
      LOG_VART(result);

      if (result == 1.0)
      {
        if (_printMatchDistanceTruth)
        {
          LOG_VART(_poiBestKvp);
          LOG_VART(_polyBestKvp);
          PoiPolygonDistanceTruthRecorder::recordDistanceTruth(
            poi, poly, _poiBestKvp, _polyBestKvp, _featureDistance);
        }
        return result;
      }
    }
  }
  LOG_VART(_poiBestKvp);
  LOG_VART(_polyBestKvp);

  if (_printMatchDistanceTruth)
  {
    PoiPolygonDistanceTruthRecorder::recordDistanceTruth(
      poi, poly, _poiBestKvp, _polyBestKvp, _featureDistance);
  }

  return result;
}

QStringList PoiPolygonTypeScoreExtractor::_getRelatedTags(const Tags& tags) const
{
  QStringList tagsList;
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString key = it.key();
    QStringList values = it.value().split(";");
    for (int i = 0; i < values.size(); i++)
    {
      QString value = values.at(i);

      if (_translateTagValuesToEnglish)
      {
        _translateTagValue(key, value);
      }

      if ((OsmSchema::getInstance().getCategories(key, value) &
           (OsmSchemaCategory::building() | OsmSchemaCategory::use() | OsmSchemaCategory::poi()))
             != OsmSchemaCategory::Empty)
      {
        tagsList.append(key + "=" + value);
      }
    }
  }
  return tagsList;
}

bool PoiPolygonTypeScoreExtractor::_typeHasName(const QString& kvp, const QString& name)
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

QString PoiPolygonTypeScoreExtractor::_getMatchingTypeName(const QString& kvp, const QString& name)
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

bool PoiPolygonTypeScoreExtractor::_haveMatchingTypeNames(const QString& kvp, const QString& name1,
                                                          const QString& name2)
{
  const QString typeName1 = _getMatchingTypeName(kvp, name1);
  const QString typeName2 = _getMatchingTypeName(kvp, name2);
  return typeName1 == typeName2 && !typeName1.isEmpty();
}

// As part of #2633, attempted to re-implement some of this hardcoded type code as categories in
// the hoot schema.  In doing that, several strange bugs started occurring and many poi/poly unit
// tests started to break.  Using the categories in that manner may not be the best approach and
// possibly a different one is needed.  The branch "2633-new-categories" is an example of the failed
// changes.

bool PoiPolygonTypeScoreExtractor::isSchool(ConstElementPtr element)
{
  const QString amenityStr = element->getTags().get("amenity").toLower();
  return amenityStr == "school" || amenityStr == "university";
}

// Schools are the only example of the concept of trying to reduce reviews between features of the
// same type when their names indicate they are actually different types.  If this concept proves
// useful with other types, the code could be abstracted to handle them.

bool PoiPolygonTypeScoreExtractor::isSpecificSchool(ConstElementPtr element)
{
  if (!isSchool(element))
  {
    return false;
  }
  return _typeHasName("amenity=school", element->getTags().getName().toLower());
}

bool PoiPolygonTypeScoreExtractor::specificSchoolMatch(ConstElementPtr element1,
                                                       ConstElementPtr element2)
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

bool PoiPolygonTypeScoreExtractor::isPark(ConstElementPtr element)
{
  return !BuildingCriterion().isSatisfied(element) &&
         (element->getTags().get("leisure") == "park");
}

bool PoiPolygonTypeScoreExtractor::isParkish(ConstElementPtr element)
{
  if (BuildingCriterion().isSatisfied(element))
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

bool PoiPolygonTypeScoreExtractor::isSport(const Tags& tags)
{
  const QString leisureVal = tags.get("leisure").toLower();
  return tags.contains("sport") || leisureVal.contains("sport") || leisureVal == "pitch";
}

bool PoiPolygonTypeScoreExtractor::isSport(ConstElementPtr element)
{
  return isSport(element->getTags());
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
         // TODO: this one is an alias of building=mosque, so we should be getting it from there
         //instead
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

bool PoiPolygonTypeScoreExtractor::isNatural(ConstElementPtr element)
{
  return element->getTags().contains("natural");
}

bool PoiPolygonTypeScoreExtractor::_haveConflictingTags(const QString& tagKey, const Tags& t1,
                                                        const Tags& t2, QString& tag1Val,
                                                        QString& tag2Val) const
{
  const QString t1Val = t1.get(tagKey).toLower();
  const bool t1HasVal = !t1Val.trimmed().isEmpty();
  const QString t2Val = t2.get(tagKey).toLower();
  const bool t2HasVal = !t2Val.trimmed().isEmpty();
  tag1Val = t1Val;
  tag2Val = t2Val;
  if (t1HasVal && t2HasVal &&
      OsmSchema::getInstance().score(tagKey + "=" + t1Val, tagKey + "=" + t2Val) != 1.0)
  {
    return true;
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::_failsCuisineMatch(const Tags& t1, const Tags& t2) const
{
  QString t1Val;
  QString t2Val;
  if (isRestaurant(t1) && isRestaurant(t2) && _haveConflictingTags("cuisine", t1, t2, t1Val, t2Val))
  {
    if (//Don't return false on regional, since its location dependent, and we don't take the
        //location into account for this.
        t1Val != "regional" && t2Val != "regional" &&
        //Don't fail on "other", since that's not very descriptive.
        t1Val != "other" && t2Val != "other")
    {
      LOG_TRACE("Failed type match on different cuisines.");
      return true;
    }
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::_failsSportMatch(const Tags& t1, const Tags& t2) const
{
  if (isSport(t1) && isSport(t2))
  {
    QString t1Val;
    QString t2Val;

    if (_haveConflictingTags("sport", t1, t2, t1Val, t2Val))
    {
      LOG_TRACE("Failed type match on different sports: " << t1Val << ", " << t2Val);
      return true;
    }
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::_failsReligionMatch(const Tags& t1, const Tags& t2) const
{
  if (isReligion(t1) && isReligion(t2))
  {
    QString t1Val;
    QString t2Val;

    if (_haveConflictingTags("denomination", t1, t2, t1Val, t2Val))
    {
      LOG_TRACE(
        "Failed type match on different religious denominations: " << t1Val << ", " << t2Val);
      return true;
    }

    if (_haveConflictingTags("religion", t1, t2, t1Val, t2Val))
    {
      LOG_TRACE("Failed type match on different religions: " << t1Val << ", " << t2Val);
      return true;
    }
  }
  return false;
}

}
