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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef IDSWAPOP_H
#define IDSWAPOP_H

#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>

namespace hoot
{

/**
 * @brief The IdSwapOp class is used to swap node and way IDs with each other
 *  It is used to preserve node IDs within ways when merging ways
 */
class IdSwapOp : public ConstOsmMapOperation
{
public:

  /**
   * @brief className - Get classname string
   * @return - "IdSwapOp"
   */
  static QString className() { return "IdSwapOp"; }

  /**
   * @brief IdSwapOp - Default constructor
   */
  IdSwapOp() = default;
  /**
   * @brief IdSwapOp - Constructor taking a swap object with the IDs to swap
   */
  IdSwapOp(const IdSwapPtr& idSwap);
  /**
   * @brief IdSwapOp - Constructor taking two elements to swap IDs
   */
  IdSwapOp(ElementId e1, ElementId e2);
  ~IdSwapOp() = default;

  /**
   * @brief apply - Apply the IdSwap op
   * @param pMap - Target map
   */
  void apply(const std::shared_ptr<OsmMap>& map) override;

  QString getInitStatusMessage() const override { return "Swapping IDs..."; }
  QString getCompletedStatusMessage() const override
  { return "Swapped " + QString::number(_numAffected) + " IDs."; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Swap IDs for ID preservation in Attribute Conflation"; }

private:

  /**
   * @brief swapNodeIdInWay Swap the node ID in the ways with the swap ID
   * @param map - Target map
   * @param nodeId - Node ID of the node in the ways that is being replaced
   * @param swapId - Node ID of the node to swap into the ways
   */
  void swapNodeIdInWay(const std::shared_ptr<OsmMap>& map, long nodeId, long swapId) const;

  /** Element IDs of elements to swap, if empty check the map for an IdSwap object */
  IdSwapPtr _idSwap;
};

}

#endif  //  IDSWAPOP_H
