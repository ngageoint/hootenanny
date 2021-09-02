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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MultiaryUtilities.h"

// hoot
#include <hoot/core/conflate/SearchBoundsCalculator.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmPbfWriter.h>
#include <hoot/rnd/conflate/multiary/MultiaryPoiMergerCreator.h>
#include <hoot/rnd/visitors/MultiaryPoiHashVisitor.h>

namespace hoot
{

void MultiaryUtilities::conflate(OsmMapPtr map)
{
  MatchFactory& matchFactory = MatchFactory::getInstance();
  matchFactory.reset();
  matchFactory.registerCreator("ScriptMatchCreator,MultiaryPoi.js");

  MergerFactory::getInstance().reset();
  std::shared_ptr<MergerFactory> mergerFactory(new MergerFactory());
  mergerFactory->registerCreator(std::make_shared<MultiaryPoiMergerCreator>());

  // Apparently, multiary will allow matches with > 1.0 review thresholds.
  std::shared_ptr<MatchThreshold> mt = std::make_shared<MatchThreshold>(0.39, 0.61, 1.1, false);

  // call new conflation routine
  UnifyingConflator conflator(mt);
  conflator.setMergerFactory(mergerFactory);
  conflator.apply(map);
}

QByteArray MultiaryUtilities::convertElementToPbf(ConstNodePtr n)
{
  std::stringstream ss;
  OsmPbfWriter writer;
  writer.writePb(n, &ss);

  QByteArray result(ss.str().data(), ss.str().size());
  return result;
}

QList<MultiaryElement> MultiaryUtilities::conflateCluster(QList<QByteArray> pbfElements)
{
  QList<MultiaryElement> result;

  OsmMapPtr map = std::make_shared<OsmMap>();

  OsmMapPtr tmpMap = std::make_shared<OsmMap>();
  foreach (const QByteArray& ba, pbfElements)
  {
    tmpMap->clear();
    OsmPbfReader tmpReader;
    tmpReader.setUseDataSourceIds(false);
    tmpReader.parseElements(ba, tmpMap);
    if (tmpMap->getElementCount() != 1)
    {
      throw IllegalArgumentException("Expected exactly one element in each pbfElements entry.");
    }
    ElementPtr n = tmpMap->getNodes().begin()->second->cloneSp();
    n->setStatus(Status::fromInput(0));
    map->addElement(n);
  }

  // we only work with nodes right now.
  assert(map->getWayCount() == 0);
  assert(map->getRelationCount() == 0);

  MapProjector::projectToPlanar(map);
  conflate(map);
  MapProjector::projectToWgs84(map);

  MultiaryPoiHashVisitor hashVisitor;
  hashVisitor.setIncludeCircularError(true);
  map->visitRw(hashVisitor);

  for (NodeMap::const_iterator it = map->getNodes().begin(); it != map->getNodes().end(); ++it)
  {
    MultiaryElement me;
    NodePtr n = it->second;
    me.setHash(n->getTags().get(MetadataTags::HootHash()));
    me.setBounds(
      getInstance().getBoundsCalculator()->calculateSearchBounds(
        map, std::dynamic_pointer_cast<Node>(n)));
    me.setPayload(convertElementToPbf(n));
    result.append(me);
  }

  return result;
}

QList<MultiarySimpleMatch> MultiaryUtilities::findMatches(
  QByteArray checkElement, QList<QByteArray> againstElements)
{
  QList<MultiarySimpleMatch> result;
  OsmPbfReader reader;
  reader.setUseDataSourceIds(false);

  OsmMapPtr map = std::make_shared<OsmMap>();

  reader.parseElements(checkElement, map);
  if (map->getElementCount() != 1)
  {
    throw IllegalArgumentException("Expected exactly one element in checkElement.");
  }
  // we only work with nodes right now.
  assert(map->getNodeCount() == 1);

  NodePtr check = map->getNodes().begin()->second;

  QList<int> ids;
  OsmMapPtr tmpMap = std::make_shared<OsmMap>();
  foreach (const QByteArray& ba, againstElements)
  {
    tmpMap->clear();
    OsmPbfReader tmpReader;
    tmpReader.setUseDataSourceIds(false);
    tmpReader.parseElements(ba, tmpMap);
    if (tmpMap->getElementCount() != 1)
    {
      throw IllegalArgumentException("Expected exactly one element in each againstElements entry.");
    }
    ids.append(tmpMap->getNodes().begin()->first);
    map->addElement(tmpMap->getNodes().begin()->second->cloneSp());
  }

  // we only work with nodes right now.
  assert(map->getWayCount() == 0);
  assert(map->getRelationCount() == 0);

  MapProjector::projectToPlanar(map);

  MatchFactory& matchFactory = MatchFactory::getInstance();
  matchFactory.reset();
  matchFactory.registerCreator("ScriptMatchCreator,MultiaryPoi.js");

  for (int i = 0; i < ids.size(); i++)
  {
    MatchPtr m = matchFactory.createMatch(map, check->getElementId(), ElementId::node(ids[i]));
    if (m && m->getProbability() > 0)
    {
      result.append(MultiarySimpleMatch(i, m->getProbability()));
    }
  }

  return result;
}

SearchBoundsCalculatorPtr MultiaryUtilities::getBoundsCalculator()
{
  if (_searchBoundsCalculator.get() == nullptr)
  {
    // find a match creator that can provide the search bounds.
    foreach (std::shared_ptr<MatchCreator> mc, MatchFactory::getInstance().getCreators())
    {
      SearchRadiusProviderPtr sbc = std::dynamic_pointer_cast<SearchRadiusProvider>(mc);

      if (sbc.get())
      {
        if (_searchBoundsCalculator.get())
        {
          LOG_WARN("Found more than one bounds calculator. Using the first one.");
        }
        else
        {
          _searchBoundsCalculator = std::make_shared<SearchBoundsCalculator>(sbc);
        }
      }
    }
  }

  return _searchBoundsCalculator;
}

MultiaryUtilities& MultiaryUtilities::getInstance()
{
  //  Local static singleton instance
  static MultiaryUtilities instance;
  return instance;
}

}
