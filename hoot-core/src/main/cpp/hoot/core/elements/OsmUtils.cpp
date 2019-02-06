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

#include "OsmUtils.h"

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/conflate/poi-polygon/criterion/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/criterion/PoiPolygonPolyCriterion.h>

//Qt
#include <QDateTime>
#include <QRegExp>

using namespace geos::geom;
using namespace std;

namespace hoot
{

void OsmUtils::printNodes(const QString nodeCollectionName,
                          const QList<boost::shared_ptr<const Node> >& nodes)
{
  if (Log::getInstance().getLevel() == Log::Debug)
  {
    LOG_DEBUG(nodeCollectionName);
    LOG_VARD(nodes.size());
    for (QList<boost::shared_ptr<const Node>>::const_iterator it = nodes.begin();
         it != nodes.end(); ++it)
    {
      boost::shared_ptr<const Node> node = *it;
      LOG_VARD(node->toString());
    }
  }
}

const QList<long> OsmUtils::nodesToNodeIds(const QList<boost::shared_ptr<const Node> >& nodes)
{
  QList<long> nodeIds;
  for (QList<boost::shared_ptr<const Node> >::const_iterator it = nodes.constBegin();
       it != nodes.constEnd(); ++it)
  {
    boost::shared_ptr<const Node> node = *it;
    nodeIds.append(node->getElementId().getId());
  }
  return nodeIds;
}

QList<boost::shared_ptr<const Node> > OsmUtils::nodeIdsToNodes(const QList<long>& nodeIds,
                                                       boost::shared_ptr<const OsmMap> map)
{
  QList<boost::shared_ptr<const Node> > nodes;
  for (QList<long>::const_iterator it = nodeIds.constBegin(); it != nodeIds.constEnd(); ++it)
  {
    nodes.append(boost::dynamic_pointer_cast<const Node>(map->getElement(ElementType::Node, *it)));
  }
  return nodes;
}

Coordinate OsmUtils::nodeToCoord(boost::shared_ptr<const Node> node)
{
  return Coordinate(node->getX(), node->getY());
}

QString OsmUtils::toTimeString(quint64 timestamp)
{
  // convert time in seconds since epoch into timestamp string
  QDateTime dt;
  dt.setTimeSpec(Qt::UTC);
  dt.setMSecsSinceEpoch(timestamp*1000);
  return dt.toString("yyyy-MM-ddThh:mm:ssZ");
}

quint64 OsmUtils::fromTimeString(QString timestamp)
{
  //2016-05-04T22:07:19Z
  QRegExp timestampRegex("\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}Z*");
  if (!timestampRegex.exactMatch(timestamp))
  {
    throw IllegalArgumentException("Invalid timestamp string: " + timestamp);
  }

  struct tm t;
  strptime(timestamp.toStdString().c_str(), "%Y-%m-%dT%H:%M:%SZ", &t);

  // calc time in seconds since epoch
  return (quint64)(t.tm_sec + t.tm_min*60 + t.tm_hour*3600 + t.tm_yday*86400 +
    (t.tm_year-70)*31536000 + ((t.tm_year-69)/4)*86400 -
    ((t.tm_year-1)/100)*86400 + ((t.tm_year+299)/400)*86400);
}

QString OsmUtils::currentTimeAsString()
{
  return QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ssZ");
}

bool OsmUtils::containsTwoOrMorePois(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(poiCount);
  return poiCount >= 2;
}

bool OsmUtils::containsTwoOrMoreBuildings(ConstOsmMapPtr map)
{
  const long buildingCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new BuildingCriterion(map)),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(buildingCount);
  return buildingCount >= 2;
}

bool OsmUtils::containsTwoOrMoreAreas(ConstOsmMapPtr map)
{
  const long areaCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new NonBuildingAreaCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(areaCount);
  return areaCount >= 2;
}

bool OsmUtils::containsOnePolygonAndOnePoi(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  const long polyCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPolyCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(poiCount);
  LOG_VART(polyCount);
  return poiCount == 1 && polyCount == 1;
}

bool OsmUtils::containsPoiPolyPolys(ConstOsmMapPtr map)
{
  const long polyCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPolyCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(polyCount);
  return polyCount > 0;
}

bool OsmUtils::containsAreas(ConstOsmMapPtr map)
{
  const long areaCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new NonBuildingAreaCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(areaCount);
  return areaCount > 0;
}

bool OsmUtils::containsBuildings(ConstOsmMapPtr map)
{
  const long buildingCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new BuildingCriterion(map)),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(buildingCount);
  return buildingCount > 0;
}

bool OsmUtils::containsPois(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  return poiCount > 0;
}

QString OsmUtils::getDetailedRelationString(ConstRelationPtr& relation, const ConstOsmMapPtr& map)
{
  QString str = relation->toString() + "\nMember Detail:\n";
  const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = map->getElement(relationMembers[i].getElementId());
    str += member->toString() + "\n";
  }
  return str;
}

}
