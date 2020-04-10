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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwayMergerAbstract.h"

// hoot
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int HighwayMergerAbstract::logWarnCount = 0;

void HighwayMergerAbstract::_markNeedsReview(const OsmMapPtr &map, ElementPtr e1, ElementPtr e2,
  QString note, QString reviewType)
{
  if (!e1 && !e2)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Unable to mark element as needing review. Neither element exists. " << note);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  else if (e1 && e2)
  {
    _reviewMarker.mark(map, e1, e2, note, reviewType);
  }
  else if (e1)
  {
    _reviewMarker.mark(map, e1, note, reviewType);
  }
  else if (e2)
  {
    _reviewMarker.mark(map, e2, note, reviewType);
  }
}

bool HighwayMergerAbstract::_mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  vector<pair<ElementId, ElementId>>& /*replaced*/)
{
  OsmMapPtr result = map;

  ElementPtr e1 = result->getElement(eid1);
  ElementPtr e2 = result->getElement(eid2);

  // if the element is no longer part of the map. This can happen in rare cases where a match may
  // not conflict with any one match in the set, but may conflict with multiple matches in the
  // set. E.g.
  // A x-------x
  // B    x--------x
  // C         x--------x
  //
  // In the above example no two matches conflict, but B conflicts with (A U C). We don't determine
  // this in the conflict code at this time, so we'll ignore the merge.
  if (!e1 || !e2)
  {
    if (!e1)
    {
      LOG_TRACE(eid1 << " is missing.");
    }
    if (!e2)
    {
      LOG_TRACE(eid2 << " is missing.");
    }
    if (e1 || e2)
    {
      QString msg = "Marking review for ";
      if (e1)
      {
        msg += e1->getElementId().toString();
      }
      if (e2)
      {
        msg += " and " + e2->getElementId().toString();
      }
      msg += "...";
      LOG_TRACE(msg);
    }

    _markNeedsReview(result, e1, e2, "Missing match pair", HighwayMatch::getHighwayMatchName());
    return true;
  }
  return false;
}

QString HighwayMergerAbstract::toString() const
{
  return QString("HighwayMerger, pairs: ") + hoot::toString(_pairs);
}

}

