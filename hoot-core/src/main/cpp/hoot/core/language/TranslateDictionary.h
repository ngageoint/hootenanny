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

#ifndef TRANSLATE_DICTIONARY_H
#define TRANSLATE_DICTIONARY_H

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
 */
class TranslateDictionary
{
public:

  TranslateDictionary();
  ~TranslateDictionary();

  static TranslateDictionary& getInstance();

  const QMap<QString, QStringList>& getTable() { return _translations; }

  void load(QString path);

  Transliterator* getTransliterator() const { return _transliterator; }
  Transliterator* getTitler() const { return _titler; }

  bool getFromTransliterationCache(const QString originalText, QString& transliteratedText);
  void insertIntoTransliterationCache(const QString originalText, const QString transliteratedText);

  bool transliterationCachingEnabled() const { return _transliterationCachingEnabled; }

private:

  static boost::shared_ptr<TranslateDictionary> _theInstance;

  QMap<QString, QStringList> _translations;
  boost::shared_ptr<Tgs::LruCache<QString, QString>> _transliterationCache;
  Transliterator* _transliterator;
  Transliterator* _titler;
  bool _transliterationCachingEnabled;

  void _loadTags(boost::property_tree::ptree& tree);
};

}

#endif // DICTIONARY_TRANSLATOR_H
