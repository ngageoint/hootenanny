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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIARYMATCHCOMPARATOR_H
#define MULTIARYMATCHCOMPARATOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>

// Qt



namespace hoot
{

/**
 * Assumptions:
 *  - All features to be evaluated have a ID#= tag. (e.g. ID1=1-000001)
 *  - All manually matched features have the MATCH#= tag where the value is the smallest ID in the
 *    cluster of matched features.
 *  - The inputs were conflated and the result is passed in as the conflated object.
 *
 * This has been designed for buildings and POIs, but not roads.
 *
 * I used MatchComparator as a starting point for this class. There are enough differences between
 * the two that it doesn't really make sense to combine them at this point. At some point it may
 * make sense to add all the functionality that is currently in MatchComparator to this class, but
 * not today.
 *
 * See https://github.com/ngageoint/hootenanny/issues/1662 for some background.
 */
class MultiaryMatchComparator
{

public:

  static QString className() { return "hoot::MultiaryMatchComparator"; }

  static int logWarnCount;

  using IdToEid = QMap<QString, ElementId>;

  MultiaryMatchComparator();

  virtual ~MultiaryMatchComparator() = default;

  /**
   * Compares the maps and stores the confusion matrix.
   *
   * Returns the percent correct, or the Jaccard Index (https://en.wikipedia.org/wiki/Jaccard_index)
   *
   * @param conflated The map that resulted from conflation. If tag errors is disabled then this
   *  map will not be modified.
   * @return Percent of correct matches (tp / (tp + fp + fn))
   */
  virtual double evaluateMatches(const ConstOsmMapPtr& in, const OsmMapPtr& conflated);

  /**
   * Returns the F-Score.
   *
   * Must be called after evaluateMatches.
   *
   * https://en.wikipedia.org/wiki/F1_score
   */
  double getFScore(double beta=1) const;

  /**
   * Returns the percentage of records correctly classified (true positive rate)
   *
   * Must be called after evaluateMatches.
   *
   * getPercentNeedsReview() + getPercentWrong() + getPercentCorrect() == 1.0
   */
  double getPercentCorrect() const;

  /**
   * Returns the percentage of records incrorrectly classified, but that were not classified as
   * "review"
   *
   * Must be called after evaluateMatches.
   *
   * getPercentNeedsReview() + getPercentWrong() + getPercentCorrect() == 1.0
   */
  double getPercentWrong() const;

  /**
   * Returns the percentage of records that were incorrectly classified as needs review. This does
   * _not_ include the records that were correctly classified as needs review.
   *
   * Must be called after evaluateMatches.
   *
   * getPercentNeedsReview() + getPercentWrong() + getPercentCorrect() == 1.0
   */
  double getPercentUnnecessaryReview() const;

  /**
   * Returns the number of entries in the confusion matrix (as always, excludes true negatives)
   */
  int getTotalCount() const;

  /**
   * Return the values of the confusion matrix. True negative doesn't have a good definition here.
   * it would be all the remaining buildings pairs that weren't matched -- kinda meaningless.
   */
  double getTp() const { return _tp; }
  double getFp() const { return _fp; }
  double getFn() const { return _fn; }

  /**
   * If set to true then hoot:mismatch will be set to "true".
   */
  void setTagErrors(bool tagErrors) { _tagErrors = tagErrors; }

  /**
   * The script to use to determine the category of a feature. This can produce category based
   * confusion matrices.
   */
  void setTranslationScript(QString t) { _translationScript = t; }

  /**
   * Returns a bunch of info including the confusion matrix.
   */
  QString toString() const;

private:

  /// a cluster of IDs.
  using IdCluster = QSet<QString>;
  using IdClusterPtr = std::shared_ptr<IdCluster>;

  /// Provides indexed access to a set of elements that need to be reviewed.
  class ReviewClusterIndex : public QMap<QString, IdClusterPtr>
  {
  public:

    /**
     * Add a review entry from id1 to id2 and back.
     */
    void addReview(QString id1, QString id2)
    {
      _insert(id1, id2);
      _insert(id2, id1);
    }

    /**
     * Return the cluster that contains id. If the cluster doesn't exist an empty cluster is
     * returned.
     */
    IdClusterPtr getCluster(QString id) const
    {
      IdClusterPtr result = value(id);
      if (!result.get())
      {
        result = std::make_shared<IdCluster>();
      }
      return result;
    }

