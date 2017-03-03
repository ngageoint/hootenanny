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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagKeyCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagKeyCriterion)

TagKeyCriterion::TagKeyCriterion(QString key)
{
  addKey(key);
}

TagKeyCriterion::TagKeyCriterion(QString key1, QString key2)
{
  addKey(key1);
  addKey(key2);
}

TagKeyCriterion::TagKeyCriterion(QString key1, QString key2, QString key3)
{
  addKey(key1);
  addKey(key2);
  addKey(key3);
}

TagKeyCriterion::TagKeyCriterion(QStringList keys)
{
  _keys = keys;
}

void TagKeyCriterion::addKey(QString key)
{
  _keys.append(key);
}

bool TagKeyCriterion::isSatisfied(const shared_ptr<const Element> &e) const
{
  for (int i = 0; i < _keys.size(); i++)
  {
    if (e->getTags().contains(_keys[i]))
    {
      return true;
    }
  }
  return false;
}

}
