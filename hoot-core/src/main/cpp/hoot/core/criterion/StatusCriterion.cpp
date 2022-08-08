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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "StatusCriterion.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, StatusCriterion)

StatusCriterion::StatusCriterion()
{
  setConfiguration(conf());
}

StatusCriterion::StatusCriterion(Status s) :
_status(s)
{
}

bool StatusCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(_status);
  LOG_VART(e->getStatus());
  return _status == e->getStatus();
}

void StatusCriterion::setConfiguration(const Settings& conf)
{
  _status = Status::fromString(ConfigOptions(conf).getStatusCriterionStatus());
}

QString StatusCriterion::toString() const
{
  return className() + " status: " + _status.toString();
}

}
