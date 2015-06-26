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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CookieCutterOp.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapReprojector.h>
#include <hoot/core/conflate/AlphaShapeGenerator.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

// Qt
#include <QUuid>
#include <QFile>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CookieCutterOp)

CookieCutterOp::CookieCutterOp()
{
  setConfiguration(conf());
}

void CookieCutterOp::setConfiguration(const Settings& conf)
{
  ConfigOptions config(conf);
  setAlpha(config.getCookieCutterAlpha());
  setAlphaShapeBuffer(config.getCookieCutterAlphaShapeBuffer());
  setOutputBuffer(config.getCookieCutterOutputBuffer());
  setCrop(config.getCookieCutterOutputCrop());
}

void CookieCutterOp::apply(shared_ptr<OsmMap>& map)
{
  //filter unknown1 out of the input map into another map
  OsmMapPtr refMap(new OsmMap(map));
  RemoveElementsVisitor unknown1Visitor(ElementCriterionPtr(new StatusCriterion(Status::Unknown1)));
  unknown1Visitor.setRecursive(true);
  refMap->visitRw(unknown1Visitor);
  LOG_VARD(refMap->getNodeMap().size());
    
  //create an alpha shape based on the filtered unknown1 map
  OsmMapPtr cutShapeMap = AlphaShapeGenerator(_alpha, _alphaShapeBuffer).generate(refMap);
  LOG_VARD(cutShapeMap->getNodeMap().size());
    
  //filter unknown2 out of the input map into another map
  OsmMapPtr doughMap(new OsmMap(map));
  RemoveElementsVisitor unknown2Visitor(ElementCriterionPtr(new StatusCriterion(Status::Unknown2)));
  unknown2Visitor.setRecursive(true);
  doughMap->visitRw(unknown2Visitor);
  LOG_VARD(doughMap->getNodeMap().size());
  
  //cookie cut the alpha shape from the unknown2 map
  CookieCutter(_crop, _outputBuffer).cut(cutShapeMap, doughMap);
  OsmMapPtr cookieCutMap = doughMap;
  LOG_VARD(cookieCutMap->getNodeMap().size());
    
  //combine the unknown1 map with the cookie cut unknown2 map back into the input map
  //TODO: not confident yet if the append operation is wise or solid, so leaving the code in here
  //for now that has the extra round of I/O
  const bool combineInMemory = true;
  if (combineInMemory)
  {
    refMap->append(cookieCutMap);
    OsmMapPtr result = refMap;
    map.reset(new OsmMap(result));
  }
  else
  {
    //TODO: since there isn't any easy way to do this in memory yet, writing out to file and reading
    //back in, for now
    MapReprojector::reprojectToWgs84(refMap);
    const QString refMapPath = "tmp/ref-map-" + QUuid::createUuid().toString() + ".osm";
    OsmUtils::saveMap(refMap, refMapPath);
    MapReprojector::reprojectToWgs84(cookieCutMap);
    const QString cookieCutMapPath = "tmp/cookie-cut-map-" + QUuid::createUuid().toString() + ".osm";
    OsmUtils::saveMap(cookieCutMap, cookieCutMapPath);

    OsmMapPtr result(new OsmMap());
    OsmUtils::loadMap(result, refMapPath, true, Status::Unknown1);
    LOG_VARD(result->getNodeMap().size());
    QFile::remove(refMapPath);
    OsmUtils::loadMap(result, cookieCutMapPath, true, Status::Unknown2);
    LOG_VARD(result->getNodeMap().size());
    QFile::remove(cookieCutMapPath);
    map.reset(new OsmMap(result));
    LOG_VARD(map->getNodeMap().size());
  }
}

}
