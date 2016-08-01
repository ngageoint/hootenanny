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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveTagVisitor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveTagVisitor)

RemoveTagVisitor::RemoveTagVisitor()
{
  setConfiguration(conf());
}

RemoveTagVisitor::RemoveTagVisitor(QString key)
{
  addKey(key);
}

RemoveTagVisitor::RemoveTagVisitor(QString key1, QString key2)
{
  addKey(key1);
  addKey(key2);
}

RemoveTagVisitor::RemoveTagVisitor(QStringList keys) :
_keys(keys)
{
}

void RemoveTagVisitor::setConfiguration(const Settings& conf)
{
  _keys = ConfigOptions(conf).getRemoveTagVisitorKeys();
  LOG_VARD(_keys);
}

void RemoveTagVisitor::addKey(QString key)
{
  _keys.append(key);
}

void RemoveTagVisitor::visit(const shared_ptr<Element>& e)
{
  for (int i = 0; i < _keys.size(); i++)
  {
    LOG_DEBUG("Remove tag " << _keys[i]);
    e->getTags().remove(_keys[i]);
  }
}

}
