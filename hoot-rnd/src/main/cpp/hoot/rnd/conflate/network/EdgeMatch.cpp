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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "EdgeMatch.h"

namespace hoot
{

/// @todo This and qHash are effective, but inefficient, please come up with a better method.
bool operator==(const hoot::EdgeMatchPtr& em1, const hoot::EdgeMatchPtr& em2)
{
  return em1->toString() == em2->toString();
}

uint qHash(const hoot::EdgeMatchPtr& em)
{
  return qHash(em->toString());
}

EdgeMatch::EdgeMatch()
{
  _edges1.reset(new EdgeString());
  _edges2.reset(new EdgeString());
}

shared_ptr<EdgeMatch> EdgeMatch::clone() const
{
  EdgeMatchPtr result(new EdgeMatch);
  result->_edges1 = _edges1->clone();
  result->_edges2 = _edges2->clone();

  return result;
}

bool EdgeMatch::contains(ConstNetworkEdgePtr e) const
{
  return getString1()->contains(e) || getString2()->contains(e);
}

bool EdgeMatch::overlaps(const shared_ptr<const EdgeMatch> &other) const
{
  foreach (const EdgeString::EdgeEntry& ee, _edges1->getAllEdges())
  {
    if (other->contains(ee.e))
    {
      return true;
    }
  }

  foreach (const EdgeString::EdgeEntry& ee, _edges2->getAllEdges())
  {
    if (other->contains(ee.e))
    {
      return true;
    }
  }

  return false;
}

QString EdgeMatch::toString() const
{
  return QString("s1: %1 s2: %2").arg(_edges1->toString()).arg(_edges2->toString());
}

}
