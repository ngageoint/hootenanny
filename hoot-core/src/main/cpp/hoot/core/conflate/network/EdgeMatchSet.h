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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __EDGE_MATCH_SET_H__
#define __EDGE_MATCH_SET_H__

#include <hoot/core/conflate/network/EdgeMatch.h>

namespace hoot
{

class EdgeMatchSet
{
public:

  EdgeMatchSet() = default;
  virtual ~EdgeMatchSet() = default;

  virtual bool contains(const ConstEdgeMatchPtr &em) const = 0;

  virtual QString toString() const = 0;
};

using EdgeMatchSetPtr = std::shared_ptr<EdgeMatchSet>;
using ConstEdgeMatchSetPtr = std::shared_ptr<const EdgeMatchSet>;

// not implemented
bool operator<(ConstEdgeMatchSetPtr, ConstEdgeMatchSetPtr);

}

#endif // __EDGE_MATCH_SET_H__
