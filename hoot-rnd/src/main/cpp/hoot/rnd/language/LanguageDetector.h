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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */


#ifndef LANGUAGE_DETECTOR_H
#define LANGUAGE_DETECTOR_H

// Hoot
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Detects a source language, given text
 */
class LanguageDetector : public Configurable
{
public:

  static QString className() { return "hoot::LanguageDetector"; }

  LanguageDetector() = default;
  virtual ~LanguageDetector() = default;

  /**
   * Detects the source language of text
   *
   * @param text text to detect the language of
   * @return an ISO-6391- language code
   */
  virtual QString detect(const QString& text) = 0;
};

}

#endif // LANGUAGE_DETECTOR_H
