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
 * @copyright Copyright (C) 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>

namespace hoot
{

/*
 * @todo genericize this to use GeometricRelationShip?
 */
class InBoundsCriterion : public ElementCriterion, public Boundable, public ConstOsmMapConsumer,
  public Configurable
{
public:

  static std::string className() { return "hoot::InBoundsCriterion"; }

  InBoundsCriterion();
  InBoundsCriterion(const bool mustCompletelyContain);
  virtual ~InBoundsCriterion();

  /**
   * @see ElementCriterion
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  /**
   * @see ConstOsmMapConsumer
   */
  virtual void setOsmMap(const OsmMap* map);

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new InBoundsCriterion()); }

  void setTreatWayNodesAsPartOfWays(const bool treatAsPartOfWays)
  { _treatWayNodesAsPartOfWays = treatAsPartOfWays; }

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const
  { return "Determines whether an element is within a specified geospatial bounds"; }

  void setMustCompletelyContain(bool mustCompletelyContain)
  { _mustCompletelyContain = mustCompletelyContain; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

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
