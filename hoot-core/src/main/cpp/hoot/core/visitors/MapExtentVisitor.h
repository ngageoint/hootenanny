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
#ifndef MAPEXTENTVISITOR_H
#define MAPEXTENTVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>

// GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{
using namespace std;

/**
 * Determines the extent of a map
 */
class MapExtentVisitor : public ElementVisitor, public ConstOsmMapConsumer
{

public:

  static string className() { return "hoot::MapExtentVisitor"; }

  MapExtentVisitor();

  virtual ~MapExtentVisitor() {}

  Envelope getExtent() const { return _bounds; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

private:

  const OsmMap* _map;
  Envelope _bounds;

};

}

#endif // MAPEXTENTVISITOR_H
