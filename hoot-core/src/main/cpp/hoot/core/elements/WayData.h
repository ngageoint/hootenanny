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
#ifndef __ELEMENT_WAY_DATA_H__
#define __ELEMENT_WAY_DATA_H__

// Local
#include <hoot/core/elements/ElementData.h>

// Standard
#include <vector>

namespace hoot
{

class WayData : public ElementData
{
public:

  static long PID_EMPTY;

  WayData(long id, long changeset = ElementData::CHANGESET_EMPTY,
          long version = ElementData::VERSION_EMPTY,
          unsigned int timestamp = ElementData::TIMESTAMP_EMPTY,
          QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
          bool visible = ElementData::VISIBLE_EMPTY, long pid = WayData::PID_EMPTY);

  WayData(const WayData& from);

  virtual ~WayData() = default;

  void addNode(long id) { _nodes.push_back(id); }
  void insertNode(long index, long id ) { _nodes.insert(_nodes.begin() + index, id); }

  void clear();

  const std::vector<long>& getNodeIds() const { return _nodes; }

  std::vector<long>& getNodeIds() { return _nodes; }

  long getPid() const { return _pid; }
  void setPid(long pid) { _pid = pid; }

protected:

  /** List of node IDs in the way */
  std::vector<long> _nodes;

  /** Parent ID, i.e. the ID of the way that this way was split from or created from */
  long _pid;
};

}

#endif // __ELEMENT_WAY_H__
