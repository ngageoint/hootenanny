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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGMERGER_H
#define BUILDINGMERGER_H

// hoot
#include <hoot/core/conflate/MergerBase.h>

// Standard
#include <set>

namespace hoot
{

class BuildingMerger : public MergerBase
{
public:

  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  BuildingMerger(const std::set< std::pair<ElementId, ElementId> >& pairs);

  virtual void apply(const OsmMapPtr& map, std::vector< std::pair<ElementId, ElementId> >& replaced);

  static boost::shared_ptr<Element> buildBuilding(const OsmMapPtr& map,
                                                  const std::set<ElementId>& eids);

  virtual QString toString() const;

protected:

  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:

  std::set< std::pair<ElementId, ElementId> > _pairs;

  boost::shared_ptr<Element> _buildBuilding1(const OsmMapPtr& map) const;
  boost::shared_ptr<Element> _buildBuilding2(const OsmMapPtr& map) const;
};

}

#endif // BUILDINGMERGER_H
