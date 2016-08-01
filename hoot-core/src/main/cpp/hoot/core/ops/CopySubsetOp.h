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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COPYSUBSETOP_H
#define COPYSUBSETOP_H

// hoot
#include <hoot/core/OsmMap.h>

#include "OsmMapOperation.h"

namespace hoot
{

/**
 * Copies a subset of the map into a new map. The old map is unchanged.
 */
class CopySubsetOp : public OsmMapOperation
{
public:
  CopySubsetOp(const ConstOsmMapPtr& from, const set<ElementId>& eids);

  CopySubsetOp(const ConstOsmMapPtr& from, const vector<long>& ids);

  CopySubsetOp(const ConstOsmMapPtr& from, ElementId eid1, ElementId eid2);

  /**
   * A new map is created and the eids specified in the constructor and their depedencies will be
   * copied into the new map. The @a map will be set to point to the new map.
   */
  virtual void apply(shared_ptr<OsmMap>& map);

private:
  set<ElementId> _eids;
  const ConstOsmMapPtr& _from;
};

}

#endif // COPYSUBSETOP_H
