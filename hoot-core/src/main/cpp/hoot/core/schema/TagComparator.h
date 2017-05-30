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

#ifndef TAGCOMPARATOR_H
#define TAGCOMPARATOR_H

// Standard
#include <set>
#include <vector>

// Qt
#include <QString>
#include <QSet>

namespace hoot
{

class Tags;

class TagComparator
{
public:

  TagComparator();

  /**
   * @param keepAllUnknownTags If this is set to true then all unknown tags will simply be
   *  concatenated using Tag lists.
   */
  void averageTags(const Tags& t1, const Tags& t2, Tags& result, bool keepAllUnknownTags = false);

  /**
   * @param keepAllUnknownTags If this is set to true then all unknown tags will simply be
   *  concatenated using Tag lists.
   */
  void averageTags(const Tags& t1, double w1, const Tags& t2, double w2, Tags& result,
    bool keepAllUnknownTags = false);

  void compareEnumeratedTags(Tags t1, Tags t2, double& score, double& weight);

  void compareNames(const Tags& t1, const Tags& t2, double& score, double& weight,
                    bool strict = false);

  double compareTags(const Tags& t1, const Tags& t2, bool strict = false);

  /**
   * Compares all non-name text tags and puts the score in score and weight in weight.
   */
  void compareTextTags(const Tags& t1, const Tags& t2, double& score, double& weight);

  /**
   * Generalize the tags in t1 and t2 to make them consistent. Using the following rules:
   * - Names are merged using mergeNames.
   * - Exact matches are unchanged (lists are treated as unordered)
   * - Unrecognized tags are concatenated in a list.
   * - Tags that share an ancestor are promoted to the first common ancestor
   * - If there are no conflicting tags the tag is kept
   */
  Tags generalize(Tags t1, Tags t2, bool overwriteUnrecognizedTags = false);

  static TagComparator& getInstance();

  void mergeNames(Tags& t1, Tags& t2, Tags& result);

  /**
   * Keep all names. If there is a conflict in tags between t1 and t2 then use the value in t1.
   */
  Tags overwriteMerge(Tags t1, Tags t2);

  void setCaseSensitive(bool caseSensitive) { _caseSensitive = caseSensitive; }

  /**
   * Determines whether two tag sets have identical non-name, non-metadata tags.  Case sensitivity
   * is determined with duplicate.name.case.sensitive.
   *
   * @param t1 first set of tags to compare
   * @param t2 second set of tags to compare
   * @return true if both tag sets have identical non-name, non-metadata contents (excluding
   * ordering); false otherwise
   */
  bool nonNameTagsExactlyMatch(const Tags& t1, const Tags& t2);

private:

  static TagComparator* _theInstance;

  void _addDefaults(Tags& t);

  void _addAsDefault(Tags& t, const QString& key, const QString& value);

  /**
   * Add any tags to result from t1 that don't conflict with t2. Any tag that is added to result
   * will also be removed from t1. This will not add any tags in t2 to result, nor modify t2.
   */
  void _addNonConflictingTags(Tags& t1, const Tags& t2, Tags& result);

  /**
   * Any exact matches between t1 & t2 are put into result and removed from t1/t2. All lists are
   * treated as unordered lists.
   */
  void _mergeExactMatches(Tags& t1, Tags& t2, Tags& result);

  void _mergeText(Tags& t1, Tags& t2, Tags& result);

  /**
   * Any tag that is unrecognized will be put into result. If the values differ then a list will
   * be created and the value will be a list in result.
   */
  void _mergeUnrecognizedTags(Tags& t1, Tags& t2, Tags& result);

  /**
   * Write t2 tags to result, then write t1 tags. If there are conflicts then the t1 tags will
   * overwrite the t2 values. t1 & t2 will be cleared when this is done.
   */
  void _overwriteRemainingTags(Tags& t1, Tags& t2, Tags& result);

  void _overwriteUnrecognizedTags(Tags& t1, Tags& t2, Tags& result);

  /**
   * If two tags share a common ancestor then the result gets the common ancestor and the tags are
   * removed from t1 and t2.
   */
  void _promoteToCommonAncestor(Tags& t1, Tags& t2, Tags& result);

  QSet<QString> _toSet(const Tags& t, const QString& k);

  bool _caseSensitive;
};

}

#endif // TAGCOMPARATOR_H
