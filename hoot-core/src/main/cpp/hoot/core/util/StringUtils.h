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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace hoot
{

/**
 * General utilities needed when working with strings
 */
class StringUtils
{

public:

  /**
   * Converts seconds to DD:MM:SS
   *
   * @param durationInMilliseconds seconds to convert
   * @return a DD:MM:SS string
   */
  static QString millisecondsToDhms(const qint64 durationInMilliseconds);
  /**
   * Converts a large number to a more human readable format
   *
   * @param number the number to format
   * @return a formatted number string
   */
  static QString formatLargeNumber(const unsigned long number);
  /**
   * Pads the front of a number string with zeroes
   *
   * @param number the number to pad
   * @param padSize the number of zeroes to add
   * @return a padded number string
   */
  static QString padFrontOfNumberStringWithZeroes(const int number, const int padSize);

  /**
   * Determines whether a string contains an alphabetic character
   *
   * @param input string to examine
   * @return true if the input has at least one alphabetic character; false otherwise
   */
  static bool hasAlphabeticCharacter(const QString& input);
  /**
   * Determines whether a string contains a number
   *
   * @param input string to examine
   * @return true if the input has at least one number; false otherwise
   */
  static bool hasDigit(const QString& input);
  /**
   * Determines if text is a number
   *
   * @param input text to examine
   * @return true if the input is a number; false otherwise
   * @note only implemented for long int
   */
  static bool isNumber(const QString& input);
  /**
   * Determines if a string is made up of entirely alphanumeric characters
   *
   * @param input string to examine
   * @return true if the string contains only alphanumeric characters; false otherwise
   */
  static bool isAlphaNumeric(const QString& input);

  /**
   * Converts a JSON string to a Boost property tree
   *
   * @param jsonStr the string to convert
   * @return a Boost property tree
   */
  static std::shared_ptr<boost::property_tree::ptree> jsonStringToPropTree(const QString& jsonStr);
  /**
   * Creates a JSON array from a list of strings
   *
   * @param stringList a list of strings
   * @return a Boost JSON property tree containing a string array
   */
  static std::shared_ptr<boost::property_tree::ptree> stringListToJsonArray(
    const QStringList& stringList);

  /**
   * Finds duplicate strings in a list
   *
   * @param input the list to search
   * @return a collection of duplicated strings
   */
  static QSet<QString> getDuplicates(const QStringList& input);
  /**
   * @brief hasDuplicates Determines if duplicate strings are in a list of strings
   * @param input the list to check for duplicates
   * @return true if the list contains at least one duplicate; false otherwise
   */
  static bool hasDuplicates(const QStringList& input);

  /**
   * Determines whether any string in a list of strings contains a substring
   *
   * @param input a list of strings to examine
   * @param substring the substring to search for
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if any string in the input list contains the given substring; false otherwise
   */
  static bool containsSubstring(
    const QStringList& input, const QString& substring,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Determines whether any string in a list of strings contains a substring
   *
   * @param input a list of strings to examine
   * @param substrings a list of substrings to search for
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if any string in the input list contains any string in the given substrings;
   * false otherwise
   */
  static bool containsSubstrings(
    const QStringList& input, const QStringList& substrings,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Returns the first index of a substring in a collection of input strings
   *
   * @param input a list of strings to examine
   * @param substring the substring to search for
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return an index if the substring in the input list is found; -1 otherwise
   */
  static int indexOfSubstring(
    const QStringList& input, const QString& substring,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Determines if a list contains any strings from another list
   *
   * @param input list to search
   * @param toCompare list to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if at least one match is found; false otherwise
   */
  static bool containsAny(
    const QStringList& input, const QStringList& compareList,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Determines if a string matches any of a collection of wildcard strings
   *
   * @param str the string to match
   * @param wildcards the wildcard strings to match against
   * @return true if at least one match is found; false otherwise
   */
  static bool matchesWildcard(const QString& str, const QStringList& wildcards);

  /**
   * Determines if a string ends with any one of a group of strings
   *
   * @param input the string to examine
   * @param compareList list to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if the input ends with any string in the compare list; false otherwise
   */
  static bool endsWithAny(
    const QString& input, const QStringList& compareList,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Determines if any one of a group of strings ends with a single string
   *
   * @param inputs the strings to examine
   * @param compareStr string to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if the input ends with any string in the compare list; false otherwise
   */
  static bool endsWithAny(
    const QStringList& inputs, const QString& compareStr,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Returns the first string at the end of the input that matches any one of a group of strings
   *
   * @param input the string to examine
   * @param compareList list to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return the first matching string found at the end of the input or an empty string if no match
   * was found
   */
  static QString endsWithAnyAsStr(
    const QString& input, const QStringList& compareList,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Determines if a string contains a matching instance of any one of a group of regular
   * expressions which bisects it
   *
   * @param input the string to examine
   * @param compareList list of regular expressions to match with
   * @return true if the input has a bisecting token matching any regular expression in the compare
   * list; false otherwise
   */
  static bool bisectsAny(const QString& input, const QList<QRegExp>& compareList);

  /**
   * @brief removeAllContaining Removes all strings from a list containing a particular text
   * @param input the list to remove strings from
   * @param text the text to match in the list
   * @param caseSensitivity determines whether comparisons are case sensitive
   */
  static void removeAllContaining(
    QStringList& input, const QString& text,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Removes the last occurrence of a group of strings from a single string
   *
   * @param input the string to remove from
   * @param toRemove the tokens to remove
   * @param caseSensitivity determines whether comparisons are case sensitive
   */
  static void removeLastIndexOf(
    QString& input, const QStringList& toRemove,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);
  /**
   * Removes all empty/blank strings from a list
   *
   * @param strings the strings to modify
   */
  static void removeEmptyStrings(QStringList& strings);
  /**
   * Removes matching prefixes from a collection of strings
   *
   * @param prefix the prefix to remove from the strings
   * @param input the collection of strings to modify
   * @return the number of prefixes removed
   */
  static int removePrefixes(const QString& prefix, QStringList& input);

  /**
   * Replaces the last occurrence of a string with another string
   *
   * @param input the string to modify
   * @param strToReplace the token to replace in the string
   * @param replacementStr the token to replace with
   * @param caseSensitivity determines whether comparisons are case sensitive
   */
  static void replaceLastIndexOf(
    QString& input, const QString& strToReplace, const QString& replacementStr,
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * @brief insertAfter inserts a specified string after another string in a list
   * @param strList the list to modify
   * @param strToInsertAfter the string to insert strToInsert after in the list
   * @param strToInsert the string to insert after strToInsertAfter in the list
   * @return true if the string was inserted; false otherwise
   */
  static bool insertAfter(
    QStringList& strList, const QString& strToInsertAfter, const QString& strToInsert);

  /**
   * Splits a string on the first matching token in a group of regular expressions and returns it
   * only if is of a specified size
   *
   * @param input string to split
   * @param tokenList list of regular expression to match with
   * @param numOutputTokens the split output list must contain this number of strings
   * @return a collection of strings
   */
  static QStringList splitOnAny(
    const QString& input, const QList<QRegExp>& tokenList, const int numOutputTokens);
  /**
   * Splits a string on a regular expression and removes the token found at the specified index
   *
   * @param input the string to remove from; the string remains unmodified if the string split parts
   * have no entry at the specified index
   * @param splitExp the regular expression to split the input string on
   * @param index the index at which to remove from the split string parts
   */
  static void splitAndRemoveAtIndex(QString& input, const QRegExp& splitExp, const int index);

  /**
   * Converts a set to a delimited string
   *
   * @param set the set to generate a string for
   * @return a string
   * @todo need to genericize this to work for all container types
   */
  template<typename T>
  static QString setToString(const std::set<T>& set)
  {
    QString out;
    for (typename std::set<T>::const_iterator itr = set.begin(); itr != set.end(); ++itr)
    {
      out += (*itr).toString() + ",";
    }
    out.chop(1);
    return out;
  }

  /**
   * @brief setToString converts a string set to a delimited string
   * @param set the set to generate a string for
   * @return a string
   */
  static QString setToString(const std::set<QString>& set)
  {
    QString out;
    for (std::set<QString>::const_iterator itr = set.begin(); itr != set.end(); ++itr)
    {
      out += (*itr) + ",";
    }
    out.chop(1);
    return out;
  }

private:

  static QString _splitAndRemoveAtIndex(
    QStringList& input, const int index, const QString& separator);
};

}

#endif // STRINGUTILS_H
