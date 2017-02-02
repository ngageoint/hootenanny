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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef STATUSFILTER_H
#define STATUSFILTER_H

#include "WayFilter.h"

#include <hoot/core/elements/Way.h>

namespace hoot
{

/**
 * Matches when an element's status matches the status provided.
 */
class StatusFilter : public WayFilter
{
public:

  StatusFilter(Status s) : _status(s) { }

  virtual bool isFiltered(const Way &w) const;

private:

  Status _status;
  FilterType _type;
};

}

#endif // STATUSFILTER_H
