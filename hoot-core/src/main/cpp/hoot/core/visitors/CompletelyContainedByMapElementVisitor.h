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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COMPLETELYCONTAINEDBYMAPELEMENTVISITOR_H
#define COMPLETELYCONTAINEDBYMAPELEMENTVISITOR_H

// geos
#include <geos/geom/MultiLineString.h>

// hoot
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

// standard
#include <string>

namespace hoot
{

class Relation;
class Way;


/**
 * Checks to see if the elements visited are completely contained by the specified map. This is
 * relevant during tile based operations because not all elements may be completely represented
 * within a tile.
 */
class CompletelyContainedByMapElementVisitor : public ElementConstOsmMapVisitor
{
public:

  static std::string className() { return "hoot::CompletelyContainedByMapElementVisitor"; }

  CompletelyContainedByMapElementVisitor();

  /**
   * Returns true if all the children of this element are available within the specified map.
   */
  static bool isComplete(const OsmMap* map, ElementId eid);

  bool isComplete() const { return _complete; }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const
  { return "Determines if a element is completely contained within a map"; }

  virtual std::string getClassName() const { return className(); }

protected:

  bool _complete;

  bool _isComplete(const std::shared_ptr<const Element>& e);

  void _visit(const std::shared_ptr<const Way>& w);

  void _visit(const std::shared_ptr<const Relation>& r);
};

}

#endif // COMPLETELYCONTAINEDBYMAPELEMENTVISITOR_H
