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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "TagCriterion.h"

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagCriterion)

TagCriterion::TagCriterion()
  : _caseSensitive(false)
{
  setConfiguration(conf());
}

TagCriterion::TagCriterion(const QString& k, const QString& v)
  : _caseSensitive(false)
{
  _kvps.append(k + "=" + v);
}

void TagCriterion::setKvps(const QStringList kvps)
{
  _kvps = kvps;
  for (const auto& kvp : qAsConst(_kvps))
  {
    const QStringList kvpParts = kvp.split("=");
    if (kvpParts.size() != 2)
      throw IllegalArgumentException("Invalid TagCriterion KVP: " + kvp);
  }
}

void TagCriterion::setConfiguration(const Settings &s)
{
  ConfigOptions confOpts = ConfigOptions(s);
  setKvps(confOpts.getTagCriterionKvps());
  LOG_VART(_kvps);
  setCaseSensitive(confOpts.getTagCriterionCaseSensitive());
}

bool TagCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (_kvps.empty())
    throw IllegalArgumentException("No tag key/value pairs specified for: " + className());

  LOG_VART(e);
  const Qt::CaseSensitivity caseSensitivity = _caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
  for (const auto& kvp : qAsConst(_kvps))
  {
    const QStringList kvpParts = kvp.split("=");
    const QString key = kvpParts[0];
    const QString val = kvpParts[1];
    LOG_VART(key);
    LOG_VART(val);
    if (e->getTags().get(key).compare(val, caseSensitivity) == 0)
    {
      LOG_TRACE("crit satisifed");
      return true;
    }
  }
  return false;
}

QString TagCriterion::toString() const
{
  return className() + ":kvps:" + _kvps.join(",");
}

}
