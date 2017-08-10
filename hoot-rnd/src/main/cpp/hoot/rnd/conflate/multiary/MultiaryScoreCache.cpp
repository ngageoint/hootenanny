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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiaryScoreCache.h"

// boost
#include <boost/scoped_ptr.hpp>

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/MatchClassification.h>

namespace hoot
{

MultiaryScoreCache::MultiaryScoreCache(ConstOsmMapPtr map,
  boost::shared_ptr<MatchCreator> matchCreator) :
  _map(map),
  _matchCreator(matchCreator)
{
}

MatchClassification MultiaryScoreCache::getScore(MultiaryClusterPtr c1, MultiaryClusterPtr c2)
{
  // make sure all the elements in c1 can possibly (regardless of how remotely) be merged with
  // any of the elements in c2.
  foreach (const ConstElementPtr& e1, *c1)
  {
    foreach (const ConstElementPtr& e2, *c2)
    {
      MatchClassification mc = getScore(e1, e2);
      if (mc.getMissP() == 1)
      {
        return mc;
      }
    }
  }

  return getScore(c1->mergedElement, c2->mergedElement);
}

MatchClassification MultiaryScoreCache::getScore(ConstElementPtr e1, ConstElementPtr e2)
{
  _lastExplainText.clear();

  boost::scoped_ptr<Match> m(
    _matchCreator->createMatch(_map, e1->getElementId(), e2->getElementId()));

  // default to a hard miss.
  MatchClassification result(0, 1, 0);

  // if the MatchCreator returns a valid match class, use that as the score.
  if (m)
  {
    result = m->getClassification();
    _lastExplainText = m->explain();
  }

  return result;
}

}
