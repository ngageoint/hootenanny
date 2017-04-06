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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAYFILTER_H
#define WAYFILTER_H

#include <hoot/core/util/NotImplementedException.h>

#include "Filter.h"

namespace hoot
{
  class Way;

/**
 * Base class for filtering ways. One of the two isFiltered methods should be overridden.
 */
class WayFilter : public Filter
{
public:

  virtual ~WayFilter() {}

  /**
   * Returns true if the way should be filtered.
   */
  virtual bool isFiltered(const boost::shared_ptr<const Way>& w) const { return isFiltered(*w); }

protected:
  /**
   * A convenience method that can be overridden by the implementing class.
   */
  virtual bool isFiltered(const Way&) const { throw NotImplementedException(); }
};

}

#endif // WAYFILTER_H
