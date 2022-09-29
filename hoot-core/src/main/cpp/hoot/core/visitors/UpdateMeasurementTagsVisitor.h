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
#ifndef UPDATEMEASUREMENTTAGSVISITOR_H
#define UPDATEMEASUREMENTTAGSVISITOR_H

// Hoot
#include <hoot/core/visitors/MeasurementTagsVisitor.h>

// Geos
#include <geos/geom/Polygon.h>

namespace hoot
{

/**
 * @brief The UpdateMeasurementTagsVisitor class is a Visitor that calculate specific measurements of a
 * way element and updates them as tags to the element that already has those tags
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
class UpdateMeasurementTagsVisitor : public MeasurementTagsVisitor
{
public:

  UpdateMeasurementTagsVisitor() = default;
  ~UpdateMeasurementTagsVisitor() override = default;

  static QString className() { return "UpdateMeasurementTagsVisitor"; }

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override { return "Update existing measurement tags..."; }
  QString getCompletedStatusMessage() const override
  { return QString("Updated tags on %1 elements").arg(QString::number(_numAffected)); }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Modifies map geometry as specified"; }

protected:
  /** See MeasurementTagsVisitor::shouldCalculate() */
  bool shouldCalculate(const ElementPtr& /*pElement*/) const override { return _shouldCalculate; }

  bool _shouldCalculate = false;

};

}

#endif // UPDATEMEASUREMENTTAGSVISITOR_H
