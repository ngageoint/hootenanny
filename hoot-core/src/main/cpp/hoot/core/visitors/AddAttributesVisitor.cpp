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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AddAttributesVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementAttributeType.h>
#include <hoot/core/util/OsmUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, AddAttributesVisitor)

AddAttributesVisitor::AddAttributesVisitor()
{
  setConfiguration(conf());
}

AddAttributesVisitor::AddAttributesVisitor(const QStringList attributes) :
_attributes(attributes),
_addOnlyIfEmpty(ConfigOptions().getAddAttributesVisitorAddOnlyIfEmpty())
{
}

void AddAttributesVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _attributes = configOptions.getAddAttributesVisitorKvps();
  _addOnlyIfEmpty = configOptions.getAddAttributesVisitorAddOnlyIfEmpty();
}

void AddAttributesVisitor::visit(const boost::shared_ptr<Element>& e)
{
  for (int i = 0; i < _attributes.length(); i++)
  {
    QString attributeValue;
    const ElementAttributeType attrType = _getAttributeType(_attributes.at(i), attributeValue);

    bool ok = false;
    switch (attrType.getEnum())
    {
      case ElementAttributeType::Changeset:
        if (!_addOnlyIfEmpty || e->getChangeset() == ElementData::CHANGESET_EMPTY)
        {
          e->setChangeset(attributeValue.toLong(&ok));
          if (!ok)
          {
            throw IllegalArgumentException("Invalid attribute value: " + attributeValue);
          }
          LOG_TRACE("Added " << attrType.toString() << "=" << attributeValue);
        }
        break;

      case ElementAttributeType::Timestamp:
        if (!_addOnlyIfEmpty || e->getTimestamp() == ElementData::TIMESTAMP_EMPTY)
        {
          e->setTimestamp(OsmUtils::fromTimeString(attributeValue));
          LOG_TRACE("Added " << attrType.toString() << "=" << attributeValue);
        }
        break;

      case ElementAttributeType::User:
        if (!_addOnlyIfEmpty || e->getUser() == ElementData::USER_EMPTY)
        {
          e->setUser(attributeValue);
        }
        LOG_TRACE("Added " << attrType.toString() << "=" << attributeValue);
        break;

      case ElementAttributeType::Uid:
        if (!_addOnlyIfEmpty || e->getUid() == ElementData::UID_EMPTY)
        {
          e->setUid(attributeValue.toLong(&ok));
          if (!ok)
          {
            throw IllegalArgumentException("Invalid attribute value: " + attributeValue);
          }
          LOG_TRACE("Added " << attrType.toString() << "=" << attributeValue);
        }
        break;

      case ElementAttributeType::Version:
        if (!_addOnlyIfEmpty || e->getVersion() == ElementData::VERSION_EMPTY)
        {
          e->setVersion(attributeValue.toLong(&ok));
          if (!ok)
          {
            throw IllegalArgumentException("Invalid attribute value: " + attributeValue);
          }
          LOG_TRACE("Added " << attrType.toString() << "=" << attributeValue);
        }
        break;

      default:
        throw IllegalArgumentException("Invalid attribute value: " + attributeValue);
    }
  }
}

ElementAttributeType::Type AddAttributesVisitor::_getAttributeType(const QString attribute,
                                                                   QString& attributeValue)
{
  //LOG_VART(attribute);
  const QStringList attributeParts = attribute.split("=");
  if (attributeParts.size() != 2)
  {
    throw IllegalArgumentException("Invalid attribute: " + attribute);
  }
  const QString attributeName = attributeParts[0];
  attributeValue = attributeParts[1].trimmed();
  if (attributeValue.isEmpty())
  {
    throw IllegalArgumentException("Invalid empty attribute.");
  }
  return ElementAttributeType::fromString(attributeName);
}

}
