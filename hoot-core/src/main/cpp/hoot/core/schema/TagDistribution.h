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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TAG_DISTRIBUTION_H
#define TAG_DISTRIBUTION_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/PartialOsmMapReader.h>

// Qt
#include <QStringList>
#include <QRegExp>

namespace hoot
{

/**
 * Counts tag values for given tag keys
 */
class TagDistribution
{
public:

  TagDistribution();

  /**
   * Calculates tag counts for tag keys
   *
   * @param inputs one or more map files to examine
   * @return a mapping of tag value strings to occurrence counts
   */
  std::map<QString, int> getTagCounts(const QStringList& inputs);

  /**
   * Returns a summary for tag count data
   *
   * @param tagCounts the data to display
   */
  QString getTagCountsString(const std::map<QString, int>& tagCounts) const;

  void setTagKeys(const QStringList& keys) { _tagKeys = keys; }
  void setCriterionClassName(const QString& name) { _criterionClassName = name; }
  void setSortByFrequency(const bool sort) { _sortByFrequency = sort; }
  void setTokenize(const bool tokenize) { _tokenize = tokenize; }
  void setLimit(const int limit) { _limit = limit; }

private:

  // the tag keys to collect tag values distribution stats for
  QStringList _tagKeys;

  // optional filtering crit
  QString _criterionClassName;

  // if true, tag values sorted by decreasing frequency; otherwise sorted alphabetically
  bool _sortByFrequency;

  // breaks up tag values into tokens
  bool _tokenize;

  // max number of tag values to process per key
  int _limit;

  // count of total tags processed for all keys
  long _total;

  int _taskStatusUpdateInterval;
  QRegExp _nonWord;

  void _countTags(const QString& input, std::map<QString, int>& tagCounts);
  std::shared_ptr<PartialOsmMapReader> _getReader(const QString& input) const;
  ElementInputStreamPtr _getFilteredInputStream(const ElementInputStreamPtr& inputStream) const;
  ElementCriterionPtr _getCriterion() const;

  QString _getPercentageStr(const double percentage) const;
};

}
#endif // TAG_DISTRIBUTION_H
