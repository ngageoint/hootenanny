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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveAttributeVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementAttributeType.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveAttributeVisitor)

RemoveAttributeVisitor::RemoveAttributeVisitor()
{
  setConfiguration(conf());
}

RemoveAttributeVisitor::RemoveAttributeVisitor(const QList<ElementAttributeType>& types) :
_types(types)
{
}

void RemoveAttributeVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setTypes(configOptions.getRemoveAttributeVisitorTypes());
}

void RemoveAttributeVisitor::setTypes(const QStringList& types)
{
  LOG_VART(types);
  for (int i = 0; i < types.size(); i++)
  {
    _types.append(ElementAttributeType::fromString(types.at(i)));
  }
}

void RemoveAttributeVisitor::visit(const boost::shared_ptr<Element>& e)
{
  //see extensibility issue comments in ElementAttributeType
  for (int i = 0; i < _types.length(); i++)
  {
    LOG_VART(_types.at(i).toString());
    switch (_types.at(i).getEnum())
    {
      case ElementAttributeType::Changeset:
        e->setChangeset(ElementData::CHANGESET_EMPTY);
        break;
      case ElementAttributeType::Timestamp:
        e->setTimestamp(ElementData::TIMESTAMP_EMPTY);
        break;
      case ElementAttributeType::User:
        e->setUser(ElementData::USER_EMPTY);
        break;
      case ElementAttributeType::Uid:
        e->setUid(ElementData::UID_EMPTY);
        break;
      case ElementAttributeType::Version:
        e->setVersion(ElementData::VERSION_EMPTY);
        break;
    }
  }
}

}
