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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGMERGER_H
#define BUILDINGMERGER_H

// hoot
#include <hoot/core/conflate/merging/MergerBase.h>

// Standard
#include <set>

namespace hoot
{

/**
 * Merges two buildings
 */
class BuildingMerger : public MergerBase
{
public:

  static unsigned int logWarnCount;

  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  BuildingMerger(const std::set< std::pair<ElementId, ElementId> >& pairs);

  virtual void apply(const OsmMapPtr& map, std::vector< std::pair<ElementId, ElementId> >& replaced);

  static boost::shared_ptr<Element> buildBuilding(const OsmMapPtr& map,
                                                  const std::set<ElementId>& eids);

  virtual QString toString() const;

  /**
   * Utility method that allows for merging an unlimited number of buildings
   *
   * The map passed may or may not contain constituent elements (way nodes, relation members).
   *
   * @param map a map containing the buildings to be merged
   * @param mergeTargetId the ID of the building which all other buildings should be merged into
   */
  static void mergeBuildings(OsmMapPtr map, const ElementId& mergeTargetId);

protected:

  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:

  std::set< std::pair<ElementId, ElementId> > _pairs;

  boost::shared_ptr<Element> _buildBuilding1(const OsmMapPtr& map) const;
  boost::shared_ptr<Element> _buildBuilding2(const OsmMapPtr& map) const;

  QSet<ElementId> _getMultiPolyMemberIds(const ConstElementPtr& element) const;
};

}

#endif // BUILDINGMERGER_H
