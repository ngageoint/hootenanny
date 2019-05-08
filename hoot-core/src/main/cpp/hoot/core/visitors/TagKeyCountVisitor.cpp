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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagKeyCountVisitor.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, TagKeyCountVisitor)

TagKeyCountVisitor::TagKeyCountVisitor() :
_keyCount(0)
{
}

TagKeyCountVisitor::TagKeyCountVisitor(const QString& key) :
_keys(QStringList(key)),
_keyCount(0)
{
}

TagKeyCountVisitor::TagKeyCountVisitor(const QStringList& keys) :
_keys(keys),
_keyCount(0)
{
}

void TagKeyCountVisitor::setConfiguration(const Settings& conf)
{
  _keys = ConfigOptions(conf).getTagsVisitorKeys();
  LOG_VART(_keys);
}

void TagKeyCountVisitor::visit(const ConstElementPtr& e)
{
  for (int i = 0; i < _keys.size(); i++)
  {
    const QString key = _keys.at(i);
    if (e->getTags().contains(key))
    {
      _keyCount++;
    }
  }
}

}
