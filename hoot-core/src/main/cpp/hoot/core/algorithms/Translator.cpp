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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Translator.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// ICU
#include <unicode/utypes.h>
#include <unicode/uobject.h>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

// Qt
#include <QMap>
#include <QRegExp>
#include <QStringList>

// Standard
#include <fstream>
#include <iostream>
using namespace std;

namespace hoot
{
using namespace std;

/**
 * JSON Spirit isn't a very well behaved header file so I'm only using it within the CPP file.
 * This avoids some conflicts with Boost.
 */
class JsonDictionary
{
public:
  JsonDictionary()
  {
  }

  const QMap<QString, QStringList>& getTable() { return _translations; }

  void load(QString path)
  {
    try
    {
      pt::ptree pt;
      pt::read_json(path.toAscii().data(), pt);

      _loadTags(pt);
    }
    catch (std::exception e)
    {
      QString reason = e.what();
      throw HootException("Error parsing JSON. " + reason);
    }
  }

private:
  QMap<QString, QStringList> _translations;

  void _loadTags(pt::ptree& tree)
  {
    BOOST_FOREACH(pt::ptree::value_type& translation, tree.get_child("Dictionary"))
    {
      int i = 0;
      QString from;
      BOOST_FOREACH(pt::ptree::value_type& t, translation.second.get_child(""))
      {
        string s = t.second.data();
        if (i == 0)
          from = QString::fromUtf8(s.c_str()).toLower();
        else
          _translations[from].push_back(QString::fromUtf8(s.c_str()).toLower());
        i++;
      }
    }
  }
};



Translator* Translator::_theInstance = 0;

Translator::Translator()
{
  // if this assertion isn't true then bad things will happen when converting between QString and
  // UnicodeString
  assert(sizeof(UChar) == sizeof(QChar));
  _transliterator = 0;
  _titler = 0;
  _whiteSpace.setPattern("\\W+");
}

Translator::~Translator()
{
  delete _transliterator;
  delete _titler;
}

Translator& Translator::getInstance()
{
  if (_theInstance == 0)
  {
    QString dictionary = ConfPath::search("dictionary.json");

    _theInstance = new Translator();
    _theInstance->_bufferLength = 1024;
    _theInstance->_buffer = new char[_theInstance->_bufferLength + 1];
    _theInstance->_dictionary = new JsonDictionary();
    _theInstance->_dictionary->load(dictionary);

    _theInstance->_streetTypes.insert("street");
    _theInstance->_streetTypes.insert("lane");
    _theInstance->_streetTypes.insert("boulevard");
    _theInstance->_streetTypes.insert("highway");
    _theInstance->_streetTypes.insert("freeway");
    _theInstance->_streetTypes.insert("tollway");
    _theInstance->_streetTypes.insert("road");
  }
  return *_theInstance;
}

QString Translator::toEnglish(const QString& input)
{
  QStringList l = input.split(QRegExp("\\W+"), QString::SkipEmptyParts);

  const QMap<QString, QStringList>& dict = _dictionary->getTable();

  for (int i = 0; i < l.size(); i++)
  {
    QString s = l[i].toLower();
    if (dict.contains(s))
    {
      l[i] = dict[s][0];
    }
    else
    {
      l[i] = transliterateToLatin(s);
    }
  }

  QString result = l.join(" ");

  return result;
}

QStringList Translator::toEnglishAll(const QString& input)
{
  QStringList l = input.split(_whiteSpace, QString::SkipEmptyParts);
  return toEnglishAll(l);
}

QStringList Translator::toEnglishAll(const QStringList& l)
{
  QStringList result;
  if (l.size() == 0)
  {
    return result;
  }

  const QMap<QString, QStringList>& dict = _dictionary->getTable();

  int biggestMatchSize = 0;
  QStringList biggestMatch;
  QString s = l[0].toLower();
  // find the biggest list of consecutive words that match our dictionary
  for (QMap<QString, QStringList>::const_iterator it = dict.find(s); it != dict.constEnd(); it++)
  {
    QStringList from = it.key().split(" ");
    // if this no longer starts with our first word.
    if (from[0] != s)
    {
      break;
    }

    // determine if from matches our list of strings
    bool match = from.size() <= l.size();
    if (from.size() > biggestMatchSize)
    {
      for (int i = 0; i < from.size() && i < l.size(); i++)
      {
        if (from[i] != l[i].toLower())
        {
          match = false;
        }
      }
    }

    // if this is a solid match
    if (match)
    {
      biggestMatchSize = from.size();
      biggestMatch = it.value();
    }
  }

  if (biggestMatchSize == 0)
  {
    biggestMatch.push_back(transliterateToLatin(s));
    biggestMatchSize = 1;
  }

  QStringList children = toEnglishAll(l.mid(biggestMatchSize));
  if (children.size() == 0)
  {
    result = biggestMatch;
  }
  else
  {
    for (int i = 0; i < biggestMatch.size(); i++)
    {
      for (int j = 0; j < children.size(); j++)
      {
        if (children[j].size() > 0)
        {
          result.push_back(biggestMatch[i] + " " + children[j]);
        }
        else
        {
          result.push_back(biggestMatch[i]);
        }
      }
    }
  }

  return result;
}

QString Translator::toTitleCase(const QString& input)
{
  if (_titler == 0)
  {
    UErrorCode error = U_ZERO_ERROR;
    _titler = Transliterator::createInstance("Any-Title", UTRANS_FORWARD, error);
    if (_titler == NULL || error != U_ZERO_ERROR)
    {
      LOG_ERROR("transliterator error code: " << error);
      throw HootException("transliterator error");
    }
  }

  return _transform(_titler, input);
}

QString Translator::translateStreet(const QString& input)
{
  QStringList l = input.split(QRegExp("\\W+"), QString::SkipEmptyParts);

  const QMap<QString, QStringList>& dict = _dictionary->getTable();

  for (int i = 0; i < l.size(); i++)
  {
    QString s = l[i].toLower();
    if (dict.contains(s))
    {
      l[i] = dict[s][0];
    }
    else
    {
      l[i] = transliterateToLatin(s);
    }
  }

  if (l.size() > 0 && _streetTypes.contains(l[0]))
  {
    QString first = l[0];
    l.removeFirst();
    l.append(first);
  }

  QString result = l.join(" ");

  return toTitleCase(result);
}

QString Translator::transliterateToLatin(const QString& input)
{
  if (_transliterator == 0)
  {
    UErrorCode error = U_ZERO_ERROR;
    _transliterator = Transliterator::createInstance("Any-Latin; Latin-ASCII", UTRANS_FORWARD, error);
    if (_transliterator == NULL || error != U_ZERO_ERROR)
    {
      LOG_ERROR("transliterator error code: " << error);
      throw HootException("transliterator error");
    }
  }

  // cache incoming requests -- we sometimes get a lot of duplicates.
  QString result;
  if (!_cache.get(input, result))
  {
    result = _transform(_transliterator, input);
    _cache.insert(input, result);
  }

  return result;
}

QString Translator::_transform(Transliterator* t, const QString& input)
{
  UnicodeString str((UChar*)input.constData(), input.size());

  t->transliterate(str);

  QString result((QChar*)str.getTerminatedBuffer(), str.length());
  LOG_TRACE("from: " << input.toUtf8().data() << " to " << result.toUtf8().data());

  return result;
}

}
