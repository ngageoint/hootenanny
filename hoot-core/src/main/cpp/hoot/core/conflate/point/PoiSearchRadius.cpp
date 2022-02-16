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

#include "PoiSearchRadius.h"

// Hoot
#include <hoot/core/util/StringUtils.h>

// Boost
#include <boost/bind.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace hoot
{

PoiSearchRadius::PoiSearchRadius(QString key, QString val, int distance)
  : _key(key),
    _value(val),
    _distance(distance)
{
}

QList<PoiSearchRadius> PoiSearchRadius::readSearchRadii(const QString& jsonStringOrFile)
{ 
  LOG_DEBUG("Reading search radii from: " << jsonStringOrFile << "...");

  std::shared_ptr<boost::property_tree::ptree> propTree;
  if (!jsonStringOrFile.endsWith(".json", Qt::CaseInsensitive))
    propTree = StringUtils::jsonStringToPropTree(jsonStringOrFile);
  else
  {
    QFileInfo distancesFileInfo(jsonStringOrFile);
    if (!distancesFileInfo.exists())
      throw IllegalArgumentException("POI to POI search radii file does not exist.");

    propTree = std::make_shared<boost::property_tree::ptree>();
    try
    {
      boost::property_tree::read_json(jsonStringOrFile.toStdString(), *propTree);
    }
    catch (const boost::property_tree::json_parser::json_parser_error& e)
    {
      throw HootException(
        QString("Error parsing JSON: %1 (line %2)")
          .arg(QString::fromStdString(e.message()), QString::number(e.line())));
    }
    catch (const std::exception& e)
    {
      const QString reason = e.what();
      throw HootException("Error parsing JSON " + reason);
    }
  }

  QList<PoiSearchRadius> radii;

  for (const auto& distProp : propTree->get_child("search_radii"))
  {
    const QString key =
      QString::fromStdString(distProp.second.get<std::string>("key", "")).trimmed();
    LOG_VART(key);
    if (key.isEmpty())
      throw IllegalArgumentException("Missing 'key' in POI search radius entry.");

    // value is optional
    boost::optional<std::string> valProp = distProp.second.get_optional<std::string>("value");
    QString val = "";
    if (valProp)
      val = QString::fromStdString(valProp.get()).trimmed();
    LOG_VART(val);

    const int distance = distProp.second.get<int>("distance", -1);
    LOG_VART(distance);
    if (distance < 0)
      throw IllegalArgumentException("Missing 'distance' in POI search radius entry.");

    radii.append(PoiSearchRadius(key, val, distance));
  }

  LOG_VARD(radii.size());
  return radii;
}

QString PoiSearchRadius::toString() const
{
  return "Key: " + _key + ", Value: " + _value + ", distance: " + QString::number(_distance);
}

}
