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
 * @copyright Copyright (C) 2015, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DICTIONARY_TRANSLATOR_H
#define DICTIONARY_TRANSLATOR_H

// Hoot
#include "ToEnglishTranslator.h"

// Qt
#include <QRegExp>
#include <QSet>
#include <QString>

// ICU
#include <unicode/translit.h>

// Tgs
#include <tgs/LruCache.h>

namespace hoot
{

class JsonDictionary;

/**
 * This makes a reasonable attempt to translate or transliterate common map words in and pronouns
 * into english/latin characters. This is by no means an exhaustive translator and does not have
 * any concept of grammar. If the word is unknown it is assumed to be a pronoun and will be
 * transliterated into latin characters.
 */
class DictionaryTranslator : public ToEnglishTranslator
{
public:

  static std::string className() { return "hoot::DictionaryTranslator"; }

  DictionaryTranslator();
  ~DictionaryTranslator();

  static DictionaryTranslator& getInstance();

  /**
   * Translates the given input string into a translation & transliteration of the input.
   */
  QString toEnglish(const QString& input);

  /**
   * @see ToEnglishTranslator; wraps call to toEnglish
   */
  virtual QString translate(const QString textToTranslate);

  /**
   * Converts the given input string into all possible known translations. E.g.
   * using the dictionary as of 5/16/2013, "rumah sakit umum" would return:
   * { "home sakit umum",
   *   "house sakit umum",
   *   "hospital umum",
   *   "general hospital" }
   */
  QStringList toEnglishAll(const QString& input);

  /**
   * Similar to above, but the input has already been tokenized.
   */
  QStringList toEnglishAll(const QStringList& l);

  QString toTitleCase(const QString& input);

  QString transliterateToLatin(const QString& input);

  QString translateStreet(const QString& input);

  virtual QStringList getSourceLanguages() const { return QStringList(); }
  virtual void setSourceLanguages(const QStringList /*langCodes*/) {}
  virtual bool detectionMade() const { return false; }

  virtual void setConfiguration(const Settings& /*conf*/) {}

private:

  char* _buffer;
  int _bufferLength;
  Tgs::LruCache<QString, QString> _cache;
  static boost::shared_ptr<DictionaryTranslator> _theInstance;
  boost::shared_ptr<JsonDictionary> _dictionary;
  QSet<QString> _streetTypes;
  Transliterator* _transliterator;
  Transliterator* _titler;
  QRegExp _whiteSpace;

  QString _transform(icu::Transliterator* t, const QString& input) const;
};

}

#endif // DICTIONARY_TRANSLATOR_H
