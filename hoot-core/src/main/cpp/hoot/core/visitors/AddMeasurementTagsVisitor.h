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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ADDMEASUREMENTTAGSVISITOR_H
#define ADDMEASUREMENTTAGSVISITOR_H

// Hoot
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

// Geos
#include <geos/geom/Polygon.h>

namespace hoot
{

/*
 * Visitor that calculate specific measurements of a way element and adds them as tags
 * to the element.
 * Currently supports:
 *  length and width of the aligned bounding box for:
 *    - any open and closed way
 *    - the combined open and closed way members of a relationship
 *  area for:
 *    - any closed way
 *    - the combined closed way members of a relationship where outer
 *      entries are added and inner entries are subtracted
 */
class AddMeasurementTagsVisitor : public ElementOsmMapVisitor
{
public:

  AddMeasurementTagsVisitor() = default;
  AddMeasurementTagsVisitor(bool area, bool length, bool width);
  ~AddMeasurementTagsVisitor() override = default;

  static QString className() { return "AddMeasurementTagsVisitor"; }

  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override { return "Adding measurement tags..."; }
  QString getCompletedStatusMessage() const override
  { return "Added tags to " + QString::number(_numAffected) + " elements"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Modifies map geometry as specified"; }

private:

  bool _addArea = true;
  bool _addLength = true;
  bool _addWidth = true;

  void _processRelation(const RelationPtr pRelation );
  void _processWay(const WayPtr pWay);

  void _calculateExtents(geos::geom::Geometry* pGeometry, double& length, double &width) const;
};

}

#endif // ADDMEASUREMENTTAGSVISITOR_H
