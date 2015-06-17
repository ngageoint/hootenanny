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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAYFILTERCHAIN_H
#define WAYFILTERCHAIN_H

#include "WayFilter.h"

// Standard
#include <vector>

namespace hoot
{
  using namespace std;

class WayFilterChain : public WayFilter
{
public:
  typedef enum Aggregator
  {
    // Only filter if all the filters say it should be filtered.
    And,
    // Filter if any of the filters say it should be filtered.
    Or
  } Aggregator;

  WayFilterChain();

  WayFilterChain(shared_ptr<WayFilter> f1);

  WayFilterChain(shared_ptr<WayFilter> f1, shared_ptr<WayFilter> f2);

  void addFilter(shared_ptr<WayFilter> f) { _filters.push_back(f); _filtersFast.push_back(f.get());}

  virtual bool isFiltered(const shared_ptr<const Way>& w) const;

  void setAggregator(Aggregator a) { _aggregator = a; }

private:
  std::vector< shared_ptr<WayFilter> > _filters;
  std::vector< WayFilter* > _filtersFast;
  Aggregator _aggregator;
};

}

#endif // WAYFILTERCHAIN_H
