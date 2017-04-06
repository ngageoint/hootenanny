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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MOSTENGLISHNAME_H
#define MOSTENGLISHNAME_H

// Hoot
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QSet>

// Tgs
#include <tgs/SharedPtr.h>

// Std
#include <string>

namespace hoot
{

using namespace std;

class MostEnglishName;
class Tags;

typedef boost::shared_ptr<MostEnglishName> MostEnglishNamePtr;

/**
 * Return a best guess at the "most english" name in the list. There are no guarantees. This is
 * an ad-hoc routine that should be better than taking the first choice in most cases.
 *
 * If there are no names then an empty string is returned.
 *
 * Logic:
 *  - Tokenize the string into words.
 *  - If the word is in the known dictionary (case insensitive) increase the strings's score by 1
 *    per letter in the word.
 *  - If the word is not in the known dictionary:
 *    - Increase the score by 0.5 for every latin letter.
 *    - Don't modify the score for every latin non-letter (e.g. punctuation and numbers)
 *    - Decrease the score by 0.5 for every non-latin character.
 */
class MostEnglishName : public Configurable
{
public:
  static string className() { return "hoot::MostEnglishName"; }

  static unsigned int logWarnCount;

  MostEnglishName();

  static const MostEnglishNamePtr& getInstance();

  QString getMostEnglishName(const Tags& tags);

  double scoreName(QString n);

  void setConfiguration(const Settings& conf);

private:
  static MostEnglishNamePtr _theInstance;

  QSet<QString> _englishWords;
  StringTokenizer _tokenizer;
  QStringList _wordPaths;
  bool _initialized;

  const QSet<QString>& _getWords();

  long _loadEnglishWords(QString path);

};

}

#endif // MOSTENGLISHNAME_H
