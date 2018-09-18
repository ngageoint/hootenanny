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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */


#ifndef TO_ENGLISH_TRANSLATOR_H
#define TO_ENGLISH_TRANSLATOR_H

// Hoot
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Interface for a to English language translator
 */
class ToEnglishTranslator : public Configurable
{
public:

  static std::string className() { return "hoot::ToEnglishTranslator"; }

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
  virtual void setSourceLanguages(const QStringList langCodes) = 0;

  /**
   * Translates text to English
   *
   * @param textToTranslate the text to translate
   */
  virtual void translate(const QString textToTranslate) = 0;

  /**
   * Returns text that has been translated to English by translate
   *
   * @return translated text
   */
  virtual QString getTranslatedText() const = 0;

  /**
   * Determines if a language detection was made by this translator before translation
   *
   * @return true if a language detection was made; false otherwise
   */
  virtual bool detectionMade() const = 0;
};

}

#endif // TO_ENGLISH_TRANSLATOR_H
