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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ImplicitTagCustomRules.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QFile>

namespace hoot
{

ImplicitTagCustomRules::ImplicitTagCustomRules()
{
}

void ImplicitTagCustomRules::init()
{
  LOG_DEBUG("Intializing POI implicit tag custom rules...");
  _clear();
  _readAllowLists();
  _readIgnoreLists();
}

void ImplicitTagCustomRules::_clear()
{
  _tagIgnoreList.clear();
  _wordIgnoreList.clear();
  _customRulesList.clear();
}

void ImplicitTagCustomRules::_readCustomRuleFile()
{
  LOG_VARD(_customRuleFile);
  if (!_customRuleFile.trimmed().isEmpty())
  {
    QFile customRulesFile(_customRuleFile);
    if (!customRulesFile.open(QIODevice::ReadOnly))
    {
      throw HootException(
        QObject::tr("Error opening %1 for writing.").arg(customRulesFile.fileName()));
    }
    _customRulesList.clear();
    while (!customRulesFile.atEnd())
    {
      const QString line = QString::fromUtf8(customRulesFile.readLine().constData()).trimmed();
      if (!line.trimmed().isEmpty() && !line.startsWith("#"))
      {
        const QStringList lineParts = line.trimmed().split("\t");
        _customRulesList[lineParts[0].trimmed()] = lineParts[1].trimmed();
      }
    }
    customRulesFile.close();
  }
  LOG_VART(_customRulesList);
}

void ImplicitTagCustomRules::_readIgnoreList(const QString inputPath, QStringList& outputList)
{
  LOG_VARD(inputPath);
  if (!inputPath.trimmed().isEmpty())
  {
    QFile inputFile(inputPath);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
      throw HootException(
        QObject::tr("Error opening %1 for writing.").arg(inputFile.fileName()));
    }
    outputList.clear();
    while (!inputFile.atEnd())
    {
      const QString line = QString::fromUtf8(inputFile.readLine().constData()).trimmed();
      if (!line.trimmed().isEmpty() && !line.startsWith("#"))
      {
        outputList.append(line);
      }
    }
    inputFile.close();
  }
  LOG_VART(outputList);
}

void ImplicitTagCustomRules::_readAllowLists()
{
  LOG_DEBUG("Reading allow lists...");
  _readCustomRuleFile();
}

void ImplicitTagCustomRules::_readIgnoreLists()
{
  LOG_DEBUG("Reading ignore lists...");
  _readIgnoreList(_tagIgnoreFile, _tagIgnoreList);
  _readIgnoreList(_wordIgnoreFile, _wordIgnoreList);
}

}
