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
#include "NetworkMerger.h"

// hoot
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>

namespace hoot
{

NetworkMerger::NetworkMerger(const set< pair<ElementId, ElementId> >& pairs) :
  _pairs(pairs)
{
  assert(_pairs.size() == 1);
}

void NetworkMerger::apply(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  pair<ElementId, ElementId> p = *_pairs.begin();
  ElementPtr keeper = map->getElement(p.first);
  ElementPtr scrap = map->getElement(p.second);

  // use the default tag merging mechanism
  Tags newTags = TagMergerFactory::mergeTags(keeper->getTags(), scrap->getTags(), ElementType::Way);

  keeper->setTags(newTags);
  keeper->setStatus(Status::Conflated);

  // remove the duplicate element.
  ReplaceElementOp(keeper->getElementId(), scrap->getElementId()).apply(map);
  RecursiveElementRemover(scrap->getElementId()).apply(map);
  scrap->getTags().clear();

  replaced.push_back(pair<ElementId, ElementId>(scrap->getElementId(), keeper->getElementId()));
}

QString NetworkMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("NetworkMerger %1").arg(s);
}

}
