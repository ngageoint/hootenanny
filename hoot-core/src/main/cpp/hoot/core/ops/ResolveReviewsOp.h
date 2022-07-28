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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef RESOLVE_REVIEWS_OP_H
#define RESOLVE_REVIEWS_OP_H

// Hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{
class OsmMap;

/**
 * Goes through all review relations and resolves them
 */
class ResolveReviewsOp : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "ResolveReviewsOp"; }

  ResolveReviewsOp();
  ~ResolveReviewsOp() override = default;

  /**
   * @brief The ResolveType enum Keep, Resolve, or Remove all reviews
   */
  enum ResolveType
  {
    KeepReviews,
    ResolveReviews,
    RemoveReviews
  };

  /**
   * OsmMapOperation interface overrides
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

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
    return "Resolved " + StringUtils::formatLargeNumber(_numAffected) + " conflation review relations";
  }

  ResolveType getResolveType() const { return _type; }

  void setResolveType(ResolveType type) { _type = type; }

private:

  int _taskStatusUpdateInterval;
  /** Type of resolve operation, keep all, remove all, or resolve all */
  ResolveType _type;
  /** Cache matches that have already been calculated so it doesn't have to be repeated */
  std::map<long, std::shared_ptr<Match>> _matchCache;

  /**
   * @brief _resolveReview
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveReview(const std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                      const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _resolveMultipleReviews
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveMultipleReviews(const std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                               const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _resolveMatchReview
   * @param match
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveMatchReview(const std::shared_ptr<Match>& match, const std::shared_ptr<OsmMap>& map,
                           const ElementId& relation_id, const ElementId& eid1,
                           const ElementId& eid2) const;
  /**
   * @brief _resolveManualReview
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   */
  void _resolveManualReview(const std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                            const ElementId& eid1, const ElementId& eid2) const;
  /**
   * @brief _getCachedMatch
   * @param map
   * @param relation_id
   * @param eid1
   * @param eid2
   * @return
   */
  MatchPtr _getCachedMatch(const std::shared_ptr<OsmMap>& map, const ElementId& relation_id,
                           const ElementId& eid1, const ElementId& eid2);
  /**
   * @brief _resolveString Convert text string to ResolveType
   * @param type Must be one of "Keep" (default), "Remove", or "Resolve"
   * @return ResolveType equivalent of string
   */
  ResolveType _resolveString(const QString& type) const;
};

}

#endif // RESOLVE_REVIEWS_OP_H
