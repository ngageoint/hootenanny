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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "RemoveAttributesVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementAttributeType.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveAttributesVisitor)

RemoveAttributesVisitor::RemoveAttributesVisitor()
{
  setConfiguration(conf());
}

RemoveAttributesVisitor::RemoveAttributesVisitor(const QStringList types)
{
  setTypes(types);
}

RemoveAttributesVisitor::RemoveAttributesVisitor(const QList<ElementAttributeType>& types)
  : _types(types)
{
}

void RemoveAttributesVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setTypes(configOptions.getRemoveAttributesVisitorTypes());
}

void RemoveAttributesVisitor::setTypes(const QStringList types)
{
  LOG_VART(types);
  for (const auto& type_string : qAsConst(types))
    _types.append(ElementAttributeType::fromString(type_string));
  LOG_VART(_types);
}

void RemoveAttributesVisitor::visit(const std::shared_ptr<Element>& e)
{
  for (const auto& attrType : qAsConst(_types))
  {
    //LOG_VART(attrType.toString());
    switch (attrType.getEnum())
    {
    case ElementAttributeType::Changeset:
      e->setChangeset(ElementData::CHANGESET_EMPTY);
      LOG_TRACE("Removed " << attrType.toString() << ".");
      break;
    case ElementAttributeType::Timestamp:
      e->setTimestamp(ElementData::TIMESTAMP_EMPTY);
      LOG_TRACE("Removed " << attrType.toString() << ".");
      break;
    case ElementAttributeType::User:
      e->setUser(ElementData::USER_EMPTY);
      LOG_TRACE("Removed " << attrType.toString() << ".");
      break;
    case ElementAttributeType::Uid:
      e->setUid(ElementData::UID_EMPTY);
      LOG_TRACE("Removed " << attrType.toString() << ".");
      break;
    case ElementAttributeType::Version:
      e->setVersion(ElementData::VERSION_EMPTY);
      LOG_TRACE("Removed " << attrType.toString() << ".");
      break;
    default:
      throw IllegalArgumentException("Invalid attribute type: " + attrType.toString());
    }
  }
}

}
