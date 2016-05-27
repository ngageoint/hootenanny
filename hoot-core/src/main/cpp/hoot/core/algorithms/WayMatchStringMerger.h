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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYSTRINGMERGER_H
#define WAYSTRINGMERGER_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMapping.h>
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * Provides methods for merging features/geometries in one string of ways into another string of
 * ways.
 */
class WayMatchStringMerger
{
public:
  WayMatchStringMerger(WaySublineMatchStringPtr matchString, WayMatchStringMappingPtr mapping);

  /**
   * Merge all the tags from the scrap into the associated keeper ways.
   */
  void mergeTags(const TagMerger& merger);

  /**
   * Set the status on all the keeper ways.
   */
  void setKeeperStatus(Status s);

  /**
   * Merge scrapNode into the keeper way.
   */
  void mergeNode(ElementId scrapNode);


};

typedef boost::shared_ptr<WayMatchStringMerger> WayMatchStringMergerPtr;
typedef boost::shared_ptr<const WayMatchStringMerger> ConstWayMatchStringMergerPtr;

}

#endif // WAYSTRINGMERGER_H
