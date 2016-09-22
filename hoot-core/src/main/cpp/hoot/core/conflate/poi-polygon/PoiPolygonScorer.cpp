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
#include "PoiPolygonScorer.h"

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/Translator.h>

namespace hoot
{

PoiPolygonScorer::PoiPolygonScorer(const QString testUuid = "") :
_testUuid(testUuid)
{

}

double PoiPolygonScorer::getNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  return
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))))
   .extract(e1, e2);
}

double PoiPolygonScorer::getExactNameScore(ConstElementPtr e1, ConstElementPtr e2) const
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

double PoiPolygonScorer::getTypeScore(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                                      QString& t2BestKvp)
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

  const double typeScore = _getTagScore(e1, e2, t1BestKvp, t2BestKvp);

  if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    LOG_VARD(typeScore);
  }

  return typeScore;
}

double PoiPolygonScorer::getTypeScore(ConstElementPtr e1, ConstElementPtr e2)
{
  QString t1BestKvp, t2BestKvp;
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp);
}

bool PoiPolygonScorer::exactTypeMatch(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                                      QString& t2BestKvp)
{
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp) == 1.0;
}

bool PoiPolygonScorer::exactTypeMatch(ConstElementPtr e1, ConstElementPtr e2)
{
  QString t1BestKvp, t2BestKvp;
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp) == 1.0;
}

double PoiPolygonScorer::_getTagScore(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                                      QString& t2BestKvp)
{
  double result = 0.0;
  t1BestKvp = "";
  t2BestKvp = "";

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
          t1BestKvp = t1Kvp;
        }
        if (!t2Kvp.isEmpty() && t2Kvp != "building=yes" && t2Kvp != "poi=yes")
        {
          t2BestKvp = t2Kvp;
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

QStringList PoiPolygonScorer::_getRelatedTags(const Tags& tags) const
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

}
