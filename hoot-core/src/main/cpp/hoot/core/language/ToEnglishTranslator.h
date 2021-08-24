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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TO_ENGLISH_TRANSLATOR_H
#define TO_ENGLISH_TRANSLATOR_H

// Hoot
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Interface for a to English language translator
 */
class ToEnglishTranslator : public Configurable
{
public:

  static QString className() { return "hoot::ToEnglishTranslator"; }

  ToEnglishTranslator() = default;
  virtual ~ToEnglishTranslator() = default;

  /**
   * Returns the translators source languages
   *
   * @return list of ISO-639-1 language codes
   */
  virtual QStringList getSourceLanguages() const = 0;

  /**
   * Set the source language for the translator
   *
   * @param langCode an ISO-639-1 language code
   */
  virtual void setSourceLanguages(const QStringList& langCodes) = 0;

  /**
   * Translates text to English
   *
   * @param text the text to translate
   * @return translated text
   */
  virtual QString translate(const QString& text) = 0;

  /**
   * Returns the language detected by the translator if language detection was performed
   *
   * @return a language name if a language was detected; an empty string otherwise
   */
  virtual QString getDetectedLanguage() const = 0;

  /**
   * Sets a unique ID for the translator; useful when many callers use different translator
   * instantiations during the same execution
   *
   * @param id
   */
  virtual void setId(const QString& id) = 0;
};

}

#endif // TO_ENGLISH_TRANSLATOR_H
