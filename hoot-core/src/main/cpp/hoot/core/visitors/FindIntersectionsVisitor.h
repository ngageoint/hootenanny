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
#ifndef FINDINTERSECTIONSVISITOR_H
#define FINDINTERSECTIONSVISITOR_H

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

/**
 * Finds all intersections (nodes), adds some parameters to them and records their node ids
 */
class FindIntersectionsVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::FindIntersectionsVisitor"; }

  FindIntersectionsVisitor() {}

  virtual void visit(const ConstElementPtr& e);

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  std::vector<long>& getIntersections() { return _ids; }
private:

  OsmMap* _map;
  std::vector<long> _ids;
};

}

#endif // FINDINTERSECTIONSVISITOR_H
