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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef STATUSUPDATEVISITOR_H
#define STATUSUPDATEVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Sets the status on elements
 *
 * @todo implement OperationStatus
 */
class StatusUpdateVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "StatusUpdateVisitor"; }

  StatusUpdateVisitor();
  StatusUpdateVisitor(Status status, bool onlyUpdateIfStatusInvalid = false);
  ~StatusUpdateVisitor() = default;

  void setConfiguration(const Settings& conf) override;

  void visit(const ElementPtr& e) override;

  QString getDescription() const override { return "Sets element statuses"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  // status to apply to elements
  Status _status;
  // if true, will only update the status if it is already set to invalid
  bool _onlyUpdateIfStatusInvalid;
};

}

#endif // STATUSUPDATEVISITOR_H
