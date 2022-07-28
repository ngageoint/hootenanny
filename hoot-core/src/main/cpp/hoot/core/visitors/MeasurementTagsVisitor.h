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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#ifndef MEASUREMENTTAGSVISITOR_H
#define MEASUREMENTTAGSVISITOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/ElementVisitor.h>

// Geos
#include <geos/geom/Polygon.h>

namespace hoot
{

/**
 * @brief The MeasurementTagsVisitor class is a base class for a Visitor that calculate specific
 *  measurements of a way element and adds them as tags to the element.
 *
 * Currently supports:
 *  length and width of the aligned bounding box for:
 *    - any open and closed way
 *    - the combined open and closed way members of a relationship
 *  area for:
 *    - any closed way
 *    - the combined closed way members of a relationship where outer
 *      entries are added and inner entries are subtracted
 */
class MeasurementTagsVisitor : public ElementOsmMapVisitor
{
public:

  MeasurementTagsVisitor() = default;
  MeasurementTagsVisitor(bool area, bool length, bool width);
  ~MeasurementTagsVisitor() override = default;

  static QString className() { return "MeasurementTagsVisitor"; }

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override { return "Calculating measurement tags..."; }
  QString getCompletedStatusMessage() const override
  { return "Calculated tags to " + QString::number(_numAffected) + " elements"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Modifies map geometry as specified"; }

  bool getArea() const { return _area; }
  void setArea(bool area) { _area = area; }
  bool getLength() const { return _length; }
  void setLength(bool length) { _length = length; }
  bool getWidth() const { return _width; }
  void setWidth(bool width) { _width = width; }

protected:

  /** Pure virtual function used in the visit() function to determine if the value should be calculated */
  virtual bool shouldCalculate(const ElementPtr& pElement) const = 0;

private:

  bool _area = true;
  bool _length = true;
  bool _width = true;

  void _processRelation(const RelationPtr pRelation );
  void _processWay(const WayPtr pWay);

  void _calculateExtents(geos::geom::Geometry* pGeometry, double& length, double &width) const;
};

}

#endif // MEASUREMENTTAGSVISITOR_H
