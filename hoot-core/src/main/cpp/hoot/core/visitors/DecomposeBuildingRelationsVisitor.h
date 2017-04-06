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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DECOMPOSEBUILDINGRELATIONSVISITOR_H
#define DECOMPOSEBUILDINGRELATIONSVISITOR_H

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

class Relation;

/**
 * Despite being called "Simple", the OSM building relation spec is complicated. At the expense of
 * losing information this visitor decomposes the complex 3D buildings into simpler elements that
 * are easier to deal with and export.
 *
 * http://wiki.openstreetmap.org/wiki/Simple_3D_Buildings
 */
class DecomposeBuildingRelationsVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::DecomposeBuildingRelationsVisitor"; }

  static unsigned int logWarnCount;

  DecomposeBuildingRelationsVisitor();

  virtual void visit(const ConstElementPtr& e);

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

private:

  OsmMap* _map;
  std::vector<long> _ids;

  void _decomposeBuilding(const boost::shared_ptr<Relation>& r);
};

}

#endif // DECOMPOSEBUILDINGRELATIONSVISITOR_H
