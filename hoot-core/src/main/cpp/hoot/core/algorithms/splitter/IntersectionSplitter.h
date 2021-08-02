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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef INTERSECTIONSPLITTER_H
#define INTERSECTIONSPLITTER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

// Qt
#include <QMultiHash>
#include <QSet>
#include <QMap>
#include <QList>

namespace hoot
{

class OsmMap;
class Way;

/**
 * G
 */
/**
 * @brief The IntersectionSplitter class given a map, makes all intersections contain only way end
 * nodes.
 *
 * For example, if two ways make a four way intesection the intersection splitter will convert that
 * into four ways that meet at a four way intersection. No nodes are modified in this process.
 */
class IntersectionSplitter : public OsmMapOperation
{
public:

  static QString className() { return "hoot::IntersectionSplitter"; }

  IntersectionSplitter() = default;
  IntersectionSplitter(const std::shared_ptr<OsmMap>& map);
  ~IntersectionSplitter() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  static void splitIntersections(const std::shared_ptr<OsmMap>& map);
  void splitIntersections();

  QString getInitStatusMessage() const override { return "Splitting linear intersections..."; }
  QString getCompletedStatusMessage() const override
  { return "Split " + QString::number(_numAffected) + " linear intersections"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Splits linear intersections by making them contain only way end nodes"; }

private:

  std::shared_ptr<OsmMap> _map;
  QMultiHash<long, long> _nodeToWays;
  QSet<long> _todoNodes;

  void _mapNodesToWays();
  void _mapNodesToWay(const std::shared_ptr<Way>& w);
  void _removeWayFromMap(const std::shared_ptr<Way>& way);

  /*
   * Given a way and a node, split the way at that node.
   */
  void _splitWay(long wayId, long nodeId);

  /*
   * Checks relations a split way belongs to and attempts to preserve membership order
   *
   * @param splitWayId the ID of way that has been split
   * @param newWays the ways replacing the way that has been split.
   * @note This may not be 100% correct yet.
   */
  void _preserveWayRelationMemberOrder(
    const ElementId& splitWayId, QList<ElementPtr>& newWays) const;
  /*
   * Given a collection of ways made as the result of splitting, determines which one is the
   * original way that was split and which one was created as a result of the split.
   *
   * @param splitWays collection of ways created after intersection splitting
   * @param splitWayId ID of the original way known to have been split
   * @param splitWay to be modified pointer to the original split way
   * @param addedWay to be modified pointer to the way created as a result of splitting
   * @return true if the original split way is first in the input collection; false otherwise
   */
  bool _determineSplitWaysOrdering(
    const QList<ElementPtr>& splitWays, const ElementId& splitWayId, WayPtr& splitWay,
    WayPtr& addedWay) const;
  /*
   * Retrieves adjoining way members from a relation for a given way
   *
   * @param wayId the ID of the way member to retrieve adjoining members for
   * @param containingRelation the relation containing the way with ID = wayId
   * @param adjoiningWayMemberIndexedBefore to be modified pointer to way member indexed before the
   * input way; may end up being null
   * @param adjoiningWayMemberIndexedAfter to be modified pointer to way member indexed after the
   * input way; may end up being null
   * @return a list of element IDs of size = 1 or 2 for the adjoining members where for a non-end
   * input way member the first ID is the ID of the member ordered immediately before the input way
   * and the second ID is the ID of the member ordered immediately after the input way. If the input
   * way is an end member (first or last member), then a list of size one is returned with either
   * the ID of the member before OR the ID of the member after.
   */
  QList<ElementId> _getAdjoiningRelationMembers(
    const ElementId& wayId, const ConstRelationPtr& containingRelation,
    ConstWayPtr& adjoiningWayMemberIndexedBefore,
    ConstWayPtr& adjoiningWayMemberIndexedAfter) const;
};

}

#endif // INTERSECTIONSPLITTER_H
