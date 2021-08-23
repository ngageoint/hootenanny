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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ToEnglishDictionaryTranslator.h"

// Hoot
#include <hoot/core/conflate/address/Address.h>
#include <hoot/core/language/ToEnglishTranslateDictionary.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt



// Standard
#include <fstream>

using namespace std;

namespace hoot
{

QSet<QString> ToEnglishDictionaryTranslator::_streetTypes;

HOOT_FACTORY_REGISTER(ToEnglishTranslator, ToEnglishDictionaryTranslator)

ToEnglishDictionaryTranslator::ToEnglishDictionaryTranslator() :
_tokenizeInput(true)
{
  // if this assertion isn't true then bad things will happen when converting between QString and
  // UnicodeString
  assert(sizeof(UChar) == sizeof(QChar));
  _whiteSpace.setPattern("\\W+");

  _streetTypes = Address::getStreetTypes(false);
}

QString ToEnglishDictionaryTranslator::translate(const QString& textToTranslate)
{
  return toEnglish(textToTranslate, _tokenizeInput);
}

QString ToEnglishDictionaryTranslator::toEnglish(const QString& input, const bool tokenize) const
{
  LOG_TRACE("Translating: " << input << " to English...");

  //Since we have all our translations already in memory, caching translations isn't going to help
  //any here.

  QStringList l;
  if (tokenize)
  {
    l = input.split(_whiteSpace, QString::SkipEmptyParts);
  }
  else
  {
    l.append(input);
  }

  const QMap<QString, QStringList>& dict = ToEnglishTranslateDictionary::getInstance().getTable();

  for (int i = 0; i < l.size(); i++)
  {
    QString s = l[i].toLower();
    if (dict.contains(s))
    {
      LOG_TRACE("Found translation for " << s << " in dictionary.");
      l[i] = dict[s][0];
    }
    else
    {
      LOG_TRACE("Translation for " << s << " not found in dictionary.  Transliterating...");
      l[i] = transliterateToLatin(s);
    }
  }

  QString result = l.join(" ");
  LOG_TRACE("Translated: " << input << " to: " << result);
  return result;
}

QStringList ToEnglishDictionaryTranslator::toEnglishAll(const QString& input)
{
  QStringList l = input.split(_whiteSpace, QString::SkipEmptyParts);
  return toEnglishAll(l);
}

QStringList ToEnglishDictionaryTranslator::toEnglishAll(const QStringList& l)
{
  LOG_VART(l);
  QStringList result;
  if (l.empty())
  {
    return result;
  }

  const QMap<QString, QStringList>& dict = ToEnglishTranslateDictionary::getInstance().getTable();

  int biggestMatchSize = 0;
  QStringList biggestMatch;
  QString s = l[0].toLower();
  LOG_VART(s);
  // find the biggest list of consecutive words that match our dictionary
  for (QMap<QString, QStringList>::const_iterator it = dict.find(s); it != dict.constEnd(); ++it)
  {
    QStringList from = it.key().split(" ");
    LOG_VART(from);
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
    LOG_VART(match);

    // if this is a solid match
    if (match)
    {
      biggestMatchSize = from.size();
      biggestMatch = it.value();
    }
  }
  LOG_VART(biggestMatchSize);
  LOG_VART(biggestMatch);

  if (biggestMatchSize == 0)
  {
    biggestMatch.push_back(transliterateToLatin(s));
    biggestMatchSize = 1;
  }
  LOG_VART(biggestMatchSize);
  LOG_VART(biggestMatch);

  QStringList children = toEnglishAll(l.mid(biggestMatchSize));
  LOG_VART(children);
  if (children.empty())
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
  LOG_VART(result);

  return result;
}

QString ToEnglishDictionaryTranslator::toTitleCase(const QString& input) const
{
  return _transform(ToEnglishTranslateDictionary::getInstance().getTitler(), input);
}

QString ToEnglishDictionaryTranslator::translateStreet(const QString& input) const
{
  QStringList l = input.split(_whiteSpace, QString::SkipEmptyParts);

  const QMap<QString, QStringList>& dict = ToEnglishTranslateDictionary::getInstance().getTable();

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

  if (!l.empty() && _streetTypes.contains(l[0]))
  {
    QString first = l[0];
    l.removeFirst();
    l.append(first);
  }

  QString result = l.join(" ");

  return toTitleCase(result);
}

QString ToEnglishDictionaryTranslator::transliterateToLatin(const QString& input) const
{
  // cache incoming requests -- we sometimes get a lot of duplicates.
  QString result;
  if (!ToEnglishTranslateDictionary::getInstance().transliterationCachingEnabled())
  {
    result = _transform(ToEnglishTranslateDictionary::getInstance().getTransliterator(), input);
  }
  else if (!ToEnglishTranslateDictionary::getInstance().getFromTransliterationCache(input, result))
  {
    result = _transform(ToEnglishTranslateDictionary::getInstance().getTransliterator(), input);
    ToEnglishTranslateDictionary::getInstance().insertIntoTransliterationCache(input, result);
  }
  return result;
}

QString ToEnglishDictionaryTranslator::_transform(const shared_ptr<Transliterator>& t, const QString& input) const
{
  UnicodeString str((UChar*)input.constData(), input.size());

  t->transliterate(str);

  QString result((QChar*)str.getTerminatedBuffer(), str.length());
  LOG_TRACE("from: " << input.toUtf8().data() << " to " << result.toUtf8().data());

  return result;
}

}
