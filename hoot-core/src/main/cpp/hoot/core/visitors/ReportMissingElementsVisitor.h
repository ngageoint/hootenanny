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
#ifndef REPORTMISSINGELEMENTSVISITOR_H
#define REPORTMISSINGELEMENTSVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

/**
 * Reports references to missing elements in a given map. If removeMissing is set to true then
 * all missing references are removed. Note: This may not give desired results if your data
 * shouldn't contain missing references.
 */
class ReportMissingElementsVisitor : public ConstElementVisitor, public OsmMapConsumer,
  public Configurable
{
public:

  static std::string className() { return "hoot::ReportMissingElementsVisitor"; }

  ReportMissingElementsVisitor(const bool removeMissing = false,
                               const Log::WarningLevel& logLevel = Log::Trace,
                               const int maxReport = Log::getWarnMessageLimit());

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

  virtual void setConfiguration(const Settings& conf);

  void setMaxReport(int maxReport) { _maxReport = maxReport; }

  int getMissingCount() const { return _missingCount; }

  virtual QString getDescription() const
  { return "Reports references to missing elements in a map"; }

  virtual QString getInitStatusMessage() const { return "Reporting missing elements..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Reported " + QString::number(_missingCount) + " missing elements."; }

  virtual std::string getClassName() const { return className(); }

protected:

  OsmMap* _map;
  Log::WarningLevel _logLevel;
  int _maxReport;
  int _missingCount;
  bool _removeMissing;

  virtual void _reportMissing(ElementId referer, ElementId missing);
  virtual void _visitRo(ElementType type, long id);
  virtual void _visitRw(ElementType type, long id);
};

}

#endif // REPORTMISSINGELEMENTSVISITOR_H
