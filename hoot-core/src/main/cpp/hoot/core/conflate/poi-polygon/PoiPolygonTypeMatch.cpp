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
#include "PoiPolygonTypeMatch.h"

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

PoiPolygonTypeMatch::PoiPolygonTypeMatch(double typeScoreThreshold,
                                   const QString testUuid = "") :
_typeScoreThreshold(typeScoreThreshold),
_testUuid(testUuid)
{

}

double PoiPolygonTypeMatch::getTypeScore(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                                      QString& t2BestKvp)
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();

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
      LOG_DEBUG("Failed type match on different cuisines.");
      return false;
    }
  }

  /*const QString religion1Val = t1.get("religion").toLower().trimmed();
  const QString religion2Val = t2.get("religion").toLower().trimmed();
  if (!religion1Val.isEmpty() && !religion1Val.isEmpty() &&
      OsmSchema::getInstance().score("religion=" + religion1Val, "religion=" + religion2Val) == 0.0)
  {
    LOG_DEBUG("Failed type match on different religions.");
    return false;
  }*/

  const double typeScore = _getTagScore(e1, e2, t1BestKvp, t2BestKvp);

  if (e1->getTags().get("uuid") == _testUuid ||
      e2->getTags().get("uuid") == _testUuid)
  {
    LOG_VARD(typeScore);
  }

  return typeScore;
}

double PoiPolygonTypeMatch::getTypeScore(ConstElementPtr e1, ConstElementPtr e2)
{
  QString t1BestKvp, t2BestKvp;
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp);
}

bool PoiPolygonTypeMatch::isTypeMatch(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                                   QString& t2BestKvp)
{
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp) >= _typeScoreThreshold;
}

bool PoiPolygonTypeMatch::isTypeMatch(ConstElementPtr e1, ConstElementPtr e2)
{
  QString t1BestKvp, t2BestKvp;
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp) >= _typeScoreThreshold;
}

bool PoiPolygonTypeMatch::isExactTypeMatch(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                                      QString& t2BestKvp)
{
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp) == 1.0;
}

bool PoiPolygonTypeMatch::isExactTypeMatch(ConstElementPtr e1, ConstElementPtr e2)
{
  QString t1BestKvp, t2BestKvp;
  return getTypeScore(e1, e2, t1BestKvp, t2BestKvp) == 1.0;
}

double PoiPolygonTypeMatch::_getTagScore(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
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
      const QString t1Kvp = t1List.at(i).toLower();
      const QString t2Kvp = t2List.at(j).toLower();
      const double score = OsmSchema::getInstance().score(t1Kvp, t2Kvp);
      if (score >= result)
      {
        //TODO: convert these to a list
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

QStringList PoiPolygonTypeMatch::_getRelatedTags(const Tags& tags) const
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
