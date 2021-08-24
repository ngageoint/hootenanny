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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "TagContainsCriterion.h"

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagContainsCriterion)

TagContainsCriterion::TagContainsCriterion() :
_caseSensitive(false)
{
}

TagContainsCriterion::TagContainsCriterion(QString key, QString valueSubstring) :
_caseSensitive(false)
{
  _keys.append(key);
  _valueSubstrings.append(valueSubstring);
}

void TagContainsCriterion::setConfiguration(const Settings &s)
{
  ConfigOptions config = ConfigOptions(s);
  setKvps(config.getTagContainsCriterionKvps());
  setCaseSensitive(config.getTagContainsCriterionCaseSensitive());
}

void TagContainsCriterion::setKvps(const QStringList kvps)
{
  for (int i = 0; i < kvps.size(); i++)
  {
    const QString kvp = kvps.at(i);
    const QStringList kvpParts = kvp.split("=");
    if (kvpParts.size() != 2)
    {
      throw IllegalArgumentException("Invalid TagCriterion KVP: " + kvp);
    }
    addPair(kvpParts.at(0), kvpParts.at(1));
  }
}

bool TagContainsCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (_keys.empty() || _valueSubstrings.empty())
  {
    throw IllegalArgumentException(
      "No tag keys or values specified for: " + className());
  }

  bool matches = false;
  const Qt::CaseSensitivity caseSensitivity =
    _caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
  LOG_VART(_keys);
  LOG_VART(_valueSubstrings);
  LOG_VART(e->getTags());
  for (int i = 0; i < _keys.size(); i++)
  {
    if (e->getTags().contains(_keys[i]) &&
        e->getTags()[_keys[i]].contains(_valueSubstrings[i], caseSensitivity))
    {
      matches = true;
      break;  //  Only one match is required
    }
  }
  if (matches)
  {
    LOG_TRACE("crit satisfied");
  }
  return matches;
}

void TagContainsCriterion::addPair(QString key, QString valueSubstring)
{
  _keys.append(key);
  _valueSubstrings.append(valueSubstring);
}

QString TagContainsCriterion::toString() const
{
  return
    className().remove("hoot::") + ":keys:" + _keys.join(",") + ":vals:" +
    _valueSubstrings.join(",");
}

}
