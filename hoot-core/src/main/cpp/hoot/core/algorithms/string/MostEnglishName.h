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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MOSTENGLISHNAME_H
#define MOSTENGLISHNAME_H

// Hoot
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QSet>

// Std
#include <string>

namespace hoot
{

class MostEnglishName;
class Tags;

using MostEnglishNamePtr = std::shared_ptr<MostEnglishName>;

/**
 * @brief The MostEnglishName class returns a best guess at the "most english" name in the list
 * (Singleton).
 *
 * There are no guarantees. This is an ad-hoc routine that should be better than taking the first
 * choice in most cases. If there are no names then an empty string is returned.
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

  static QString className() { return "hoot::MostEnglishName"; }

  static const MostEnglishNamePtr& getInstance();

  ~MostEnglishName() = default;

  void setConfiguration(const Settings& conf) override;

  /**
   * @brief getMostEnglishName returns the most English name tag value from a set of tags.
   * @param tags input to examine
   * @return the single name that has the highest resemblance to English text
   */
  QString getMostEnglishName(const Tags& tags);

  /**
   * @brief scoreName scores input as to how likely it is to be English text.
   * @param text input to examine
   * @return a score from 0.0 to 1.0 with 1.0 indicating the highest likelihood that the input is
   * English text
   */
  double scoreName(const QString& text);

  /**
   * @brief isInDictionary determines if a single input is in the English dictionary.
   * @param text input to examine
   * @return true if the input is in the English dictionary; false otherwise
   */
  bool isInDictionary(const QString& text);
  /**
   * @brief areAllInDictionary determines if all inputs are in the English dictionary.
   * @param texts input to examine
   * @return true if all text inputs are in the English dictionary; false otherwise
   */
  bool areAllInDictionary(const QStringList& texts);

  /**
   * @brief isEnglishText determines if the input is an English word.
   * @param text input to examine
   * @return true if the input is determined to be English text; false otherwise
   */
  bool isEnglishText(const QString& text);

private:

  static int logWarnCount;

  MostEnglishName();

  static MostEnglishNamePtr _theInstance;

  QSet<QString> _englishWords;
  StringTokenizer _tokenizer;
  QStringList _wordPaths;
  bool _initialized;

  const QSet<QString>& _getWords();

  long _loadEnglishWords(const QString& path);
};

}

#endif // MOSTENGLISHNAME_H
