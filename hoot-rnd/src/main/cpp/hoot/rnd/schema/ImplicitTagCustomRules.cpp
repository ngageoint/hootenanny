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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ImplicitTagCustomRules.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

ImplicitTagCustomRules::ImplicitTagCustomRules()
{
  LOG_DEBUG("Intializing POI implicit tag custom rules...");
  _clear();
  ConfigOptions opts;
  setCustomRules(opts.getImplicitTaggingDatabaseDeriverCustomRules());
  setTagIgnoreList(opts.getImplicitTaggingDatabaseDeriverTagIgnoreList());
  setWordIgnoreList(opts.getImplicitTaggingDatabaseDeriverWordIgnoreList());
}

void ImplicitTagCustomRules::_clear()
{
  _tagIgnoreList.clear();
  _wordIgnoreList.clear();
  _customRulesList.clear();
}

void ImplicitTagCustomRules::setCustomRules(const QStringList& rawRules)
{
  for (int i = 0; i < rawRules.size(); i++)
  {
    const QString rule = rawRules.at(i).trimmed();
    LOG_VART(rule);
    if (!rule.trimmed().isEmpty() && !rule.startsWith("#"))
    {
      const QStringList ruleParts = rule.trimmed().split(",");
      LOG_VART(ruleParts);
      if (ruleParts.size() != 2)
      {
        throw HootException("Invalid custom rule: " + rule);
      }
      _customRulesList[ruleParts[0].trimmed()] = ruleParts[1].trimmed();
    }
  }
  LOG_VART(_customRulesList);
}

}
