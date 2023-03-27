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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#include "MissingElementRetrievalOp.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmApiReader.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/OsmApiUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MissingElementRetrievalOp)

MissingElementRetrievalOp::MissingElementRetrievalOp()
{
  //  This operator requires that missing child references aren't removed (or nothing would be downloaded)
  conf().set(ConfigOptions().getMapReaderAddChildRefsWhenMissingKey(), true);
  //  Validate the retrieval URL
  setApiEndpointUrl(ConfigOptions().getMissingElementRetrievalUrl());
}

void MissingElementRetrievalOp::apply(std::shared_ptr<OsmMap>& map)
{
  std::set<ElementId> missingElements;
  //  Iterate all of the ways looking for incomplete ways
  const WayMap& ways = map->getWays();
  for (const auto& way : ways)
  {
    WayPtr w = way.second;
    const std::vector<long>& nodes = w->getNodeIds();
    bool request_way = false;
    for (auto node_id : nodes)
    {
      //  Add the node to the list of missing elements
      if (!map->containsNode(node_id))
      {
        request_way = true;
        break;
      }
    }
    if (request_way)
      missingElements.emplace(w->getElementId());
  }
  //  Iterate all of the relations looking for incomplete relations
  const RelationMap& relations = map->getRelations();
  for (const auto& relation : relations)
  {
    RelationPtr r = relation.second;
    const std::vector<RelationData::Entry>& members = r->getMembers();
    bool request_relation = false;
    for (const auto& member : members)
    {
      ElementId eid = member.getElementId();
      //  Add the member to the list of missing elements
      if (!map->containsElement(eid))
      {
        request_relation = true;
        break;
      }
    }
    if (request_relation)
      missingElements.emplace(r->getElementId());
  }
  //  Don't read anything if there aren't any missing elements
  if (!missingElements.empty())
  {
    OsmApiReader reader;
    reader.open(_apiEndpoint);
    //  Setup the list of missing elements...
    reader.setMissingElements(missingElements);
    //  Read all of the missing elements into this map
    reader.read(map);
  }
}

void MissingElementRetrievalOp::setApiEndpointUrl(const QString& url)
{
  //  Ensure that the url is pointed at an OSM or Overpass API endpoint
  if (!url.endsWith(OsmApiEndpoints::OVERPASS_API_PATH) && !url.endsWith(OsmApiEndpoints::OSM_API_PATH_MAP))
  {
    LOG_ERROR("Unknown missing element retrieval URL: " << url);
    return;
  }
  _apiEndpoint = url;
}

}
