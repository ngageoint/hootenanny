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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ImplicitTagCustomRules.h"

// hoot
#include <hoot/core/util/ConfigUtils.h>

namespace hoot
{

ImplicitTagCustomRules::ImplicitTagCustomRules()
{
  LOG_DEBUG("Intializing POI implicit tag custom rules...");
  _clear();
  _readCustomRuleFile();
  ConfigOptions opts;
  setTagIgnoreList(opts.getImplicitTaggingDatabaseDeriverTagIgnoreList());
  setWordIgnoreList(opts.getImplicitTaggingDatabaseDeriverWordIgnoreList());
}

void ImplicitTagCustomRules::_clear()
{
  _tagIgnoreList.clear();
  _wordIgnoreList.clear();
  _customRulesList.clear();
}

void ImplicitTagCustomRules::setCustomRuleFile(const QString& file)
{
  _customRuleFile = file;
  _readCustomRuleFile();
}

void ImplicitTagCustomRules::_readCustomRuleFile()
{
  LOG_VARD(_customRuleFile);
  if (!_customRuleFile.trimmed().isEmpty())
  {
    QFile customRulesFile(_customRuleFile);
    if (!customRulesFile.open(QIODevice::ReadOnly))
      throw HootException(QString("Error opening %1 for writing.").arg(customRulesFile.fileName()));
    _customRulesList.clear();
    while (!customRulesFile.atEnd())
    {
      const QString line = QString::fromUtf8(customRulesFile.readLine().constData()).trimmed();
      LOG_VART(line);
      if (!line.trimmed().isEmpty() && !line.startsWith("#"))
      {
        const QStringList lineParts = line.trimmed().split("\t");
        LOG_VART(lineParts);
        if (lineParts.size() != 2)
          throw HootException("Invalid custom rule: " + line);
        _customRulesList[lineParts[0].trimmed()] = lineParts[1].trimmed();
      }
    }
    customRulesFile.close();
  }
  LOG_VART(_customRulesList);
}

}
