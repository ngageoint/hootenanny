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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

// Qt
#include <QStringList>
#include <QSet>

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
  static std::shared_ptr<boost::property_tree::ptree> stringListToJsonStringArray(
    const QStringList& stringList);

  /**
   * Determines if text is a number
   *
   * @param input text to examine
   * @return true if the input is a number; false otherwise
   * @note only implemented for long int
   */
  static bool isNumber(const QString& input);

  /**
   * Pads the front of a number string with zeroes
   *
   * @param number the number to pad
   * @param padSize the number of zeroes to add
   * @return a padded number string
   */
  static QString padFrontOfNumberStringWithZeroes(const int number, const int padSize);

  /**
   * Removes all empty/blank strings from a list
   *
   * @param strings the strings to modify
   */
  static void removeEmptyStrings(QStringList& strings);

  /**
   * Determines if a string is made up of entirely alphanumeric characters
   *
   * @param input string to examine
   * @return true if the string contains only alphanumeric characters; false otherwise
   */
  static bool isAlphaNumeric(const QString& input);

  /**
   * Finds duplicate strings in a list
   *
   * @param input the list to search
   * @return a collection of duplicated strings
   */
  static QSet<QString> getDuplicates(const QStringList& input);

  /**
   * Determines whether any string in a list of strings contains a substring
   *
   * @param input a list of strings to examine
   * @param substring the substring to search for
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if any string in the input list contains the given substring; false otherwise
   */
  static bool containsSubstring(const QStringList& input, const QString& substring,
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
  static bool containsSubstrings(const QStringList& input, const QStringList& substrings,
                                 Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Returns the first index of a substring in a collection of input strings
   *
   * @param input a list of strings to examine
   * @param substring the substring to search for
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return an index if the substring in the input list is found; -1 otherwise
   */
  static int indexOfSubstring(const QStringList& input, const QString& substring,
                              Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Removes all strings in a specified list from another string list
   *
   * @param input the list to remove strings from
   * @param toRemove the list of string to remove
   */
  static void removeAll(QStringList& input, const QStringList& toRemove);

  /**
   * Removes all occurrence of a group of strings from a single string
   *
   * @param input the string to remove from
   * @param toRemove the tokens to remove
   * @param caseSensitivity determines whether comparisons are case sensitive
   */
  static void removeAll(QString& input, const QStringList& toRemove,
                        Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Removes the last occurrence of a gropu of strings from a single string
   *
   * @param input the string to remove from
   * @param toRemove the tokens to remove
   * @param caseSensitivity determines whether comparisons are case sensitive
   */
  static void removeLastIndexOf(QString& input, const QStringList& toRemove,
                                Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Determines if list contains any strings from another list
   *
   * @param input list to search
   * @param toCompare list to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if at least one match is found; false otherwise
   */
  static bool containsAny(const QStringList& input, const QStringList& compareList,
                          Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Determines if a string starts with any one of a group of strings
   *
   * @param input the string to examine
   * @param compareList list to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if the input starts with any string in the compare list; false otherwise
   */
  static bool startsWithAny(const QString& input, const QStringList& compareList,
                            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Determines if a string ends with any one of a group of strings
   *
   * @param input the string to examine
   * @param compareList list to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if the input ends with any string in the compare list; false otherwise
   */
  static bool endsWithAny(const QString& input, const QStringList& compareList,
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
  static QString endsWithAnyAsStr(const QString& input, const QStringList& compareList,
                                  Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Determines if a string is contains a single instance of any one of a group of strings which
   * bisects it
   *
   * @param input the string to examine
   * @param compareList list to compare with
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return true if the input is bisected by any string in the compare list; false otherwise
   */
  static bool bisectsAny(const QString& input, const QStringList& compareList,
                         Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Determines if a string is contains a matching instance of any one of a group of regular
   * expressions which bisects it
   *
   * @param input the string to examine
   * @param compareList list of regular expressions to match with
   * @return true if the input has a bisecting token matching any regular expression in the compare
   * list; false otherwise
   */
  static bool bisectsAny(const QString& input, const QList<QRegExp>& compareList);

  /**
   * Splits a string on the first matching token in a group of strings and returns it only if is
   * of a specified size
   *
   * @param input string to split
   * @param tokenList list of strings to compare with
   * @param numOutputTokens the split output list must contain this number of strings
   * @param caseSensitivity determines whether comparisons are case sensitive
   * @return a collection of strings
   */
  static QStringList splitOnAny(const QString& input, const QStringList& tokenList,
                                const int numOutputTokens,
                                Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Splits a string on the first matching token in a group of regular expressions and returns it
   * only if is of a specified size
   *
   * @param input string to split
   * @param tokenList list of regular expression to match with
   * @param numOutputTokens the split output list must contain this number of strings
   * @return a collection of strings
   */
  static QStringList splitOnAny(const QString& input, const QList<QRegExp>& tokenList,
                                const int numOutputTokens);

  /**
   * Removes all map entries from input that are contained in a specified list
   *
   * @param input map to modify
   * @param keysToRemove keys to remove from input map
   */
  static void removeAllWithKey(QMap<QString, QString>& input, const QStringList& keysToRemove);

  /**
   * Replaces the last occurrence of a string with another string
   *
   * @param input the string to modify
   * @param strToReplace the token to replace in the string
   * @param replacementStr the token to replace with
   * @param caseSensitivity determines whether comparisons are case sensitive
   */
  static void replaceLastIndexOf(QString& input, const QString& strToReplace,
                                 const QString& replacementStr,
                                 Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

  /**
   * Splits a string on a token and removes the token found at the specified index
   *
   * @param input the string to remove from; the string remains unmodified if the string split parts
   * have no entry at the specified index
   * @param splitToken the string token to split the input string on
   * @param index the index at which to remove from the split string parts
   */
  static void splitAndRemoveAtIndex(QString& input, const QString& splitToken, const int index);

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
   * Splits a string on a token and returns the token found at the specified index
   *
   * @param input the string to retrieve from
   * @param splitToken the string token to split the input string on
   * @param index the index at which to return a split string parts
   * @return the string at the requested index
   */
  static QString splitAndGetAtIndex(
    const QString& input, const QString& splitToken, const int index);

  /**
   * Splits a string on a regular expression and returns the token found at the specified index
   *
   * @param input the string to retrieve from
   * @param splitExp the regular expression to split the input string on
   * @param index the index at which to return a split string parts
   * @return the string at the requested index
   */
  static QString splitAndGetAtIndex(
    const QString& input, const QRegExp& splitExp, const int index);

private:

  static QString _splitAndRemoveAtIndex(
    QStringList& input, const int index, const QString& separator);
};

}

#endif // STRINGUTILS_H
