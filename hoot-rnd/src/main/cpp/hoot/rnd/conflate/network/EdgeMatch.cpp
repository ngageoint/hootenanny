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

/// @todo This could be made faster by only looking at the pointer for comparison. Unfortunately
/// this would likely break IndexedEdgeMatchSet::contains. Another data structure in there could
/// certainly alleviate this.
bool operator==(const hoot::ConstEdgeMatchPtr& em1, const hoot::ConstEdgeMatchPtr& em2)
{
  bool result = em1.get() == em2.get() ||
    (em1->getString1() == em2->getString1() && em1->getString2() == em2->getString2());

//  bool strResult = em1->toString() == em2->toString();

//  if (result != strResult)
//  {
//    LOG_VARE(result);
//    LOG_VARE(strResult);
//    LOG_VARE(em1);
//    LOG_VARE(em2);
//    throw HootException();
//  }

  return result;
}

EdgeMatch::EdgeMatch()
{
  _edges1.reset(new EdgeString());
  _edges2.reset(new EdgeString());
  _resetHash();
}

EdgeMatch::EdgeMatch(ConstEdgeStringPtr es1, ConstEdgeStringPtr es2) :
  _edges1(es1->clone()),
  _edges2(es2->clone())
{
  _resetHash();
}

shared_ptr<EdgeMatch> EdgeMatch::clone() const
{
  EdgeMatchPtr result(new EdgeMatch);
  result->_edges1 = _edges1->clone();
  result->_edges2 = _edges2->clone();

  return result;
}

bool EdgeMatch::contains(const shared_ptr<const EdgeMatch>& other) const
{
  return getString1()->contains(other->getString1()) &&
    getString2()->contains(other->getString2());
}

bool EdgeMatch::contains(ConstNetworkEdgePtr e) const
{
  return getString1()->contains(e) || getString2()->contains(e);
}

bool EdgeMatch::contains(ConstNetworkVertexPtr v) const
{
  return getString1()->contains(v) || getString2()->contains(v);
}

bool EdgeMatch::overlaps(const shared_ptr<const EdgeMatch> &other) const
{
  if (other->getString1()->overlaps(getString1()))
  {
    LOG_INFO("Overlaps: " << toString() << " " << other);
    return true;
  }

  if (other->getString2()->overlaps(getString2()))
  {
    LOG_INFO("Overlaps: " << toString() << " " << other);
    return true;
  }

  return false;
}

QString EdgeMatch::toString() const
{
  return QString("s1: %1 s2: %2").arg(_edges1->toString()).arg(_edges2->toString());
}

}
