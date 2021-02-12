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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */
#include "MultiaryPoiHashVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MultiaryPoiHashVisitor)

QString MultiaryPoiHashVisitor::_toJson(const ConstNodePtr& node) const
{
  QString result = "{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{";

  result += ElementHashVisitor::toJson(node->getTags(), node->getRawCircularError());

  const int coordinateComparisonSensitivity =
    ConfigOptions().getNodeComparisonCoordinateSensitivity();
  result += "}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[";
  result += QString::number(node->getX(), 'f', coordinateComparisonSensitivity);
  result += ",";
  result += QString::number(node->getY(), 'f', coordinateComparisonSensitivity);
  result += "]}}";

  return result;
}

}
