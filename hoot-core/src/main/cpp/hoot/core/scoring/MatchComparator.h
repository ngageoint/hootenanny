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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MATCHCOMPARATOR_H
#define MATCHCOMPARATOR_H

// hoot
#include <hoot/core/OsmMap.h>

// tgs
#include <tgs/DisjointSet/DisjointSetMap.h>

namespace hoot
{

/**
 * Assumptions:
 *  - The inputs with REF1 tagged is the "reference" set. (Unknown1)
 *  - The inputs with REF2 were manually matched to all relevant REF1 features. (Unknown2)
 *  - The inputs with REVIEW were manually matched to all relevant REF1 features. (Unknown2)
 *  - A UUID has been assigned to all features with a REF* tag and the key is "uuid" (see
 *    AddUuidVisitor)
 *  - The inputs were conflated and the result is passed in as the conflated object.
 *
 * See #2232 for some background.
 */
class MatchComparator
{

public:

  typedef QMultiMap<QString, ElementId> UuidToEid;

  MatchComparator();

  /**
   * Compares the maps and stores the confusion matrix.
   * @param conflated The map that resulted from conflation. If tag errors is disabled then this
   *  map will not be modified.
   * @return Percent of correct matches (tp / (tp + fp + fn))
   */
  virtual double evaluateMatches(const ConstOsmMapPtr& in, const OsmMapPtr& conflated);

  /**
   * Returns the percentage of records correctly classified (true positive rate)
   *
   * getPercentNeedsReview() + getPercentWrong() + getPercentCorrect() == 1.0
   */
  double getPercentCorrect() const;

  /**
   * Returns the percentage of records incrorrectly classified, but that were not classified as
   * "review"
   *
   * getPercentNeedsReview() + getPercentWrong() + getPercentCorrect() == 1.0
   */
  double getPercentWrong() const;

  /**
   * Returns the percentage of records that were incorrectly classified as needs review. This does
   * _not_ include the records that were correctly classified as needs review.
   *
   * getPercentNeedsReview() + getPercentWrong() + getPercentCorrect() == 1.0
   */
  double getPercentUnnecessaryReview() const;

  int getTotalCount() const;

  /**
   * Return the values of the confusion matrix. True negative doesn't have a good definition here.
   * it would be all the remaining buildings pairs that weren't matched -- kinda meaningless.
   */
  int getTp() const { return _tp; }
  int getFp() const { return _fp; }
  int getFn() const { return _fn; }

  /**
   * If set to true then hoot:mismatch will be set to "true".
   */
  void setTagErrors(bool tagErrors) { _tagErrors = tagErrors; }

  QString toString() const;

  /**
    Calculates the PERTY score for conflated data matches
    @see PertyScoreMatchesCmd and PertyOp
    */
  double getPertyScore() const;

private:
  typedef pair<QString, QString> UuidPair;

  set<UuidPair> _actual;
  set<UuidPair> _expected;
  // contains a disjoint set of all matched uuids. E.g. if A matched B and B matches C then
  // A, B and C will all be in the same group.
  UuidToEid _actualUuidToEid;
  Tgs::DisjointSetMap<QString> _actualMatchGroups;
  Tgs::DisjointSetMap<QString> _actualReviewGroups;
  Tgs::DisjointSetMap<QString> _expectedMatchGroups;
  Tgs::DisjointSetMap<QString> _expectedReviewGroups;

  /**
   * Confusion matrix with [actual][expected]
   */
  vector< vector<int> > _confusion;

  /**
   * Matrix of wrong values. The data is arranged as [row][col] where row <= col. Using
   *
   */
  QHash< QString, QHash<QString, QVariant> > _wrongBreakdown;

  int _tp, _fp, _fn;
  bool _tagErrors;

  QMap<ElementType::Type, long> _elementWrongCounts;

  void _addWrong(const Tags& t1, const Tags& t2);

  void _clearCache();
  void _createMatches(const set<QString>& uuids1, const set<QString>& uuids2, set<UuidPair>& matches,
    Tgs::DisjointSetMap<QString>& groups);

  bool _debugLog(QString uuid1, QString uuid2, const ConstOsmMapPtr& in,
    const ConstOsmMapPtr& conflated);

  void _findActualMatches(const ConstOsmMapPtr& in, const ConstOsmMapPtr& conflated);
  void _findExpectedMatches(const ConstOsmMapPtr& in);

  /**
   * Returns true if both these uuids are marked with needs review.
   */
  bool _isNeedsReview(QString uuid1, QString uuid2, const ConstOsmMapPtr& conflated);

  void _tagError(const OsmMapPtr& map, const QString &uuid, const QString& value);
  void _tagWrong(const OsmMapPtr &map, const QString &uuid);
  void _tagTestOutcome(const OsmMapPtr& map, const QString uuid, const QString expected,
                       const QString actual);
  void _setElementWrongCounts(const ConstOsmMapPtr& map);
  void _setElementWrongCount(const ConstOsmMapPtr& map, const ElementType::Type& elementType);
};

}

#endif // MATCHCOMPARATOR_H
