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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONMERGER_H
#define POIPOLYGONMERGER_H

// hoot
#include <hoot/core/conflate/MergerBase.h>

namespace hoot
{

/**
 * TODO: How much, if any, does this need to be genericized to handle areas (non-building polys)?
 */
class PoiPolygonMerger : public MergerBase
{

public:

  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  PoiPolygonMerger(const set< pair<ElementId, ElementId> >& pairs);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;

  /**
   * Merges a single POI with a single polygon, both as defined by PoiPolygonMerger
   *
   * @param map an OSM map containing a single node POI and a single polygon area or building, which
   * can be a way or a relation (multipolygon)
   */
  static void merge(OsmMapPtr map);

  virtual QString toString() const;

protected:

  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:

  set< pair<ElementId, ElementId> > _pairs;

  ElementId _mergeBuildings(const OsmMapPtr& map, vector<ElementId>& buildings1,
    vector<ElementId>& buildings2, vector< pair<ElementId, ElementId> >& replaced) const;

  Tags _mergePoiTags(const OsmMapPtr& map, Status s) const;

  vector<ElementId> _getBuildingParts(const OsmMapPtr& map, Status s) const;

};

}

#endif // POIPOLYGONMERGER_H
