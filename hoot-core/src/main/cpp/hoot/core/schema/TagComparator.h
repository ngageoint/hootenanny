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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef TAGCOMPARATOR_H
#define TAGCOMPARATOR_H

namespace hoot
{

class Tags;

/**
 * @brief The TagComparator class houses all the logic for feature tag merging during conflation
 * (Singleton).
 */
class TagComparator
{
public:

  static TagComparator& getInstance();

  /**
   * @param keepAllUnknownTags If this is set to true then all unknown tags will simply be
   *  concatenated using Tag lists.
   */
  void averageTags(const Tags& t1, const Tags& t2, Tags& result, bool keepAllUnknownTags = false,
                   bool caseSensitive = true);
  /**
   * @param keepAllUnknownTags If this is set to true then all unknown tags will simply be
   *  concatenated using Tag lists.
   */
  void averageTags(const Tags& t1, double w1, const Tags& t2, double w2, Tags& result,
                   bool keepAllUnknownTags = false, bool caseSensitive = true);

  void compareEnumeratedTags(Tags t1, Tags t2, double& score, double& weight) const;
  void compareNames(const Tags& t1, const Tags& t2, double& score, double& weight, bool strict = false) const;
  double compareTags(const Tags& t1, const Tags& t2, bool strict = false) const;
  /**
   * Compares all non-name text tags and puts the score in score and weight in weight.
   */
  void compareTextTags(const Tags& t1, const Tags& t2, double& score, double& weight) const;

  /**
   * Generalize the tags in t1 and t2 to make them consistent. Using the following rules:
   * - Names are merged using mergeNames.
   * - Exact matches are unchanged (lists are treated as unordered)
   * - Unrecognized tags are concatenated in a list.
   * - Tags that share an ancestor are promoted to the first common ancestor.
   * - If there are no conflicting tags, the tag is kept.
   */
  Tags generalize(Tags t1, Tags t2, bool overwriteUnrecognizedTags = false, bool caseSensitive = true);

  /**
   * @brief mergeNames - This merges "names" tags, so the name tag in t1 gets
   *                     to be the final "name" tag, and the "name" tag from t2
   *                     gets demoted to alt_name, along with any alt_names from
   *                     t1 & t2.
   * @param t1 Tags that are given preference
   * @param t2 These names/tags get demoted to alt_name
   * @param result Tags w/names merged
   * @param overwriteExcludeTagKeys keys of tags which should not be overwritten in t2
   * @param caseSensitive True for case sensitive merge names
   */
  void mergeNames(Tags& t1, Tags& t2, Tags& result, const QStringList& overwriteExcludeTagKeys = QStringList(),
                  bool caseSensitive = true) const;
  /**
   * Merge tags of type text
   *
   * @param t1 first set of tags to merge
   * @param t2 second set of tags to merge
   * @param result merged tags
   * @param overwriteExcludeTagKeys keys of tags which should not be overwritten in t2
   * @param caseSensitive True for case sensitive merge names
   */
  void mergeText(Tags& t1, Tags& t2, Tags& result, const QStringList& overwriteExcludeTagKeys = QStringList(),
                 bool caseSensitive = true) const;

  /**
   * Keep all names. If there is a conflict in tags between t1 and t2 then use the value in t1.
   *
   * @param t1 tags to keep
   * @param t2 tags to be overwritten
   * @param overwriteExcludeTagKeys optional keys of tags to exclude being overwritten in t2
   * @param accumulateValuesTagKeys optional keys of tags to preserve values for via concatenation
   * @param caseSensitive True for case sensitive merge names
   * @return merged tags
   */
  Tags overwriteMerge(Tags t1, Tags t2, const QStringList& overwriteExcludeTagKeys = QStringList(),
                      const QStringList& accumulateValuesTagKeys = QStringList(), bool caseSensitive = true);
  /**
   * Replace all tags in t2 with those from t1
   *
   * @param t1 Tags that replace
   * @param t2 Tags that are replaced
   * @param overwriteExcludeTagKeys keys of tags which should not be overwritten in t2
   * @param caseSensitive True for case sensitive merge names
   * @return merged tags
   */
  Tags replaceMerge(const Tags& t1, const Tags& t2, const QStringList& overwriteExcludeTagKeys = QStringList(),
                    bool caseSensitive = true);

  /**
   * Determines whether two tag sets have identical non-name, non-metadata tags.  Case sensitivity
   * is determined with duplicate.name.case.sensitive.
   *
   * @param t1 first set of tags to compare
   * @param t2 second set of tags to compare
   * @param caseSensitive True for case sensitive merge names
   * @return true if both tag sets have identical non-name, non-metadata contents (excluding
   * ordering); false otherwise
   */
  bool nonNameTagsExactlyMatch(const Tags& t1, const Tags& t2, bool caseSensitive = true) const;

private:

  void _addDefaults(Tags& t) const;
  void _addAsDefault(Tags& t, const QString& key, const QString& value) const;

  /**
   * Add any tags to result from t1 that don't conflict with t2. Any tag that is added to result
   * will also be removed from t1. This will not add any tags in t2 to result, nor modify t2.
   */
  void _addNonConflictingTags(Tags& t1, const Tags& t2, Tags& result) const;

  /**
   * Any exact matches between t1 & t2 are put into result and removed from t1/t2. All lists are
   * treated as unordered lists.
   */
  void _mergeExactMatches(Tags& t1, Tags& t2, Tags& result) const;
  /**
   * Any tag that is unrecognized will be put into result. If the values differ then a list will
   * be created and the value will be a list in result.
   */
  void _mergeUnrecognizedTags(Tags& t1, Tags& t2, Tags& result);

  /**
   * Write t2 tags to result, then write t1 tags. If there are conflicts then the t1 tags will
   * overwrite the t2 values. t1 & t2 will be cleared when this is done.
   */
  void _overwriteRemainingTags(Tags& t1, Tags& t2, Tags& result, const QStringList& overwriteExcludeTagKeys = QStringList(),
                               const QStringList& accumulateValuesTagKeys = QStringList(), bool caseSensitive = true) const;
  void _overwriteUnrecognizedTags(Tags& t1, Tags& t2, Tags& result) const;

  /**
   * If two tags share a common ancestor then the result gets the common ancestor and the tags are
   * removed from t1 and t2.
   */
  void _promoteToCommonAncestor(Tags& t1, Tags& t2, Tags& result) const;

  QSet<QString> _toSet(const Tags& t, const QString& k);

  /** Default constructor/destructor */
  TagComparator() = default;
  ~TagComparator() = default;
  /** Delete copy constructor and assignment operator */
  TagComparator(const TagComparator&) = delete;
  TagComparator& operator=(const TagComparator&) = delete;
};

}

#endif // TAGCOMPARATOR_H
