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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVEMISSINGELEMENTSVISITOR_H
#define REMOVEMISSINGELEMENTSVISITOR_H

#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * Removes non-existent element references from relations or ways
 */
class RemoveMissingElementsVisitor : public ConstElementVisitor, public OsmMapConsumer,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::RemoveMissingElementsVisitor"; }

  RemoveMissingElementsVisitor(const Log::WarningLevel& logLevel = Log::Trace,
                               const int maxReport = Log::getWarnMessageLimit());

  virtual void setOsmMap(OsmMap* map) { _v->setOsmMap(map);}

  virtual void setOsmMap(const OsmMap* /*map*/)
  { throw NotImplementedException("Set Map with const is not supported"); }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getInitStatusMessage() const
  { return "Removing references to elements that do not exist..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " missing element child references"; }

  virtual QString getDescription() const
  { return "Removes references to any elements that do not exist"; }

private:

  std::shared_ptr<ReportMissingElementsVisitor> _v;
};

}

#endif // REMOVEMISSINGELEMENTSVISITOR_H
