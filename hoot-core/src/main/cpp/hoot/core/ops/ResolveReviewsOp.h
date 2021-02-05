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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef RESOLVE_REVIEWS_OP_H
#define RESOLVE_REVIEWS_OP_H

// Hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{
class OsmMap;

/**
 * Goes through all review relations and resolves them
 */
class ResolveReviewsOp : public OsmMapOperation
{
public:

  static QString className() { return "hoot::ResolveReviewsOp"; }

  ResolveReviewsOp();
  virtual ~ResolveReviewsOp() = default;

  /**
   * @brief The ResolveType enum Keep, Resolve, or Remove all reviews
   */
  enum ResolveType
  {
    KeepReviews,
    ResolveReviews,
    RemoveReviews
  };

  ResolveType getResolveType() { return _type; }
  void setResolveType(ResolveType type) { _type = type; }

  /**
   * ApiEntityInfo interface overrides
   */
  QString getClassName() const override { return className(); }

  QString getName() const override { return className(); }

  QString getDescription() const override { return "Resolves conflation reviews"; }

  /**
   * OperationStatus interface overrides
   */
  QString getInitStatusMessage() const override
  { return "Resolving conflation reviews..."; }

  QString getCompletedStatusMessage() const override
  {
    return
      "Resolved " + StringUtils::formatLargeNumber(_numAffected) +
      " conflation review relations";
  }

  /**
   * OsmMapOperation interface overrides
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

private:

  /**
   * @brief _resolveReview
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveReview(std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                      const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _resolveMultipleReviews
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveMultipleReviews(std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                               const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _resolveMatchReview
   * @param match
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveMatchReview(std::shared_ptr<Match>& match, std::shared_ptr<OsmMap>& map,
                           const ElementId& relation_id, const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _resolveManualReview
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveManualReview(std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                            const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _getCachedMatch
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   * @return
   */
  MatchPtr _getCachedMatch(std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                           const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _resolveString Convert text string to ResolveType
   * @param type Must be one of "Keep" (default), "Remove", or "Resolve"
   * @return ResolveType equivalent of string
   */
  ResolveType _resolveString(const QString& type);

  int _taskStatusUpdateInterval;
  /** Type of resolve operation, keep all, remove all, or resolve all */
  ResolveType _type;
  /** Cache matches that have already been calculated so it doesn't have to be repeated */
  std::map<long, std::shared_ptr<Match>> _matchCache;
};

}

#endif // RESOLVE_REVIEWS_OP_H
