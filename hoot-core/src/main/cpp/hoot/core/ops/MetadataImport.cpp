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

#include "MetadataImport.h"

// Hoot
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/NodeMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/WayMap.h>
#include <hoot/core/util/Factory.h>

// geos
#include <geos/geom/Polygon.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MetadataImport)

void MetadataImport::_apply()
{
  LOG_INFO( "IMPORTING METADATA" );

  ElementConverter elementConverter(_pMap);

  QString indiKey = _datasetIndicator.first;
  QString indiVal = _datasetIndicator.second;

  if (indiKey.length() == 0) return;

  const WayMap allWays = _pMap->getWays();
  const NodeMap allNodes = _pMap->getNodes();
  const RelationMap allRels = _pMap->getRelations();

  // find all dataset ways
  QList<WayPtr> datasetWays;
  QList<shared_ptr<Polygon>> datasetPolys;

  for (WayMap::const_iterator it = allWays.begin(); it != allWays.end(); ++it)
  {
    const WayPtr pWay = it->second;
    const Tags& tags = pWay->getTags();

    if (tags.contains(indiKey) && tags[indiKey]==indiVal)
    {
      LOG_INFO( "Found dataset indicator in way " << pWay->getId());

      datasetWays.push_back(pWay);
      datasetPolys.push_back(elementConverter.convertToPolygon(pWay));
    }
  }

  // merge polygons with matching metadata tags
  QList<WayPtr> importWays;
  QList<shared_ptr<Geometry>> mergedImportPolys;

  for (int ds = 0; ds < datasetWays.length(); ds++)
  {
    bool matched = false;

    // check if current way matches any existing merged polys
    for (int im = 0; im < importWays.length(); im++)
    {
      if (areMetadataTagsEqual(datasetWays[ds], importWays[im]))
      {
        // merge polygon with existing polygon
        mergedImportPolys[im] =
            shared_ptr<Geometry>(mergedImportPolys[im]->Union(datasetPolys[ds].get()));
        matched = true;
        break;
      }
    }

    if (!matched)
    {
      // create new polygon entry
      importWays.push_back(datasetWays[ds]);
      mergedImportPolys.push_back(datasetPolys[ds]);
    }
  }

  // gather all potential target elements for metadata tags
  QList<ElementPtr> elementsToProcess;

  for (WayMap::const_iterator it = allWays.begin(); it != allWays.end(); ++it)
  {
    if (!datasetWays.contains(it->second) &&        // ignore the ways providing the dataset
        it->second->getTags().hasInformationTag())
    {
      elementsToProcess.push_back(it->second);
    }
  }

  for (NodeMap::const_iterator it = allNodes.begin(); it != allNodes.end(); ++it)
  {
    if (it->second->getTags().hasInformationTag())
      elementsToProcess.push_back(it->second);
  }

  for (RelationMap::const_iterator it = allRels.begin(); it != allRels.end(); ++it)
  {
    if (it->second->getTags().hasInformationTag())
      elementsToProcess.push_back(it->second);
  }

  // apply tags to all elements contained in the mergedImportPolys
  for (int im = 0; im < importWays.length(); im++)
  {

  }


}

bool MetadataImport::areMetadataTagsEqual(ElementPtr p1, ElementPtr p2)
{
  Tags t1 = p1->getTags();
  Tags t2 = p2->getTags();

  for (QString tag : _tags.keys())
  {
    bool p1hasTag = t1.contains(tag);
    bool p2hasTag = t2.contains(tag);

    if (p1hasTag && p2hasTag)
    {
      // fail with mismatching tag values
      if (t1[tag] != t2[tag]) return false;
    }
    else if (p1hasTag != p2hasTag)
    {
      // fail if one has the tag but not the other
      return false;
    }
  }

  return true;
}

void MetadataImport::applyToElement( ElementPtr )//pElement)
{

}

}
