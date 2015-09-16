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

#ifndef CHAINNODEFILTER_H
#define CHAINNODEFILTER_H

#include "NodeFilter.h"

// Standard
#include <vector>

namespace hoot
{
  using namespace std;

class ChainNodeFilter : public NodeFilter
{
public:

  typedef enum Aggregator
  {
    // Only filter if all the filters say it should be filtered.
    And,
    // Filter if any of the filters say it should be filtered.
    Or
  } Aggregator;

  ChainNodeFilter();

  ChainNodeFilter(shared_ptr<NodeFilter> f1);

  ChainNodeFilter(shared_ptr<NodeFilter> f1, shared_ptr<NodeFilter> f2);

  void addFilter(shared_ptr<NodeFilter> f) { _filters.push_back(f); _filtersFast.push_back(f.get());}

  virtual bool isFiltered(const shared_ptr<const Node>& n) const;

  void setAggregator(Aggregator a) { _aggregator = a; }

private:
  std::vector< shared_ptr<NodeFilter> > _filters;
  std::vector< NodeFilter* > _filtersFast;
  Aggregator _aggregator;
};

}

#endif // CHAINNODEFILTER_H
