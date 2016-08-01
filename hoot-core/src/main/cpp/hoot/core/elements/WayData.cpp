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

#include "Way.h"

// Boost
using namespace boost;

namespace hoot
{

WayData::WayData(long id, long changeset, long version, unsigned int timestamp, QString user,
                 long uid, bool visible) :
ElementData(id, Tags(), -1, changeset, version, timestamp, user, uid, visible)
{
}

WayData::WayData(const WayData& from) :
ElementData(from.getId(), from.getTags(), from.getCircularError(), from.getChangeset(),
            from.getVersion(), from.getTimestamp(), from.getUser(), from.getUid(),
            from.getVisible()),
_nodes(from._nodes)
{

}

WayData::~WayData()
{
}

}
