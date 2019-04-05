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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GEOMETRYMODIFIERVISITOR_H
#define GEOMETRYMODIFIERVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

class GeometryModifierVisitor : public ElementVisitor, public OperationStatusInfo
{
  friend class GeometryModifierOp;

public:
  GeometryModifierVisitor();

  virtual void setOsmMap(OsmMap* pMap) { _pMap = pMap; }

  // ElementVisitor
  static std::string className() { return "hoot::GeometryModifierVisitor"; }
  virtual void visit(const ElementPtr& e);
  QString getDescription() const { return "Modifies map geometry as specified"; }

  // OperationStatusInfo
  virtual QString getInitStatusMessage() const { return "Modifying geometry..."; }
  virtual QString getCompletedStatusMessage() const { return "Modified " + QString::number(_numAffected) + " elements"; }

private:
  void extrude(const WayPtr& pWay, double width);

  OsmMap* _pMap;
};

}

#endif // GEOMETRYMODIFIERVISITOR_H
