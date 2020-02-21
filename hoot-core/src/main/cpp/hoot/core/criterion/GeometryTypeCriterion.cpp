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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "GeometryTypeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

QStringList GeometryTypeCriterion::getCriterionClassNamesByGeometryType(const GeometryType& type)
{
  QStringList classNamesByType;
  std::vector<std::string> classNames =
    Factory::getInstance().getObjectNamesByBase("hoot::ElementCriterion");
  for (size_t i = 0; i < classNames.size(); i++)
  {
    const std::string className = classNames[i];

    ElementCriterionPtr crit(
      Factory::getInstance().constructObject<ElementCriterion>(className));
    std::shared_ptr<GeometryTypeCriterion> geometryTypeCrit =
      std::dynamic_pointer_cast<GeometryTypeCriterion>(crit);
    if (geometryTypeCrit && geometryTypeCrit->getGeometryType() == type)
    {
      classNamesByType.append(QString::fromStdString(className));
    }
  }
  return classNamesByType;
}

QString GeometryTypeCriterion::typeToString(const GeometryType& geometryType)
{
  switch (geometryType)
  {
    case GeometryType::Point:
      return "point";
    case GeometryType::Line:
      return "line";
    case GeometryType::Polygon:
      return "polygon";
    default:
      return "unknown";
  }
}

GeometryTypeCriterion::GeometryType GeometryTypeCriterion::typeFromString(
  const QString& geometryTypeStr)
{
  const QString compareVal = geometryTypeStr.toLower();
  if (compareVal == "point")
  {
    return GeometryType::Point;
  }
  else if (compareVal == "line")
  {
    return GeometryType::Line;
  }
  else if (compareVal == "polygon")
  {
    return GeometryType::Polygon;
  }
  else
  {
    return GeometryType::Unknown;
  }
}

}
