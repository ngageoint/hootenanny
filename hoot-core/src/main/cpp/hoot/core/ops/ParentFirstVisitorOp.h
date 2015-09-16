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
#ifndef PARENTFIRSTVISITOROP_H
#define PARENTFIRSTVISITOROP_H

#include "OsmMapOperation.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Visit the elements in a map where we visit the parents first and then eventually move on to the
 * children. This guarantees that elements will be visited in the following order:
 * - relations with no parents
 * - relations that have already had their parents visited
 * - ways
 * - nodes
 *
 * All elements will be visited exactly once. This method assume that there are no loops in relation
 * references.
 *
 * ### NOTE THIS IS COMPLETELY UNTESTED ###
 */
class ParentFirstVisitorOp : public OsmMapOperation
{
public:
  static string className() { return "hoot::ParentFirstVisitorOp"; }

  ParentFirstVisitorOp(shared_ptr<ElementVisitor> v);

  virtual void apply(shared_ptr<OsmMap> &map);

private:
  shared_ptr<OsmMap> _map;
  shared_ptr<ElementVisitor> _visitor;
  set<ElementId> _visited;

  void _clearCache();

  void _visit(shared_ptr<Element> e);
};

}

#endif // PARENTFIRSTVISITOROP_H
