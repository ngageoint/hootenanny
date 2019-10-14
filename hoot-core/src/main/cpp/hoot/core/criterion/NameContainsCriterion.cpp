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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "NameContainsCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, NameContainsCriterion)

NameContainsCriterion::NameContainsCriterion() :
_caseSensitive(false)
{
}

NameContainsCriterion::NameContainsCriterion(const QStringList& names, const bool caseSensitive) :
_names(names),
_caseSensitive(caseSensitive)
{
}

void NameContainsCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _names = configOptions.getNameContainsCriterionNames();
  _caseSensitive = configOptions.getNameContainsCriterionCaseSensitive();
}

bool NameContainsCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const Qt::CaseSensitivity caseSensitivity =
    _caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
  const QStringList names = e->getTags().getNames();
  for (int i = 0; i < names.size(); i++)
  {
    for (int j = 0; j < _names.size(); j++)
    {
      if (!e->getTags().get(names.at(i)).contains(_names.at(j), caseSensitivity) == 0)
      {
        return true;
      }
    }
  }
  return false;
}

}