  private:
    void _insert(QString fromId, QString toId)
    {
      IdClusterPtr c;
      if (contains(fromId))
      {
        c = value(fromId);
      }
      else
      {
        c = std::make_shared<IdCluster>();
        insert(fromId, c);
      }

      c->insert(toId);
    }
  };

  // actual clusters of matched IDs. E.g. if A, B and C were automatically conflated together they
  // should all be in a single IdCluster and only one IdCluster.
  QSet<IdCluster> _actual;
  // expected clusters of matched IDs. E.g. if A, B and C were manually matched together they should
  // all be in a single IdCluster and only one IdCluster.
  QSet<IdCluster> _expected;

  /// Maps training IDs to element IDs
  IdToEid _actualIdToEid;
  IdToEid _expectedIdToEid;

  /// Maps all IDs in a cluster to the group of IDs that are part of the match.
  QHash<QString, IdClusterPtr> _actualMatchGroups;
  ReviewClusterIndex _actualReviews;
  /// Maps all IDs in a cluster to the group of IDs that are part of the match.
  QHash<QString, IdClusterPtr> _expectedMatchGroups;
  ReviewClusterIndex _expectedReviews;

  using ConfusionTable = QHash<int, QHash<int, int>>;

  /**
   * Confusion matrix with [actual][expected]
   */
  ConfusionTable _confusion;

  QMap<ElementType::Type, long> _elementWrongCounts;

  QString _nodeBasedStatsResult;

  /**
   * Matrix of wrong values. The data is arranged as [row][col] where row <= col. Using
   *
   */
  QHash<QString, QHash<QString, QVariant>> _wrongBreakdown;

  /// true positive, false positive, false negative counts.
  double _tp, _fp, _fn;

  /// True if errors should be tagged on the elements.
  bool _tagErrors;

  QString _translationScript;

  void _addToConfusionTable(const ConfusionTable& x, ConfusionTable& addTo) const;

  /**
   * Given the assigned "hoot:wrong" labels, calculate node base stats. These are easier to
   * understand, but give less information about the algorithm.
   */
  void _calculateNodeBasedStats(const ConstOsmMapPtr& conflated);

  /// clear out all the cached data.
  void _clearCache();

  /// Find all the matches in the conflated data.
  void _findActualMatches(const ConstOsmMapPtr& conflated);
  /// Find all the matches that were manually flagged in the input.
  void _findExpectedMatches(const ConstOsmMapPtr& in);
  /// Find all the reviews in the conflated data.
  void _findActualReviews(const ConstOsmMapPtr& conflated);
  /// Find all the reviews that were manually flagged in the input.
  void _findExpectedReviews(const ConstOsmMapPtr& in);

  /// Return all the IDs on a given element. Returns an empty list if there are none.
  static QList<QString> _getAllIds(ConstElementPtr e);

  /**
   * Print the confusion table in the redmine style of table output.
   *
   * @emptyMissMiss - If true the miss/miss cell is filled with a "-" instead a number.
   */
  QString _printConfusionTable(const ConfusionTable& x, bool emptyMissMiss = false) const;

  void _setElementWrongCounts(const ConstOsmMapPtr& map);
  void _setElementWrongCount(const ConstOsmMapPtr& map, const ElementType::Type& elementType);

  /**
   * Tag the element that contains the specified ID as containing a correct review.
   *
   * If _tagErrors is false, nothing happens.
   *
   * @param map The map that contains the element.
   * @param id The ID of the element to mark as wrong.
   */
  void _tagReview(const OsmMapPtr &map, const QString &id, QString value);

  /**
   * Tag the element that contains the specified ID as wrong.
   *
   * If _tagErrors is false, nothing happens.
   *
   * @param map The map that contains the element.
   * @param id The ID of the element to mark as wrong.
   */
  void _tagWrong(const OsmMapPtr &map, const QString &id, QString value);

  /**
   * Convenience method to convert a std::set to QSet.
   *
   * QSet is strongly preferred over set in this case b/c it performs shallow lazy copies rather
   * than deep copies by default.
   */
  QSet<QString> _toQSet(std::set<QString> s);
};

}

#endif // MULTIARYMATCHCOMPARATOR_H
