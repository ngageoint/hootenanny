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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "IoUtils.h"

// Hoot
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/ops/ImmediatelyConnectedOutOfBoundsWayTagger.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QFileInfo>

// GEOS
#include <geos/geom/GeometryFactory.h>

namespace hoot
{

bool IoUtils::isSupportedOsmFormat(const QString& input)
{
  const QString inputLower = input.toLower();
  return inputLower.endsWith(".osm") || inputLower.endsWith(".osm.pbf") ||
         inputLower.startsWith("hootapidb://") || inputLower.startsWith("osmapidb://");
}

bool IoUtils::isSupportedOgrFormat(const QString& input, const bool allowDir)
{
  LOG_VART(input);
  LOG_VART(allowDir);

  if (!allowDir && QFileInfo(input).isDir())
  {
    return false;
  }

  LOG_VART(QFileInfo(input).isDir());
  //input is a dir; only accepting a dir as input if it contains a shape file or is a file geodb
  if (QFileInfo(input).isDir())
  {
    return input.toLower().endsWith(".gdb") ||
           FileUtils::dirContainsFileWithExtension(QFileInfo(input).dir(), "shp");
  }
  //single input
  else
  {
    //The only zip file format we support are ones containing OGR inputs.
    if (input.toLower().endsWith(".zip") ||
        //We only support this type of postgres URL for OGR inputs.
        input.toLower().startsWith("pg:") ||
        // Or, OGDI Vectors. Things like VPF (DNC, VMAP etc)
        input.toLower().startsWith("gltp:"))
    {
      return true;
    }
    LOG_VART(OgrUtilities::getInstance().getSupportedFormats(false));
    LOG_VART(QFileInfo(input).suffix());
    return OgrUtilities::getInstance().getSupportedFormats(false)
             .contains("." + QFileInfo(input).suffix());
  }
}

bool IoUtils::anyAreSupportedOgrFormats(const QStringList& inputs, const bool allowDir)
{
  if (inputs.size() == 0)
  {
    return false;
  }

  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    LOG_VART(input);
    const QString file = input.split(";")[0];
    LOG_VART(file);
    if (isSupportedOgrFormat(file, allowDir))
    {
      return true;
    }
  }
  return false;
}

bool IoUtils::areSupportedOgrFormats(const QStringList& inputs, const bool allowDir)
{
  if (inputs.size() == 0)
  {
    return false;
  }

  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    LOG_VART(input);
    const QString file = input.split(";")[0];
    LOG_VART(file);
    if (!isSupportedOgrFormat(file, allowDir))
    {
      return false;
    }
  }
  return true;
}

// TODO: Roll this into OsmMapReaderFactory somehow?
void IoUtils::loadMap(const OsmMapPtr& map, const QString& path, bool useFileId,
                      Status defaultStatus)
{
  QStringList pathLayer = path.split(";");
  QString justPath = pathLayer[0];
  if (OgrReader::isReasonablePath(justPath))
  {
    OgrReader reader;
    reader.setDefaultStatus(defaultStatus);
    reader.read(justPath, pathLayer.size() > 1 ? pathLayer[1] : "", map);
  }
  else
  {
    OsmMapReaderFactory::read(map, path, useFileId, defaultStatus);
  }
}

// TODO: Get rid of this??
void IoUtils::saveMap(const OsmMapPtr& map, const QString& path)
{
  // We could pass a progress in here to get more granular write status feedback.
  OsmMapWriterFactory::write(map, path);
}

void IoUtils::cropToBounds(OsmMapPtr& map, const geos::geom::Envelope& bounds,
                           const bool keepConnectedOobWays)
{
  LOG_INFO("Applying bounds filtering to input data: " << bounds << "...");
  LOG_VARD(keepConnectedOobWays);
  LOG_VARD(StringUtils::formatLargeNumber(map->getElementCount()));

  // We can get more precise bounds intersection calcs for ways when passing in a geometry here
  // instead of an envelope.
//  std::shared_ptr<geos::geom::Geometry> boundsGeom(
//    geos::geom::GeometryFactory::getDefaultInstance()->toGeometry(&bounds));
//  MapCropper cropper(boundsGeom);
  MapCropper cropper(bounds);

  cropper.setKeepEntireFeaturesCrossingBounds(
    ConfigOptions().getConvertBoundingBoxKeepEntireFeaturesCrossingBounds());
  const bool strictBoundsHandling =
    ConfigOptions().getConvertBoundingBoxKeepOnlyFeaturesInsideBounds();
  cropper.setKeepOnlyFeaturesInsideBounds(strictBoundsHandling);

  // If we want to keep ways that are outside of the crop bounds but connected to a way that's
  // inside the bounds, we need to tag them before cropping and then tell the cropper to leave
  // them alone.
  ElementCriterionPtr inclusionCrit;
  if (keepConnectedOobWays)
  {
    // The two cropper config opts above will always be opposite of one another, so we'll just
    // determine bounds strictness off of one of them.
    ImmediatelyConnectedOutOfBoundsWayTagger tagger(bounds, strictBoundsHandling);
    LOG_INFO(tagger.getInitStatusMessage());
    tagger.apply(map);
    LOG_DEBUG(tagger.getCompletedStatusMessage());
    inclusionCrit.reset(
      new ChainCriterion(
        std::shared_ptr<WayCriterion>(new WayCriterion()),
        std::shared_ptr<TagKeyCriterion>(
          new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds()))));
  }
  cropper.setInclusionCriterion(inclusionCrit);

  LOG_VARD(StringUtils::formatLargeNumber(map->getElementCount()));
  LOG_INFO(cropper.getInitStatusMessage());
  cropper.apply(map);
  LOG_DEBUG(cropper.getCompletedStatusMessage());
  LOG_VARD(StringUtils::formatLargeNumber(map->getElementCount()));
}

std::shared_ptr<ElementVisitorInputStream> getVisitorInputStream(
  const QString& input, const QString& visitorClassName, const bool useDataSourceIds)
{
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->open(input);
  reader->initializePartial();

  return
    std::shared_ptr<ElementVisitorInputStream>(
      new ElementVisitorInputStream(
        std::dynamic_pointer_cast<ElementInputStream>(reader),
        ElementVisitorPtr(
          Factory::getInstance().constructObject<ElementVisitor>(visitorClassName.toStdString()))));
}

}
