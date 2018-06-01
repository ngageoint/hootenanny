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
_attributes(attributes)
{
}

void AddAttributesVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setAttributes(configOptions.getAddAttributesVisitorKvps());
}

void AddAttributesVisitor::visit(const boost::shared_ptr<Element>& e)
{
  //see extensibility issue comments in ElementAttributeType
  for (int i = 0; i < _attributes.length(); i++)
  {
    const QString attribute = _attributes.at(i);
    LOG_VART(attribute);
    const QStringList attributeParts = attribute.split("=");
    if (attributeParts.size() != 2)
    {
      throw IllegalArgumentException("Invalid attribute: " + attribute);
    }
    const QString attributeName = attributeParts[0];
    const QString attributeValue = attributeParts[1].trimmed();
    if (attributeValue.isEmpty())
    {
      throw IllegalArgumentException("Invalid empty attribute.");
    }
    ElementAttributeType attrType = ElementAttributeType::fromString(attributeName);
    bool ok = false;
    switch (attrType.getEnum())
    {
      case ElementAttributeType::Changeset:
        e->setChangeset(attributeValue.toLong(&ok));
        if (!ok)
        {
          throw IllegalArgumentException("Invalid attribute value: " + attributeValue);
        }
        break;
      case ElementAttributeType::Timestamp:
        e->setTimestamp(OsmUtils::fromTimeString(attributeValue));
        break;
      case ElementAttributeType::User:
        e->setUser(attributeValue);
        break;
      case ElementAttributeType::Uid:
        e->setUid(attributeValue.toLong(&ok));
        if (!ok)
        {
          throw IllegalArgumentException("Invalid attribute value: " + attributeValue);
        }
        break;
      case ElementAttributeType::Version:
        e->setVersion(attributeValue.toLong(&ok));
        if (!ok)
        {
          throw IllegalArgumentException("Invalid attribute value: " + attributeValue);
        }
        break;
    }
  }
}

}
