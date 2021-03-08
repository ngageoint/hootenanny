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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonTypeScoreExtractor.h"

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonDistanceTruthRecorder.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchemaType.h>

// Qt
#include <QSet>
#include <QStringBuilder>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonTypeScoreExtractor)

std::shared_ptr<ToEnglishTranslator> PoiPolygonTypeScoreExtractor::_translator;
QMap<QString, QSet<QString>> PoiPolygonTypeScoreExtractor::_categoriesToSchemaTagValues;

PoiPolygonTypeScoreExtractor::PoiPolygonTypeScoreExtractor(PoiPolygonInfoCachePtr infoCache) :
_typeScoreThreshold(-1.0),
_featureDistance(-1.0),
_printMatchDistanceTruth(false),
_translateTagValuesToEnglish(false),
_noTypeFound(false),
_infoCache(infoCache)
{ 
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
    _translator->setId(className());
  }
}

double PoiPolygonTypeScoreExtractor::extract(const OsmMap& /*map*/,
                                             const ConstElementPtr& poi,
                                             const ConstElementPtr& poly) const
{
  if (!_infoCache)
  {
    throw HootException("No cache passed to extractor.");
  }

  LOG_VART(_translateTagValuesToEnglish);

  //be a little more restrictive with each of these
  if (_failsCuisineMatch(poi, poly))
  {
    _failedMatchRequirements.append("cusine");
    return 0.0;
  }
  else if (_failsSportMatch(poi, poly))
  {
    _failedMatchRequirements.append("sport");
    return 0.0;
  }
  else if (_failsReligionMatch(poi, poly))
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
  if (tagValue.toLower().startsWith("http://") ||
      tagValue.toLower().startsWith("https://"))
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

  QStringList poiTagList, polyTagList;
  poiTagList = _getRelatedTags(poi->getTags());
  polyTagList = _getRelatedTags(poly->getTags());

  LOG_VART(poiTagList);
  LOG_VART(polyTagList);
  if (poiTagList.empty() || polyTagList.empty())
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

  // Can this be replaced with OsmSchema::score(const Tags& tags1, const Tags& tags2)?
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
        tagsList.append(key % "=" % value);
      }
    }
  }
  return tagsList;
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
      OsmSchema::getInstance().score(tagKey % "=" % t1Val, tagKey % "=" % t2Val) != 1.0)
  {
    return true;
  }
  return false;
}

bool PoiPolygonTypeScoreExtractor::_failsCuisineMatch(const ConstElementPtr& e1,
                                                      const ConstElementPtr& e2) const
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();
  QString t1Val;
  QString t2Val;
  if (_infoCache->isType(e1, PoiPolygonSchemaType::Restaurant) &&
      _infoCache->isType(e2, PoiPolygonSchemaType::Restaurant) &&
      _haveConflictingTags("cuisine", t1, t2, t1Val, t2Val))
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

bool PoiPolygonTypeScoreExtractor::_failsSportMatch(const ConstElementPtr& e1,
                                                    const ConstElementPtr& e2) const
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();
  if (_infoCache->isType(e1, PoiPolygonSchemaType::Sport) &&
      _infoCache->isType(e2, PoiPolygonSchemaType::Sport))
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

bool PoiPolygonTypeScoreExtractor::_failsReligionMatch(const ConstElementPtr& e1,
                                                       const ConstElementPtr& e2) const
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();
  if (_infoCache->isType(e1, PoiPolygonSchemaType::Religion) &&
       _infoCache->isType(e2, PoiPolygonSchemaType::Religion))
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
