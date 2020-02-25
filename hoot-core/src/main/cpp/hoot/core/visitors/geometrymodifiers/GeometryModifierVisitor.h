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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GEOMETRYMODIFIERVISITOR_H
#define GEOMETRYMODIFIERVISITOR_H

#include "GeometryModifierAction.h"

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/*
 * Visitor utilized by the GeometryModifierOp.
 * Processes an element as requested in the currently set GeometryModifierActionDesc.
 * Applies the filter to the node and if it matches calls the requested modifier's
 * process function.
 */
class GeometryModifierVisitor : public ElementVisitor
{
  friend class GeometryModifierOp;

public:
  virtual void setOsmMap(OsmMap* pMap) { _pMap = pMap; }

  void setActionDesc(GeometryModifierActionDesc actionDesc ) { _actionDesc = actionDesc; }

  // ElementVisitor
  static std::string className() { return "hoot::GeometryModifierVisitor"; }
  QString getDescription() const { return "Modifies map geometry as specified"; }

  // visits the element, checks if it matches the filter set in _actionDesc and
  // calls the action's process function
  virtual void visit(const ElementPtr& e) override;

  // OperationStatusInfo
  virtual QString getInitStatusMessage() const { return "Modifying geometry..."; }
  virtual QString getCompletedStatusMessage() const
  { return "Modified " + QString::number(_numAffected) + " elements"; }

  virtual std::string getClassName() const { return className(); }

private:
  OsmMap* _pMap;
  GeometryModifierActionDesc _actionDesc;
};

}

#endif // GEOMETRYMODIFIERVISITOR_H
