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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
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
 */
class ReviewMarker
{
public:
  /// This definition may change over time.
  typedef ElementId ReviewUid;

  ReviewMarker();

  set<ReviewUid> getReviewUids(const ConstOsmMapPtr &map, ConstElementPtr e1) const;

  /**
   * Returns true if the element is in at least one review.
   */
  bool isNeedsReview(const ConstOsmMapPtr &map, ConstElementPtr e1);

  /**
   * Determines whether a pair of elements should be returned for review
   *
   * @param e1 the first element in the element pair being examined
   * @param e2 the second element in the element pair being examined
   * @return true if the pair should be reviewed against each other; false otherwise
   */
  bool isNeedsReview(const ConstOsmMapPtr &map, ConstElementPtr e1, ConstElementPtr e2);

  /**
   * Marks e1 and e2 as needing review and sets them to reference each other. If the score is
   * negative then the score is omitted.
   */
  void mark(const OsmMapPtr &map, ElementPtr& e1, ElementPtr& e2, const QString& note,
    double score = -1);

  /**
   * Marks a single element as needing review.
   */
  void mark(const OsmMapPtr &map, ElementPtr& e, const QString& note, double score = -1);

private:
  // don't use these keys directly, instead call the helper functions above.
  static QString revieweeKey() { return "reviewee"; }
  static QString reviewUuidKey() { return "hoot:review:uuid"; }
  static QString reviewScoreKey() { return "hoot:review:score"; }
  static QString reviewNeedsKey() { return "hoot:review:needs"; }
  static QString reviewNoteKey() { return "hoot:review:note"; }

  set<ElementId> _getReviewRelations(const ConstOsmMapPtr &map, ElementId eid) const;

  void _updateScore(Tags& t, double score);
};

}

#endif // REVIEWMARKER_H
