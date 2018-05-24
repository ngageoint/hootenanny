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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ImplicitTagUtils.h"

// Hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/algorithms/Translator.h>

namespace hoot
{

void ImplicitTagUtils::cleanName(QString& name)
{
  if (name.startsWith("-"))
  {
    name = name.replace(0, 1, "");
  }
  name =
    name.replace("(", "").replace(")", "").replace(".", "").replace("/", " ").replace("<", "")
        .replace(">", "").replace("[", "").replace("]", "").replace("@", "").replace("&", "and")
        .replace("(historical)", "").replace("-", " ");
  if (name.startsWith("_"))
  {
    name = name.replace(0, 1, "");
  }

  //another possibility here might be to replace name multiple spaces with one

  //This needs to be expanded.
  if (!name.isEmpty() && name.at(0).isDigit() &&
      (name.endsWith("th") || name.endsWith("nd") || name.endsWith("rd") ||
       name.endsWith("ave") || name.endsWith("avenue") || name.endsWith("st") ||
       name.endsWith("street") || name.endsWith("pl") || name.endsWith("plaza")))
  {
    name = "";
  }
}

QStringList ImplicitTagUtils::translateNamesToEnglish(const QStringList names, const Tags& tags)
{
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
        const QString englishName = Translator::getInstance().toEnglish(name);
        LOG_VART(englishName);
        filteredNames.append(englishName);
        break;
      }
    }
    if (filteredNames.isEmpty() && !altName.isEmpty())
    {
      if (altName.contains(";"))
      {
        altName = altName.split(";")[0];
      }
      LOG_VART(altName);
      const QString englishName = Translator::getInstance().toEnglish(altName);
      LOG_VART(englishName);
      filteredNames.append(englishName);
    }
  }
  LOG_VART(filteredNames);

  return filteredNames;
}

}
