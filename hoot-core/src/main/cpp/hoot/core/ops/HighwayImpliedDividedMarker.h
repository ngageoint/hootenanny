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

#ifndef HIGHWAYIMPLIEDDIVIDEDMARKER_H
#define HIGHWAYIMPLIEDDIVIDEDMARKER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>

// Standard



namespace hoot
{

class NodeToWayMap;
class OsmMap;
class Way;

/**
 * Locates sections that implicitly appear to be divided highways. Primarily this is bridges and
 * tunnels that are divided before and after the bridge/tunnel, but not tagged appropriately in
 * the tunnel. If two roads implicitly should be marked as divided based on the surrounding roads,
 * mark it as such. This is primarily caused by the FACC+ spec which does not allow bridges to
 * be marked as divided.
 */
class HighwayImpliedDividedMarker : public OsmMapOperation
{
public:

  static QString className() { return "hoot::HighwayImpliedDividedMarker"; }

  HighwayImpliedDividedMarker() = default;
  HighwayImpliedDividedMarker(const std::shared_ptr<const OsmMap>& map) : _inputMap(map) { }
  ~HighwayImpliedDividedMarker() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Splits all the ways in the input map and returns the resulting map.
   */
  static std::shared_ptr<OsmMap> markDivided(const std::shared_ptr<const OsmMap>& map);

  std::shared_ptr<OsmMap> markDivided();

  QString getInitStatusMessage() const override
  { return "Marking road sections that appear to be divided highways..."; }
  QString getCompletedStatusMessage() const override
  { return "Marked " + QString::number(_numAffected) + " road sections as divided highways"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   *
   * This isn't actually using HighwayCriterion in the filtering, but for the purposes of reducing
   * unnecessary conflate ops we don't need to run it unless we're running road conflation.
   */
  QStringList getCriteria() const override;

  QString getDescription() const override
  { return "Marks road sections that implicitly appear to be divided highways"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  std::shared_ptr<const OsmMap> _inputMap;
  std::shared_ptr<OsmMap> _result;
  std::shared_ptr<NodeToWayMap> _n2w;

  /**
   * Returns true if the given way has a divider highway connected on both ends.
   */
  bool _dividerSandwich(const std::shared_ptr<Way>& w) const;

  bool _hasDividerConnected(long nodeId, long excludedWayId) const;
};

}

#endif // HIGHWAYIMPLIEDDIVIDEDMARKER_H
