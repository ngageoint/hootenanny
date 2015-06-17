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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CREATEINTERSECTIONNODESOP_H
#define CREATEINTERSECTIONNODESOP_H

#include "OsmMapOperation.h"

namespace hoot
{

/**
 * Given a map, find all the locations where ways intersect and add nodes at those intersections
 * if they don't already exist. New nodes will be created at each of the intersections (i.e. the
 * network will not be artificially connected).
 *
 * Conflation usually moves ways around by a small amount so it is best to avoid running this until
 * all other conflation is complete.
 *
 * Primarily this is useful when exporting the results out to Shapefile for programs that are
 * expecting all line strings to stop at intersections.
 */
class CreateIntersectionNodesOp : public OsmMapOperation
{
public:
  CreateIntersectionNodesOp();
};

}

#endif // CREATEINTERSECTIONNODESOP_H
