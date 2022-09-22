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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "RemoveDuplicateAreasVisitor.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagDifferencer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CompletelyContainedByMapElementVisitor.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveDuplicateAreasVisitor)

RemoveDuplicateAreasVisitor::RemoveDuplicateAreasVisitor()
{
  _diff = Factory::getInstance().constructObject<TagDifferencer>(ConfigOptions().getRemoveDuplicateAreasDiff());
}

std::shared_ptr<Geometry> RemoveDuplicateAreasVisitor::_convertToGeometry(const std::shared_ptr<Element>& e1)
{
  auto it = _geoms.find(e1->getElementId());
  if (it != _geoms.end())
    return it.value();
  ElementToGeometryConverter gc(_map->shared_from_this());
  std::shared_ptr<Geometry> g = gc.convertToGeometry(e1);
  _geoms[e1->getElementId()] = g;
  return g;
}

bool RemoveDuplicateAreasVisitor::_equals(const std::shared_ptr<Element>& e1, const std::shared_ptr<Element>& e2)
{
  if (e1->getStatus() != e2->getStatus())
    return false;

  // if e1 is a parent of e2 or vice versa, then don't consider them duplicates.
  if (_map->getIndex().getParents(e1->getElementId()).count(e2->getElementId()) == 1 ||
      _map->getIndex().getParents(e2->getElementId()).count(e1->getElementId()) == 1)
    return false;

  // if they effectively have different tags
  if (_diff->diff(_map->shared_from_this(), e1, e2) > 0.0)
    return false;

  // convert to geometry and cache as relevant.
  std::shared_ptr<Geometry> g1 = _convertToGeometry(e1);
  std::shared_ptr<Geometry> g2 = _convertToGeometry(e2);
  if (!g1 || g1->isEmpty() || !g2 || g2->isEmpty())
    return false;

  double a1 = g1->getArea();
  double a2 = g2->getArea();

  // if there is a significant difference in area, they aren't a match
  if (fabs(a1 - a2) >= (a1 * 1e-9) && a1 > 0.0 && a2 > 0.0)
    return false;

  // calculate the overlap of the areas
  std::shared_ptr<Geometry> overlap;
  try
  {
    overlap = std::shared_ptr<Geometry>(g1->intersection(g2.get()));
  }
  catch (const geos::util::TopologyException&)
  {
    g1.reset(GeometryUtils::validateGeometry(g1.get()));
    g2.reset(GeometryUtils::validateGeometry(g2.get()));
    overlap = g1->intersection(g2.get());
  }

  double ao = overlap->getArea();
  // if the overlap is outside double precision
  if (fabs(ao - a1) >= (a1 * 1e-9))
    return false;

  return true;
}

void RemoveDuplicateAreasVisitor::_removeOne(const std::shared_ptr<Element>& e1,
                                            const std::shared_ptr<Element>& e2)
{
  if (e1->getTagCount() > e2->getTagCount())
    RecursiveElementRemover(e2->getElementId()).apply(_map->shared_from_this());
  else if (e1->getTagCount() < e2->getTagCount())
    RecursiveElementRemover(e1->getElementId()).apply(_map->shared_from_this());
  else if (e1->getId() < e2->getId())
    RecursiveElementRemover(e1->getElementId()).apply(_map->shared_from_this());
  else
    RecursiveElementRemover(e2->getElementId()).apply(_map->shared_from_this());
  _numAffected++;
}

void RemoveDuplicateAreasVisitor::visit(const std::shared_ptr<Element>& e)
{
  if (!e.get())
    return;
  LOG_VART(e->getElementId());

  if (e->getElementType() != ElementType::Node)
  {
    AreaCriterion areaCrit;
    areaCrit.setOsmMap(_map);
    std::shared_ptr<Envelope> env(e->getEnvelope(_map->shared_from_this()));

    // if the envelope is null or the element is incomplete
    if (env->isNull() ||
        CompletelyContainedByMapElementVisitor::isComplete(_map, e->getElementId()) == false ||
        areaCrit.isSatisfied(e) == false)
    {
      LOG_TRACE("Envelope null or incomplete element.");
      return;
    }

    set<ElementId> neighbors = _map->getIndex().findWayRelations(*env); // LARGEST BOTTLENECK
    LOG_VART(neighbors.size());

    for (const auto eid : neighbors)
    {
      if (e->getElementId() < eid && eid.getType() != ElementType::Node)
      {
        std::shared_ptr<Element> e2 = _map->getElement(eid);
        // check to see if e2 is null, it is possible that we removed it w/ a previous call to
        // remove a parent. run _equals() first as it is much faster than isSatisfied() (which
        // ends up doing lots of regex matching)
        if (e2 != nullptr && _equals(e, e2) && areaCrit.isSatisfied(e2))
        {
          LOG_TRACE("e2 is area and e1/e2 equal.");
          // remove the crummier one.
          _removeOne(e, e2);
          // if we've deleted the element we're visiting.
          if (_map->containsElement(e) == false)
            return;
        }
      }
    }
  }

  // TODO: let's give this the PoiPolygonMatchVisitor status update treatment to throttle the
  // interval down for slower jobs
  _numProcessed++;
  if (_numProcessed % 10000 == 0)
  {
    PROGRESS_INFO("\tProcessed " << StringUtils::formatLargeNumber(_numProcessed) << " elements for duplicate area removal.");
  }
}

QStringList RemoveDuplicateAreasVisitor::getCriteria() const
{
  QStringList criteria;
  // need to also add building here for unecessary conflation op removal purposes, since
  // AreaCriterion includes buildings in some instances
  criteria.append(BuildingCriterion::className());
  criteria.append(AreaCriterion::className());
  return criteria;
}

}
