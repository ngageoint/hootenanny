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
#include "TagCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagCriterion)

TagCriterion::TagCriterion()
{
  setConfiguration(conf());
}

TagCriterion::TagCriterion(const QString& k, const QString& v)
{
  _kvps.append(k + "=" + v);
}

void TagCriterion::setKvps(const QStringList kvps)
{
  _kvps = kvps;
  for (int i = 0; i < _kvps.size(); i++)
  {
    const QString kvp = _kvps.at(i);
    const QStringList kvpParts = kvp.split("=");
    if (kvpParts.size() != 2)
    {
      throw IllegalArgumentException("Invalid TagCriterion KVP: " + kvp);
    }
  }
}

void TagCriterion::setConfiguration(const Settings &s)
{
  setKvps(ConfigOptions(s).getTagCriterionKvps());
}

bool TagCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getTags());
  for (int i = 0; i < _kvps.size(); i++)
  {
    const QStringList kvpParts = _kvps.at(i).split("=");
    const QString key = kvpParts[0];
    const QString val = kvpParts[1];
    LOG_VART(key);
    LOG_VART(val);
    if (e->getTags().get(key) == val)
    {
      LOG_TRACE("crit satisifed");
      return true;
    }
  }
  return false;
}

}
