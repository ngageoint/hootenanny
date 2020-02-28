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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SpatialIndexer_H
#define SpatialIndexer_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/info/OperationStatusInfo.h>

// TGS
#include <tgs/RStarTree/HilbertRTree.h>

// Standard
#include <deque>
#include <functional>

namespace hoot
{

/**
 * This class is used to build an index of input elements (which can be filtered by various
 * criteria, if need be). The envelope plus the search radius is created as the index box for each
 * element. This is more efficient than using the OsmMapIndex index.
 */
class ElementCriterion;

class SpatialIndexer : public ElementConstOsmMapVisitor, public ElementCriterionConsumer
{
public:

  static std::string className() { return "hoot::CreateIndexVisitor"; }

  explicit SpatialIndexer(std::shared_ptr<Tgs::HilbertRTree>& index,
                          std::deque<ElementId>& indexToEid,
                          const std::shared_ptr<ElementCriterion>& criterion,
                          std::function<Meters (const ConstElementPtr& e)> getSearchRadius,
                          ConstOsmMapPtr pMap);

  void addCriterion(const ElementCriterionPtr& e);

  void visit(const ConstElementPtr& e);

  void finalizeIndex();

  /**
   * Find elements within a specified bounds
   *
   * @param env the bounds within the map to search
   * @param index a geospatial index for the input map
   * @param indexToEid a set of element IDs belonging to the input geospatial index
   * @param pMap the map to search
   * @param elementType the type of element to search for
   * @param includeContainingRelations if true, also returns relations containing the nearby
   * elements found
   * @return element IDs of the neighbors
   */
  static std::set<ElementId> findNeighbors(
    const geos::geom::Envelope& env, const std::shared_ptr<Tgs::HilbertRTree>& index,
    const std::deque<ElementId>& indexToEid, ConstOsmMapPtr pMap,
    const ElementType& elementType = ElementType::Unknown,
    const bool includeContainingRelations = true);

  /**
   * Find nodes nearby a specified node sorted by increasing distance given a bounds
   *
   * @param node node to search neighbors for
   * @param env the bounds within the map to search
   * @param index a geospatial index for the input map
   * @param indexToEid a set of element IDs belonging to the input geospatial index
   * @param pMap the map to search
   * @return element IDs of the neighbors
   */
  static QList<ElementId> findSortedNodeNeighbors(
    const ConstNodePtr& node, const geos::geom::Envelope& env,
    const std::shared_ptr<Tgs::HilbertRTree>& index, const std::deque<ElementId>& indexToEid,
    ConstOsmMapPtr pMap);

  virtual QString getDescription() const { return "Build an index of input elements"; }

  virtual QString getInitStatusMessage() const
  { return "Indexing elements..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Indexed " + QString::number(_numAffected) + " elements."; }

  long getSize() const { return _numAffected; }

  virtual std::string getClassName() const { return className(); }

private:

  std::shared_ptr<ElementCriterion> _criterion;
  std::function<Meters (const ConstElementPtr& e)> _getSearchRadius;

  std::shared_ptr<Tgs::HilbertRTree>& _index;
  std::deque<ElementId>& _indexToEid;
  std::vector<Tgs::Box> _boxes;
  std::vector<int> _fids;
};


}

#endif // SpatialIndexer_H
