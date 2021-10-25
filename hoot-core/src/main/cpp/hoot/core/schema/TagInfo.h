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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TAGINFO_H
#define TAGINFO_H

// Hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Prints information about a set of tags
 */
class TagInfo
{
public:

  /**
   * @brief Constructor
   * @param tagValuesPerKeyLimit the maximum number of tag values to return per key
   * @param keys specific tag keys for which to return tag values only; if empty, then values are
     returned for all tag keys
   * @param keysOnly if true, only tag keys will be returned
   * @param caseSensitive if true, tag comparisons are case sensitive
   * @param exactKeyMatch if true, specified/feature tag keys need to match exactly for values to be
   * written; otherwise they match if any part of the feature tag key is contained in the specified
   * tag key
   * @param delimitedTextOutputprints the output as a single delimited string instead of JSON
   */
  TagInfo(
    const int tagValuesPerKeyLimit = INT_MAX, const QStringList& keys = QStringList(),
    const bool keysOnly = false, const bool caseSensitive = true, const bool exactKeyMatch = true,
    const bool delimitedTextOutput = false);

  /**
   * Returns a JSON string with tag values grouped by keys
   *
   * @param inputs data sources with tags to examine
   * @return a JSON tags string
   */
  QString getInfo(const QStringList& inputs) const;

private:

  using TagInfoHash = QHash<QString, QHash<QString, int>>;

  // see descriptions in constructor doc

  int _tagValuesPerKeyLimit;

  QStringList _keys;

  bool _keysOnly;
  bool _caseSensitive;
  bool _exactKeyMatch;
  bool _delimitedTextOutput;

  int _taskStatusUpdateInterval;

  QString _getInfo(const QString& input) const;
  QString _getInfoFromOgrInput(QString& input) const;
  QString _getInfoFromStreamableInput(const QString& input) const;
  QString _getInfoFromMemoryBoundInput(const QString& input) const;

  QString _printJSON(const QString& lName, TagInfoHash& data) const;
  QString _printDelimitedText(const TagInfoHash& data) const;

  void _parseElement(const ConstElementPtr& e, TagInfoHash& result) const;
  bool _tagKeysMatch(const QString& tagKey) const;
};

}

#endif // TAGINFO_H
