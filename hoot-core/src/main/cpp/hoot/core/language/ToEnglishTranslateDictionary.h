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
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TO_ENGLISH_TRANSLATE_DICTIONARY_H
#define TO_ENGLISH_TRANSLATE_DICTIONARY_H

// Qt
#include <QString>
#include <QMap>

// Boost
#include <boost/property_tree/ptree.hpp>

// ICU
#include <unicode/translit.h>

// Tgs
#include <tgs/LruCache.h>

namespace hoot
{

/**
 * (Singleton)
 */
class ToEnglishTranslateDictionary
{
public:

  static ToEnglishTranslateDictionary& getInstance();

  const QMap<QString, QStringList>& getTable() const { return _translations; }

  void load(const QString& path);

  std::shared_ptr<Transliterator> getTransliterator() const { return _transliterator; }
  std::shared_ptr<Transliterator> getTitler() const { return _titler; }

  bool getFromTransliterationCache(const QString& originalText, QString& transliteratedText) const;
  void insertIntoTransliterationCache(const QString& originalText, const QString& transliteratedText) const;

  bool transliterationCachingEnabled() const { return _transliterationCachingEnabled; }

private:

  void _loadTags(boost::property_tree::ptree& tree);

  QMap<QString, QStringList> _translations;
  std::shared_ptr<Tgs::LruCache<QString, QString>> _transliterationCache;
  std::shared_ptr<Transliterator> _transliterator;
  std::shared_ptr<Transliterator> _titler;
  bool _transliterationCachingEnabled;

  ToEnglishTranslateDictionary();
  /** Default destructor */
  ~ToEnglishTranslateDictionary() = default;
  /** Delete copy constructor and assignment operator */
  ToEnglishTranslateDictionary(const ToEnglishTranslateDictionary&) = delete;
  ToEnglishTranslateDictionary& operator=(const ToEnglishTranslateDictionary&) = delete;

};

}

#endif // TO_ENGLISH_TRANSLATE_DICTIONARY_H
