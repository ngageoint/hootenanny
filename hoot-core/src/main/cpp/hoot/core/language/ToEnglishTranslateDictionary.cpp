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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

#include "ToEnglishTranslateDictionary.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// ICU
#include <unicode/utypes.h>
#include <unicode/uobject.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

namespace hoot
{

ToEnglishTranslateDictionary::ToEnglishTranslateDictionary() :
_transliterationCachingEnabled(false)
{
  if (ConfigOptions().getTransliterationMaxCacheSize() != -1)
  {
    _transliterationCache.reset(
      new Tgs::LruCache<QString, QString>(ConfigOptions().getTransliterationMaxCacheSize()));
    _transliterationCachingEnabled = true;
  }
  LOG_VARD(_transliterationCachingEnabled);

  UErrorCode error = U_ZERO_ERROR;

  _titler.reset(Transliterator::createInstance("Any-Title", UTRANS_FORWARD, error));
  if (_titler == NULL || error != U_ZERO_ERROR)
  {
    LOG_ERROR("transliterator error code: " << error);
    throw HootException("transliterator error");
  }

  _transliterator.reset(
    Transliterator::createInstance("Any-Latin; Latin-ASCII", UTRANS_FORWARD, error));
  if (_transliterator == NULL || error != U_ZERO_ERROR)
  {
    LOG_ERROR("transliterator error code: " << error);
    throw HootException("transliterator error");
  }
  //  Load the dictionary file
  load(ConfPath::search("dictionary.json"));
}

ToEnglishTranslateDictionary& ToEnglishTranslateDictionary::getInstance()
{
  //  Local static singleton instance
  static ToEnglishTranslateDictionary instance;
  return instance;
}

bool ToEnglishTranslateDictionary::getFromTransliterationCache(const QString& originalText,
                                                      QString& transliteratedText)
{
  return _transliterationCache->get(originalText, transliteratedText);
}

void ToEnglishTranslateDictionary::insertIntoTransliterationCache(const QString& originalText,
                                                         const QString& transliteratedText)
{
  _transliterationCache->insert(originalText, transliteratedText);
}

void ToEnglishTranslateDictionary::load(const QString& path)
{
  try
  {
    pt::ptree pt;
    pt::read_json(path.toLatin1().data(), pt);

    _loadTags(pt);
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON. " + reason);
  }
}

void ToEnglishTranslateDictionary::_loadTags(pt::ptree& tree)
{
  for (pt::ptree::value_type& translation : tree.get_child("Dictionary"))
  {
    int i = 0;
    QString from;
    for (pt::ptree::value_type& t : translation.second.get_child(""))
    {
      std::string s = t.second.data();
      if (i == 0)
        from = QString::fromUtf8(s.c_str()).toLower();
      else
        _translations[from].push_back(QString::fromUtf8(s.c_str()).toLower());
      i++;
    }
  }
}

}
