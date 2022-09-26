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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "StatusUpdateVisitor.h"

// hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, StatusUpdateVisitor)

StatusUpdateVisitor::StatusUpdateVisitor()
{
  setConfiguration(conf());
}

StatusUpdateVisitor::StatusUpdateVisitor(Status status, bool onlyUpdateIfStatusInvalid)
  : _status(status),
    _onlyUpdateIfStatusInvalid(onlyUpdateIfStatusInvalid)
{
  LOG_VART(_status);
  LOG_VART(_onlyUpdateIfStatusInvalid);
}

void StatusUpdateVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _onlyUpdateIfStatusInvalid = configOptions.getStatusUpdateVisitorOnlyUpdateInvalidStatus();
  if (configOptions.getStatusUpdateVisitorStatus().trimmed().isEmpty())
    _status = Status::Invalid;
  else
    _status = Status::fromString(configOptions.getStatusUpdateVisitorStatus());
  LOG_VART(_status);
  LOG_VART(_onlyUpdateIfStatusInvalid);
}

void StatusUpdateVisitor::visit(const ElementPtr& e)
{
  LOG_VART(e->getStatus());
  if (_onlyUpdateIfStatusInvalid && e->getStatus() != Status::Invalid)
    return;

  e->setStatus(_status);
  e->getTags()[MetadataTags::HootStatus()] = _status.toString();
}

}
