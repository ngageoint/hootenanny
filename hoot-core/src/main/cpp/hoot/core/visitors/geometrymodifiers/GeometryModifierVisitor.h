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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef GEOMETRYMODIFIERVISITOR_H
#define GEOMETRYMODIFIERVISITOR_H

#include "GeometryModifierAction.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * @brief The GeometryModifierVisitor class is a Visitor utilized by the GeometryModifierOp.
 *
 * It Pprocesses an element as requested in the currently set GeometryModifierActionDesc, applies
 * the filter to the node and if it matches calls the requested modifier's process function.
 */
class GeometryModifierVisitor : public ElementVisitor
{
  friend class GeometryModifierOp;

public:

  GeometryModifierVisitor() = default;
  ~GeometryModifierVisitor() override = default;

  // TODO: implement OsmMapConsumer instead
  void setOsmMap(OsmMap* pMap) { _pMap = pMap; }

  static QString className() { return "GeometryModifierVisitor"; }

  // visits the element, checks if it matches the filter set in _actionDesc and
  // calls the action's process function
  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override { return "Modifying geometry..."; }
  QString getCompletedStatusMessage() const override
  { return "Modified " + QString::number(_numAffected) + " elements"; }

  QString getDescription() const override { return "Modifies map geometry as specified"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

   void setActionDesc(const GeometryModifierActionDesc& actionDesc ) { _actionDesc = actionDesc; }

private:

  OsmMap* _pMap;
  GeometryModifierActionDesc _actionDesc;
};

}

#endif // GEOMETRYMODIFIERVISITOR_H
