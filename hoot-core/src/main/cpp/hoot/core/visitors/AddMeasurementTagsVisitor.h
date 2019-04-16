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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADDMEASUREMENTTAGSVISITOR_H
#define ADDMEASUREMENTTAGSVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

class AddMeasurementTagsVisitor : public ElementOsmMapVisitor, public OperationStatusInfo
{
public:
  AddMeasurementTagsVisitor() {}
  AddMeasurementTagsVisitor(bool area, bool length, bool width) :
    _addArea(area), _addLength(length), _addWidth(width) {}

  // ElementVisitor
  static std::string className() { return "hoot::AddMeasurementTagsVisitor"; }
  QString getDescription() const { return "Modifies map geometry as specified"; }

  virtual void visit(const ElementPtr& e);

  // OperationStatusInfo
  virtual QString getInitStatusMessage() const { return "Adding measurement tags..."; }
  virtual QString getCompletedStatusMessage() const { return "Added tags to " + QString::number(_numAffected) + " elements"; }

private:
  bool _addArea = true;
  bool _addLength = true;
  bool _addWidth = true;
};

} // namespace hoot

#endif // ADDMEASUREMENTTAGSVISITOR_H
