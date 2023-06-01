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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVEMISSINGELEMENTSVISITOR_H
#define REMOVEMISSINGELEMENTSVISITOR_H

// hoot
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Removes non-existent element references from relations or ways
 */
class RemoveMissingElementsVisitor : public ConstElementVisitor, public OsmMapConsumerImpl
{
public:

  static QString className() { return "RemoveMissingElementsVisitor"; }

  RemoveMissingElementsVisitor(const Log::WarningLevel& logLevel = Log::Trace,
                               const int maxReport = Log::getWarnMessageLimit());
  ~RemoveMissingElementsVisitor() override = default;

  void setOsmMap(OsmMap* map) override;

  /**
   * @see ElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Removing references to elements that do not exist..."; }
  QString getCompletedStatusMessage() const override
  {
    return QString("Removed %1 missing element child references").arg(StringUtils::formatLargeNumber(_numAffected)); }

  QString getDescription() const override
  { return "Removes references to any elements that do not exist"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  std::shared_ptr<ReportMissingElementsVisitor> _v;
};

}

#endif // REMOVEMISSINGELEMENTSVISITOR_H
