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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONMERGER_H
#define POIPOLYGONMERGER_H

// hoot
#include <hoot/core/conflate/merging/MergerBase.h>

namespace hoot
{

/**
 * Merges POIs with polygons
 */
class PoiPolygonMerger : public MergerBase
{

public:

  static std::string className() { return "hoot::PoiPolygonMerger"; }

  static unsigned int logWarnCount;

  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  PoiPolygonMerger(const std::set< std::pair<ElementId, ElementId> >& pairs);

  virtual void apply(const OsmMapPtr& map, std::vector< std::pair<ElementId, ElementId> >& replaced);

  virtual QString toString() const;

  /**
   * Utility method for merging a single POI with a single polygon
   *
   * The map passed may or may not contain constituent elements (way nodes, relation members).

   * @param map a map containing the two features to merge
   * @return the ID of the element that was merged into
   */
  static ElementId mergePoiAndPolygon(OsmMapPtr map);

protected:

  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:

  std::set< std::pair<ElementId, ElementId> > _pairs;

  bool _autoMergeManyPoiToOnePolyMatches;

  ElementId _mergeBuildings(const OsmMapPtr& map, std::vector<ElementId>& buildings1,
                            std::vector<ElementId>& buildings2,
                            std::vector< std::pair<ElementId, ElementId> >& replaced) const;

  Tags _mergePoiTags(const OsmMapPtr& map, Status s) const;

  std::vector<ElementId> _getBuildingParts(const OsmMapPtr& map, Status s) const;

};

}

#endif // POIPOLYGONMERGER_H
