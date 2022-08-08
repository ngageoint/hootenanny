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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "LanguageUtils.h"

// hoot
#include <hoot/core/algorithms/string/MostEnglishName.h>

namespace hoot
{

bool LanguageUtils::normalizeAndDetermineIfTextIsEnglish(const QString& text)
{
  const QString textToCheck =
    text
      .simplified()
      .replace("&", "and"); //will likely need some additions to this
  const QStringList tokens = textToCheck.split(" ");
  LOG_VART(tokens);
  int englishTagValuePartCount = 0;
  for (int i = 0; i < tokens.length(); i++)
  {
    LOG_VART(tokens.at(i));
    if (MostEnglishName::getInstance()->isEnglishText(tokens.at(i)))
    {
      englishTagValuePartCount++;
    }
  }
  LOG_VART(englishTagValuePartCount);
  LOG_VART(tokens.size());
  return englishTagValuePartCount == tokens.size();
}

}
