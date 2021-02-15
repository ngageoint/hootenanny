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
 * @copyright Copyright (C) 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "LanguageDetectionConfidenceLevel.h"

// Hoot
#include <hoot/core/util/HootException.h>

namespace hoot
{

LanguageDetectionConfidenceLevel::LanguageDetectionConfidenceLevel() :
_level(Level::None)
{
}

LanguageDetectionConfidenceLevel::LanguageDetectionConfidenceLevel(Level level) :
_level(level)
{
}

bool LanguageDetectionConfidenceLevel::operator==(LanguageDetectionConfidenceLevel l) const
{
  return l._level == _level;
}

QString LanguageDetectionConfidenceLevel::toString() const
{
  switch (_level)
  {
  case LanguageDetectionConfidenceLevel::None:
    return "none";
  case LanguageDetectionConfidenceLevel::Low:
    return "low";
  case LanguageDetectionConfidenceLevel::Medium:
    return "medium";
  case LanguageDetectionConfidenceLevel::High:
    return "high";
  default:
    return QString("Unknown (%1)").arg(_level);
  }
}

LanguageDetectionConfidenceLevel::Level LanguageDetectionConfidenceLevel::fromString(
  QString levelString)
{
  levelString = levelString.toLower();
  if (levelString == "none")
  {
    return None;
  }
  else if (levelString == "low")
  {
    return Low;
  }
  else if (levelString == "medium")
  {
    return Medium;
  }
  else if (levelString == "high")
  {
    return High;
  }
  else
  {
    throw HootException("Invalid language detection confidence string: " + levelString);
  }
}

}
