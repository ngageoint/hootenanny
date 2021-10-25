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

#ifndef IN_BOUNDS_CRITERION_H
#define IN_BOUNDS_CRITERION_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

// GEOS
#include <geos/geom/Geometry.h>

namespace hoot
{

/**
 * @brief The InBoundsCriterion class determines if an element is within a specified bounds.
 *
 * @todo genericize this to use GeometricRelationShip?
 */
class InBoundsCriterion : public ElementCriterion, public Boundable, public ConstOsmMapConsumer,
  public Configurable
{
public:

  static QString className() { return "InBoundsCriterion"; }

  InBoundsCriterion();
  InBoundsCriterion(const bool mustCompletelyContain);
  ~InBoundsCriterion() override;

  /**
   * @see ElementCriterion
   */
  bool isSatisfied(const ConstElementPtr& e) const override;
  /**
   * @see ElementCriterion
   */
  ElementCriterionPtr clone() override { return std::make_shared<InBoundsCriterion>(); }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see ConstOsmMapConsumer
   */
  void setOsmMap(const OsmMap* map) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  /**
   * @see ApiEntityInfo
   */
  QString getDescription() const override
  { return "Determines whether an element is within a specified geospatial bounds"; }

  void setTreatWayNodesAsPartOfWays(const bool treatAsPartOfWays)
  { _treatWayNodesAsPartOfWays = treatAsPartOfWays; }
  void setMustCompletelyContain(bool mustCompletelyContain)
  { _mustCompletelyContain = mustCompletelyContain; }

private:

  // This map must be in the same coord sys as the bounds.
  ConstOsmMapPtr _map;
  std::shared_ptr<ElementToGeometryConverter> _elementConverter;
  std::shared_ptr<WayNodeCriterion> _wayNodeCrit;
  // If false, the element can cross the bounds and still be considered within bounds.
  bool _mustCompletelyContain;
  // If this is enabled, only the parent ways of way nodes are checked for bounds satisfaction.
  bool _treatWayNodesAsPartOfWays;

  bool _nonWayNodeInBounds(const ConstElementPtr& e) const;
};

}

#endif // IN_BOUNDS_CRITERION_H
