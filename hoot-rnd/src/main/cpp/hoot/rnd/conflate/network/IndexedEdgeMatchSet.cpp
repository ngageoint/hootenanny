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
#include "IndexedEdgeMatchSet.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

IndexedEdgeMatchSet::IndexedEdgeMatchSet()
{
}

void IndexedEdgeMatchSet::addEdgeMatch(const EdgeMatchPtr& em, double score)
{
  if (!contains(em))
  {
    _matches.insert(em, score);
  }
}

shared_ptr<IndexedEdgeMatchSet> IndexedEdgeMatchSet::clone() const
{
  IndexedEdgeMatchSetPtr result(new IndexedEdgeMatchSet());
  result->_matches = _matches;
  return result;
}

bool IndexedEdgeMatchSet::contains(const EdgeMatchPtr& em) const
{
  bool result = false;

  if (_matches.contains(em))
  {
    result = true;
  }
  else
  {
    EdgeMatchPtr r = em->clone();
    r->reverse();
    if (_matches.contains(r))
    {
      result = true;
    }
  }

  return result;
}

EdgeMatchPtr IndexedEdgeMatchSet::getMatch(const EdgeMatchPtr &em) const
{
  MatchHash::const_iterator it = _matches.find(em);

  if (it == _matches.end())
  {
    throw HootException("The specified match was not found: " + em->toString());
  }

  return it.key();
}

QSet<EdgeMatchPtr> IndexedEdgeMatchSet::getMatchesWithTermination(ConstNetworkVertexPtr v1,
  ConstNetworkVertexPtr v2) const
{
  QSet<EdgeMatchPtr> result;

  /// @todo replace this with an index. This will get quite slow w/ 100k edges.

  foreach (const EdgeMatchPtr& em, _matches.keys())
  {
    if (em->getString1()->getFrom() == v1 &&
      em->getString2()->getFrom() == v2)
    {
      result.insert(em);
    }
    else if (em->getString1()->getTo() == v1 &&
      em->getString2()->getTo() == v2)
    {
      result.insert(em);
    }
  }

  return result;
}

QString IndexedEdgeMatchSet::toString() const
{
  return hoot::toString(_matches);
}

}
