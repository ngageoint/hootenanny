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
  static QString getNumberStringPaddedWithZeroes(const int number, const int padSize);

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
   * @return true if any string in the input list contains the given substring; false otherwise
   */
  static bool containsSubstring(const QStringList& input, const QString& substring);

  /**
   * Removes all strings in a specified list from another string list
   *
   * @param input the list to remove strings from
   * @param toRemove the list of string to remove
   */
  static void removeAll(QStringList& input, const QStringList& toRemove);

  /**
   * Determines if list contains any strings from another list
   *
   * @param input list to search
   * @param toCompare list to compare with
   * @return true if at least one match is found; false otherwise
   */
  static bool containsAny(const QStringList& input, const QStringList& compareList);

  /**
   * Removes all map entries from input that are contained in a specified list
   *
   * @param input map to modify
   * @param keysToRemove keys to remove from input map
   */
  static void removeAllWithKey(QMap<QString, QString>& input, const QStringList& keysToRemove);
};

}

#endif // STRINGUTILS_H
