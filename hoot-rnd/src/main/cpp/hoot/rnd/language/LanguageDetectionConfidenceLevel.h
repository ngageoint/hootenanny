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
#ifndef LANGUAGE_DETECTION_CONFIDENCE_LEVEL_H
#define LANGUAGE_DETECTION_CONFIDENCE_LEVEL_H

// Qt
#include <QString>

namespace hoot
{

/**
 * The confidence attributed to a positive source language detection
 */
class LanguageDetectionConfidenceLevel
{
public:

  enum Level
  {
    None = 0,
    Low,
    Medium,
    High
  };

  LanguageDetectionConfidenceLevel();
  LanguageDetectionConfidenceLevel(Level level);

  /**
   * Determines whether two language detection confidence level are equal
   *
   * @param l the level to compare with
   * @return true if this level and the level passed in are equal to each other; false otherwise
   */
  bool operator==(LanguageDetectionConfidenceLevel l) const;

  Level getEnum() const { return _level; }

  /**
   * Returns a string respresentation of language detection confidence level
   *
   * @return a confidence level string
   */
  QString toString() const;

  /**
   * Creates a language detection confidence level from its string representation
   *
   * @param levelString a string representation of language detection confidence level
   * @return a language detection confidence level
   */
  static Level fromString(QString levelString);

private:

  LanguageDetectionConfidenceLevel::Level _level;
};

}

#endif // LANGUAGE_DETECTION_CONFIDENCE_LEVEL_H
