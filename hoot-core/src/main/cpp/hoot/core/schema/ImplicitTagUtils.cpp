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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ImplicitTagUtils.h"

// Hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

QStringList ImplicitTagUtils::_nameCleaningTokens;
QStringList ImplicitTagUtils::_streetTypes;

bool ImplicitTagUtils::sortCommandExists()
{
  const QString cmd = "sort --help > /dev/null";
  const int cmdExitStatus = std::system(cmd.toStdString().c_str());
  return cmdExitStatus == 0;
}

void ImplicitTagUtils::cleanName(QString& name)
{
  name = name.simplified();
  if (name.startsWith("-"))
  {
    name = name.replace(0, 1, "");
  }
  _modifyUndesirableTokens(name);
  if (name.startsWith("_"))
  {
    name = name.replace(0, 1, "");
  }

  _filterOutStreets(name);
}

void ImplicitTagUtils::_modifyUndesirableTokens(QString& name)
{
  // This assumes these file will be populated with at least one entry.  Having the option of
  // disabling use of these files completely would require different logic.
  if (_nameCleaningTokens.isEmpty())
  {
    _nameCleaningTokens =
      FileUtils::readFileToList(ConfigOptions().getImplicitTaggingNameCleaningTokensFile());
  }

  for (int i = 0; i < _nameCleaningTokens.size(); i++)
  {
    const QString replacementEntry = _nameCleaningTokens.at(i);
    const QStringList replacementEntryParts = replacementEntry.split("\t");
    if (replacementEntryParts.size() != 2)
    {
      throw HootException(
        "Invalid implicit tag rules name cleaning token entry: " + replacementEntry);
    }
    const QString replaceText = replacementEntryParts.at(1);
    if (replaceText.trimmed().isEmpty())
    {
      throw HootException(
          "Empty text specified for implicit tag rules name cleaning token entry.");
    }
    else if (replaceText == "e")
    {
      name = name.replace(replacementEntryParts.at(0), "");
    }
    else if (replaceText == "s")
    {
      name = name.replace(replacementEntryParts.at(0), " ");
    }
    else
    {
      name = name.replace(replacementEntryParts.at(0), replaceText);
    }
  }
}

void ImplicitTagUtils::_filterOutStreets(QString& name)
{
  if (name.isEmpty() || !name.at(0).isDigit())
  {
    return;
  }
  // TODO: This one seems kind of awkward and probably needs to be rethought or expanded somehow...
  //apparently its to catch address parts like "2nd" or "3rd"
  else if (name.endsWith("th") || name.endsWith("nd"))
  {
    name = "";
  }
  else
  {
    // see related note in _modifyUndesirableTokens
    if (_streetTypes.isEmpty())
    {
      _streetTypes = FileUtils::readFileToList(ConfigOptions().getStreetTypesFile());
    }

    // This list could be expanded.  See the note in the associated config file.
    for (int i = 0; i < _streetTypes.size(); i++)
    {
      const QString streetTypeEntry = _streetTypes.at(i);
      const QStringList streetTypeEntryParts = streetTypeEntry.split("\t");
      if (streetTypeEntryParts.size() != 2)
      {
        throw HootException("Invalid street type entry: " + streetTypeEntry);
      }
      if (name.endsWith(streetTypeEntryParts.at(0)) || name.endsWith(streetTypeEntryParts.at(1)))
      {
        name = "";
        break;
      }
    }
  }
}

QStringList ImplicitTagUtils::translateNamesToEnglish(const QStringList names, const Tags& tags,
                                                      std::shared_ptr<ToEnglishTranslator> translator)
{
  LOG_VART(translator.get());
  QStringList filteredNames;
  if (tags.contains("name:en"))
  {
    filteredNames.append(tags.get("name:en"));
  }
  else
  {
    QString altName = tags.get("alt_name");
    LOG_VART(altName);
    for (int i = 0; i < names.size(); i++)
    {
      const QString name = names.at(i);
      LOG_VART(name);
      if (name != altName)
      {
        const QString englishName = translator->translate(name);
        LOG_VART(englishName);
        if (!englishName.isEmpty())
        {
          filteredNames.append(englishName);
        }
        else
        {
          filteredNames.append(name);
        }
        break;
      }
    }

    LOG_VART(filteredNames.size());
    if (filteredNames.isEmpty() && !altName.isEmpty())
    {
      if (altName.contains(";"))
      {
        altName = altName.split(";")[0];
      }
      LOG_VART(altName);
      const QString englishName = translator->translate(altName);
      LOG_VART(englishName);
      if (!englishName.isEmpty())
      {
        filteredNames.append(englishName);
      }
      else
      {
        filteredNames.append(altName);
      }
    }
  }
  LOG_VART(filteredNames.size());
  LOG_VART(filteredNames);

  return filteredNames;
}

}
