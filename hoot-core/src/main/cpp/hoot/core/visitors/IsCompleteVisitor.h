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
#ifndef ISCOMPLETEVISITOR_H
#define ISCOMPLETEVISITOR_H

// geos
#include <geos/geom/MultiLineString.h>

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>

// standard
#include <string>

// tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
class Relation;
class Way;

using namespace std;

/**
 * Checks to see if the elements visited are completely contained by the specified map. This is
 * relevant during tile based operations because not all elements may be completely represented
 * within a tile.
 */
class IsCompleteVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:
  static string className() { return "hoot::IsCompleteVisitor"; }

  IsCompleteVisitor();

  /**
   * Returns true if all the children of this element are available within the specified map.
   */
  static bool isComplete(const OsmMap* map, ElementId eid);

  bool isComplete() const { return _complete; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

protected:
  const OsmMap* _map;
  bool _complete;

  bool _isComplete(const shared_ptr<const Element>& e);

  void _visit(const shared_ptr<const Way>& w);

  void _visit(const shared_ptr<const Relation>& r);
};

}

#endif // ISCOMPLETEVISITOR_H
