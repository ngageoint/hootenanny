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

#include "PoiMatchDistance.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QFileInfo>

// Boost
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace hoot
{

PoiMatchDistance::PoiMatchDistance() :
_maxMatchDistance(-1),
_maxReviewDistance(-1)
{
}

PoiMatchDistance::PoiMatchDistance(
  QString key, QString val, int maxMatchDistance, int maxReviewDistance) :
_key(key),
_value(val),
_maxMatchDistance(maxMatchDistance),
_maxReviewDistance(maxReviewDistance)
{
}

QList<PoiMatchDistance> PoiMatchDistance::readDistances(const QString& jsonStringOrFile)
{ 
  LOG_DEBUG("Reading distances...");

  std::shared_ptr<boost::property_tree::ptree> propTree;
  if (!jsonStringOrFile.toLower().endsWith(".json"))
  {
    propTree = StringUtils::jsonStringToPropTree(jsonStringOrFile);
  }
  else
  {
    QFileInfo distancesFileInfo(jsonStringOrFile);
    if (!distancesFileInfo.exists())
    {
      throw IllegalArgumentException("POI to POI match distances file does not exist.");
    }

    propTree.reset(new boost::property_tree::ptree());
    try
    {
      boost::property_tree::read_json(jsonStringOrFile.toStdString(), *propTree);
    }
    catch (boost::property_tree::json_parser::json_parser_error& e)
    {
      throw HootException(
        QString("Error parsing JSON: %1 (line %2)")
          .arg(QString::fromStdString(e.message()))
          .arg(QString::number(e.line())));
    }
    catch (const std::exception& e)
    {
      const QString reason = e.what();
      throw HootException("Error parsing JSON " + reason);
    }
  }

  QList<PoiMatchDistance> distances;

  for (boost::property_tree::ptree::value_type& distProp : propTree->get_child("distances"))
  {
    const QString key =
      QString::fromStdString(distProp.second.get<std::string>("key", "")).trimmed();
    LOG_VART(key);
    if (key.isEmpty())
    {
      throw IllegalArgumentException("TODO");
    }

    // value is optional
    boost::optional<std::string> valProp = distProp.second.get_optional<std::string>("value");
    QString val = "";
    if (valProp)
    {
      val = QString::fromStdString(valProp.get()).trimmed();
    }
    LOG_VART(val);

    const int maxMatchDistance = distProp.second.get<int>("maxMatchDistance");
    LOG_VART(maxMatchDistance);
    if (maxMatchDistance < 0)
    {
      throw IllegalArgumentException("TODO");
    }

    const int maxReviewDistance = distProp.second.get<int>("maxReviewDistance");
    LOG_VART(maxReviewDistance);
    if (maxReviewDistance < 0)
    {
      throw IllegalArgumentException("TODO");
    }

    distances.append(PoiMatchDistance(key, val, maxMatchDistance, maxReviewDistance));
  }

  LOG_VARD(distances.size());
  return distances;
}

QString PoiMatchDistance::toString() const
{
  return
    "Key: " + _key + ", Value: " + _value + ", match: " + QString::number(_maxMatchDistance) +
    ", review: " + QString::number(_maxReviewDistance);
}

}
