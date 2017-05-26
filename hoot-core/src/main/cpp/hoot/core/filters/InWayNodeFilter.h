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

#ifndef INWAYNODEFILTER_H
#define INWAYNODEFILTER_H

#include "NodeFilter.h"

// Standard
#include <set>
#include <vector>

namespace hoot
{

class OsmMap;

/**
 * The filter matches all nodes that are in at least one of the ways referenced by wids.
 */
class InWayNodeFilter : public NodeFilter
{
public:
  InWayNodeFilter(FilterType type, const OsmMap& map, const std::vector<long>& wids);

protected:
  virtual bool isFiltered(const Node& n) const;

private:
  std::set<long> _nids;
  FilterType _type;
};

}

#endif // INWAYNODEFILTER_H
