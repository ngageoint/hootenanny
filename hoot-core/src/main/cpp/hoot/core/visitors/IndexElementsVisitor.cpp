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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "IndexElementsVisitor.h"

#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

IndexElementsVisitor::IndexElementsVisitor(shared_ptr<HilbertRTree>& index,
                                           deque<ElementId>& indexToEid,
                                           const shared_ptr<ElementCriterion>& filter,
                                           boost::function<Meters (const ConstElementPtr& e)> getSearchRadius,
                                           ConstOsmMapPtr pMap):
  _pMap(pMap),
  _filter(filter),
  _getSearchRadius(getSearchRadius),
  _index(index),
  _indexToEid(indexToEid)
{
  //_getSearchRadius = getSearchRadius;
}

void IndexElementsVisitor::setOsmMap(OsmMap* map)
{
  // Do nothing
  (void) map;
}

void IndexElementsVisitor::visit(const ConstElementPtr& e)
{
  if (_filter->isSatisfied(e))
  {
    _fids.push_back((int)_indexToEid.size());
    _indexToEid.push_back(e->getElementId());

    Box b(2);
    Meters searchRadius = _getSearchRadius(e);
    auto_ptr<Envelope> env(e->getEnvelope(_pMap));
    env->expandBy(searchRadius);
    b.setBounds(0, env->getMinX(), env->getMaxX());
    b.setBounds(1, env->getMinY(), env->getMaxY());

    _boxes.push_back(b);
  }
}

} // End namespace hoot
