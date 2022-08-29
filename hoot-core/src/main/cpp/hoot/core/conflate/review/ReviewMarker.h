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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REVIEWMARKER_H
#define REVIEWMARKER_H

#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/OsmMap.h>

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
  using ReviewUid = ElementId;

  ReviewMarker();

  /**
   * Marks e1 and e2 as needing review and sets them to reference each other. If the score is
   * negative then the score is omitted.
   *
   * @param note A human readable note describing the review.
   * @param reviewType A human readable review type. Typically this is a one word description of
   *  the feature being reviewed. E.g. "Highway" or "Building".
   */
  void mark(const OsmMapPtr& map, const ConstElementPtr& e1, const ConstElementPtr& e2, const QString& note,
            const QString& reviewType, double score = -1,
            const std::vector<QString>& choices = std::vector<QString>()) const;
  /**
   * Marks a set of elements as needing review and sets them to reference each other. If the score
   * is negative then the score is omitted.
   *
   * @param note A human readable note describing the review.
   * @param reviewType A human readable review type. Typically this is a one word description of
   *  the feature being reviewed. E.g. "Highway" or "Building".
   */
  void mark(const OsmMapPtr& map, const std::set<ElementId>& ids, const QString& note,
            const QString& reviewType, double score = -1,
            const std::vector<QString>& choices = std::vector<QString>()) const;
  /**
   * Marks a single element as needing review.
   */
  void mark(const OsmMapPtr& map, const ConstElementPtr& e, const QString& note, const QString& reviewType,
            double score = -1, const std::vector<QString>& choices = std::vector<QString>()) const;
  /**
   * Marks a vector of elements as needing review and sets them to reference each other. If the score
   * is negative then the score is omitted.
   *
   * @param note A human readable note describing the review.
   * @param reviewType A human readable review type. Typically this is a one word description of
   *  the feature being reviewed. E.g. "Highway" or "Building".
   */
  void mark(const OsmMapPtr& map, const std::vector<ElementId>& ids, const QString& note,
            const QString& reviewType, double score = -1,
            const std::vector<QString>& choices = std::vector<QString>()) const;

  static QString getBadGeometryType() { return _complexGeometryType; }

  static std::set<ElementId> getReviewElements(const ConstOsmMapPtr& map, ReviewUid uid);

  static std::set<ReviewUid> getReviewUids(const ConstOsmMapPtr& map, ConstElementPtr e1);
  static std::set<ReviewUid> getReviewUids(const ConstOsmMapPtr& map, ReviewUid uid);
  static std::set<ReviewUid> getReviewUids(const ConstOsmMapPtr& map);

  static QString getReviewType(const ConstOsmMapPtr& map, ReviewUid uid);

  /**
   * Returns true if the element is in at least one review.
   */
  static bool isNeedsReview(const ConstOsmMapPtr& map, ConstElementPtr e1);
  /**
   * Determines whether a pair of elements should be returned for review
   *
   * @param e1 the first element in the element pair being examined
   * @param e2 the second element in the element pair being examined
   * @return true if the pair should be reviewed against each other; false otherwise
   */
  static bool isNeedsReview(const ConstOsmMapPtr& map, ConstElementPtr e1, ConstElementPtr e2);

  /**
   * Returns true if this element is a review record (not an element that needs to be reviewed, but
   * the relation that describes the review).
   *
   * isReviewUid is preferred over this method. If we change the review mechanism in the future
   * this method may be broken. (unlikely)
   */
  static bool isReview(const ConstElementPtr& e);

  /**
   * Returns true if the specified UID is a review tag.
   */
  static bool isReviewUid(const ConstOsmMapPtr& map, ReviewUid uid);

  /**
   * Removes a single element.
   */
  static void removeElement(const OsmMapPtr& map, ElementId eid);

private:

  // don't use these keys directly, instead call the helper functions above.
  static QString _complexGeometryType;

  //also marks features contained by a review relation
  bool _addReviewTagsToFeatures;

  /**
   * Returns a hilbert value that represents the center of the bounds that covers e1 and e2.
   */
  static int64_t _calculateHilbertValue(const ConstOsmMapPtr& map, ConstElementPtr e1,
                                        ConstElementPtr e2 = ConstElementPtr());

  static std::set<ElementId> _getReviewRelations(const ConstOsmMapPtr& map, ElementId eid);

  void _updateScore(Tags& t, double score);

  // for white box testing.
  friend class RemoveInvalidReviewRelationsVisitorTest;
};

}

#endif // REVIEWMARKER_H
