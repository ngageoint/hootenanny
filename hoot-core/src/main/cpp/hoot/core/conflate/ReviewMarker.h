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
#ifndef REVIEWMARKER_H
#define REVIEWMARKER_H

#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * This class centralizes adding tags to mark elements for review. Please do not set or get any
 * hoot:review:* tags directly. If you need some new functionality then please add another method
 * here.
 *
 * NOTE: Some of these methods may seem like unnecessary abstraction. This is b/c the review tagging
 * approach already changed once from the original definition. Chances of another change are
 * probably pretty good. If there is another change then we want to make it easy to refactor.
 */
class ReviewMarker
{
public:
  /// This definition may change over time.
  typedef ElementId ReviewUid;

  /// Should only be used by AddHilbertReviewSortOrder. Please use the ReviewMarker helper methods
  /// for other operations.
  static QString reviewSortOrderKey;

  /// This is in place so that we know the original number of members in the review relations in case
  /// one of them is deleted during the process as a result of a merge.  This information can then
  /// be used during cleanup of review relations for validity purposes.
  static QString reviewMemberCountKey;

  ReviewMarker();

  static QString getBadGeometryType() { return _complexGeometryType; }

  static set<ElementId> getReviewElements(const ConstOsmMapPtr &map, ReviewUid uid);

  static set<ReviewUid> getReviewUids(const ConstOsmMapPtr &map, ConstElementPtr e1);

  static QString getReviewType(const ConstOsmMapPtr &map, ReviewUid uid);

  static set<ReviewUid> getReviewUids(const ConstOsmMapPtr &map);

  /**
   * Returns true if the element is in at least one review.
   */
  static bool isNeedsReview(const ConstOsmMapPtr &map, ConstElementPtr e1);

  /**
   * Determines whether a pair of elements should be returned for review
   *
   * @param e1 the first element in the element pair being examined
   * @param e2 the second element in the element pair being examined
   * @return true if the pair should be reviewed against each other; false otherwise
   */
  static bool isNeedsReview(const ConstOsmMapPtr &map, ConstElementPtr e1, ConstElementPtr e2);

  /**
   * Returns true if the specified UID is a review tag.
   */
  static bool isReviewUid(const ConstOsmMapPtr &map, ReviewUid uid);

  /**
   * Marks e1 and e2 as needing review and sets them to reference each other. If the score is
   * negative then the score is omitted.
   *
   * @param note A human readable note describing the review.
   * @param reviewType A human readable review type. Typically this is a one word description of
   *  the feature being reviewed. E.g. "Highway" or "Building".
   */
  static void mark(const OsmMapPtr &map, const ElementPtr& e1, const ElementPtr& e2,
    const QString& note, const QString& reviewType, double score = -1,
    vector<QString> choices = vector<QString>() );

  /**
   * Marks a set of elements as needing review and sets them to reference each other. If the score is
   * negative then the score is omitted.
   *
   * @param note A human readable note describing the review.
   * @param reviewType A human readable review type. Typically this is a one word description of
   *  the feature being reviewed. E.g. "Highway" or "Building".
   */
  static void mark(const OsmMapPtr &map, set<ElementId> ids,
    const QString& note, const QString& reviewType, double score = -1,
    vector<QString> choices = vector<QString>() );

  /**
   * Marks a single element as needing review.
   */
  static void mark(const OsmMapPtr &map, const ElementPtr& e, const QString& note,
    const QString& reviewType, double score = -1, vector<QString> choices = vector<QString>());

  /**
   * Removes a single element.
   */
  static void removeElement(const OsmMapPtr& map, ElementId eid);

private:
  // don't use these keys directly, instead call the helper functions above.
  static QString _complexGeometryType;
  static QString _revieweeKey;
  static QString _reviewUuidKey;
  static QString _reviewScoreKey;
  static QString _reviewNeedsKey;
  static QString _reviewNoteKey;
  static QString _reviewTypeKey;
  static QString _reviewChoicesKey;

  /**
   * Returns a hilbert value that represents the center of the bounds that covers e1 and e2.
   */
  static int64_t _calculateHilbertValue(const ConstOsmMapPtr &map, ConstElementPtr e1,
    ConstElementPtr e2 = ConstElementPtr());

  static set<ElementId> _getReviewRelations(const ConstOsmMapPtr &map, ElementId eid);

  void _updateScore(Tags& t, double score);

  // for white box testing.
  friend class RemoveInvalidReviewRelationsVisitorTest;
};

}

#endif // REVIEWMARKER_H
