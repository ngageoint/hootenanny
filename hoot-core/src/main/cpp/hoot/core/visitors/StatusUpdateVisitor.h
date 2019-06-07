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
#ifndef STATUSUPDATEVISITOR_H
#define STATUSUPDATEVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Sets the status on elements
 *
 * This really should be a ElementVisitor. See #2831.
 */
class StatusUpdateVisitor : public ElementOsmMapVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::StatusUpdateVisitor"; }

  StatusUpdateVisitor();
  StatusUpdateVisitor(Status status, bool onlyUpdateIfStatusInvalid = false);

  virtual void setConfiguration(const Settings& conf);

  virtual void visit(const std::shared_ptr<Element>& e) override;

  virtual QString getDescription() const { return "Sets element statuses"; }

private:

  //status to apply to elements
  Status _status;
  //if true, will only update the status if it is already set to invalid
  bool _onlyUpdateIfStatusInvalid;
};

}

#endif // STATUSUPDATEVISITOR_H
