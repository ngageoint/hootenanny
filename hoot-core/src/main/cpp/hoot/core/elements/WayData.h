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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __ELEMENT_WAY_DATA_H__
#define __ELEMENT_WAY_DATA_H__

// Local
#include "NodeData.h"

// Boost
#include <boost/shared_ptr.hpp>

// Standard
#include <vector>

namespace hoot
{

class WayData : public ElementData
{
public:

  WayData(long id, long changeset = ElementData::CHANGESET_EMPTY,
          long version = ElementData::VERSION_EMPTY,
          unsigned int timestamp = ElementData::TIMESTAMP_EMPTY,
          QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
          bool visible = ElementData::VISIBLE_EMPTY);

  WayData(const WayData& from);

  virtual ~WayData();

  void addNode(long id) { _nodes.push_back(id); }

  void clear() { ElementData::clear(); _nodes.clear(); }

  const std::vector<long>& getNodeIds() const { return _nodes; }

  std::vector<long>& getNodeIds() { return _nodes; }

protected:

  std::vector<long> _nodes;
};

}

#endif // __ELEMENT_WAY_H__
