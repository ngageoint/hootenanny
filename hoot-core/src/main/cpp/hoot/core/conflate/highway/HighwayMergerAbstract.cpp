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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwayMergerAbstract.h"

// geos
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveReviewsByEidOp.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/visitors/ExtractWaysVisitor.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/RemoveElementOp.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

unsigned int HighwayMergerAbstract::logWarnCount = 0;

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
  LOG_VART(eid1);
  LOG_VART(eid2);

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
    LOG_TRACE("Missing match pair.");
    if (!e1)
    {
      LOG_TRACE(eid1 << " is missing.");
    }
    if (!e2)
    {
      LOG_TRACE(eid2 << " is missing.");
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

