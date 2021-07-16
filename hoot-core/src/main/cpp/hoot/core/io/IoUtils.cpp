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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "IoUtils.h"

// Hoot
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/WayCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/ImmediatelyConnectedOutOfBoundsWayTagger.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/io/ElementCriterionInputStream.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QFileInfo>
#include <QDirIterator>

// GEOS
#include <geos/geom/GeometryFactory.h>

namespace hoot
{

bool IoUtils::urlsAreBoundable(const QStringList& urls)
{
  for (int i = 0; i < urls.size(); i++)
  {
    const QString url = urls.at(i);
    std::shared_ptr<OsmMapReader> reader =
      OsmMapReaderFactory::createReader(url, true, Status::Invalid);

    std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(reader);
    if (!boundable)
    {
      return false;
    }
  }
  return true;
}

bool IoUtils::isSupportedOsmFormat(const QString& input)
{
  const QString inputLower = input.toLower();
  return inputLower.endsWith(".osm") || inputLower.endsWith(".osm.pbf") ||
         inputLower.startsWith(MetadataTags::HootApiDbScheme() + "://") ||
         inputLower.startsWith(MetadataTags::OsmApiDbScheme() + "://");
}

bool IoUtils::isSupportedOgrFormat(const QString& input, const bool allowDir)
{
  LOG_VART(input);
  LOG_VART(allowDir);

  const QString justPath = input.split(";")[0];

  if (!allowDir && QFileInfo(justPath).isDir())
  {
    return false;
  }

  // input is a dir; only accepting a dir as input if it contains a shape file or is a file geodb
  if (QFileInfo(justPath).isDir())
  {
    return
      justPath.toLower().endsWith(".gdb") ||
      FileUtils::dirContainsFileWithExtension(QFileInfo(input).dir(), "shp");
  }
  // single input
  else
  {
    // The only zip file format we support are ones containing OGR inputs.
    if (justPath.toLower().endsWith(".zip") ||
        // We only support this type of postgres URL for OGR inputs.
        justPath.toLower().startsWith("pg:") ||
        // Or, OGDI Vectors. Things like VPF (DNC, VMAP etc)
        justPath.toLower().startsWith("gltp:"))
    {
      return true;
    }
    LOG_VART(OgrUtilities::getInstance().getSupportedFormats(false));
    LOG_VART(QFileInfo(justPath).suffix());
    return
      OgrUtilities::getInstance().getSupportedFormats(false).contains(
        "." + QFileInfo(justPath).suffix());
  }
}

bool IoUtils::anyAreSupportedOgrFormats(const QStringList& inputs, const bool allowDir)
{
  if (inputs.empty())
    return false;

  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    if (isSupportedOgrFormat(input, allowDir))
      return true;
  }
  return false;
}

bool IoUtils::areSupportedOgrFormats(const QStringList& inputs, const bool allowDir)
{
  if (inputs.empty())
    return false;

  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    if (!isSupportedOgrFormat(input, allowDir))
      return false;
  }
  return true;
}

void IoUtils::ogrPathsAndLayersToPaths(QStringList& inputs)
{
  QStringList modifiedInputs;
  for (int i = 0; i < inputs.size(); i++)
  {
    QString input = inputs.at(i);
    ogrPathAndLayerToPath(input);
    modifiedInputs.append(input);
  }
  inputs = modifiedInputs;
}

void IoUtils::ogrPathAndLayerToPath(QString& input)
{
  input = input.split(";")[0];
}

void IoUtils::ogrPathAndLayerToLayer(QString& input)
{
  if (input.contains(";"))
  {
    input = input.split(";")[1];
  }
  else
  {
    input = "";
  }
}

bool IoUtils::isOgrPathAndLayer(const QString& input)
{
  return input.split(";").size() == 2;
}

QStringList IoUtils::getSupportedInputsRecursively(
  const QStringList& topLevelPaths, const QStringList& nameFilters)
{
  QStringList validInputs;
  for (int i = 0; i < topLevelPaths.size(); i++)
  {
    // If its a file and not a dir, go ahead and add it if its supported.
    const QString path = topLevelPaths.at(i);
    if (!QFileInfo(path).isDir())
    {
      if ((nameFilters.isEmpty() || StringUtils::matchesWildcard(path, nameFilters)) &&
           isSupportedInputFormat(path))
      {
        validInputs.append(path);
      }
    }
    else
    {
      QDirIterator itr(path, nameFilters, QDir::NoFilter, QDirIterator::Subdirectories);
      while (itr.hasNext())
      {
        const QString input = itr.next();
        if (isSupportedInputFormat(input))
        {
          validInputs.append(input);
        }
      }
    }
  }
  // Sort for consistent results for callers.
  validInputs.sort();
  return validInputs;
}

bool IoUtils::isSupportedInputFormat(const QString& url)
{
  return !OsmMapReaderFactory::getReaderName(url).trimmed().isEmpty();
}

bool IoUtils::isSupportedOutputFormat(const QString& url)
{
  return !OsmMapWriterFactory::getWriterName(url).trimmed().isEmpty();
}

bool IoUtils::isStreamableIo(const QString& input, const QString& output)
{
  QString writerName = ConfigOptions().getMapFactoryWriter();
  if (writerName.trimmed().isEmpty())
  {
    writerName = OsmMapWriterFactory::getWriterName(output);
  }
  LOG_VARD(writerName);
  LOG_VARD(isStreamableInput(input));
  LOG_VARD(isStreamableOutput(output));
  LOG_VARD(ConfigUtils::boundsOptionEnabled());
  LOG_VARD(ConfigOptions().getWriterXmlSortById());

  return
    isStreamableInput(input) && isStreamableOutput(output) &&
    // The XML writer can't keep sorted output when streaming, so require an additional config
    // option be specified in order to stream when writing that format
    (writerName != OsmXmlWriter::className() ||
     (writerName == OsmXmlWriter::className() && !ConfigOptions().getWriterXmlSortById())) &&
    // No readers when using the bounds option are able to do streaming I/O at this point.
    !ConfigUtils::boundsOptionEnabled();
}

bool IoUtils::areStreamableIo(const QStringList& inputs, const QString& output)
{
  for (int i = 0; i < inputs.size(); i++)
  {
    if (!isStreamableIo(inputs.at(i), output))
    {
      LOG_INFO(
        "Unable to stream I/O due to input: ..." << FileUtils::toLogFormat(inputs.at(i), 25) <<
        " and/or output: ..." << FileUtils::toLogFormat(output, 25) <<
        ". Loading entire map...");
      return false;
    }
  }
  return true;
}

bool IoUtils::areStreamableInputs(const QStringList& inputs, const bool logUnstreamable)
{
  for (int i = 0; i < inputs.size(); i++)
  {
    if (!isStreamableInput(inputs.at(i)))
    {
      if (logUnstreamable)
      {
        LOG_INFO(
          "Unable to stream inputs due to input: " << inputs.at(i).right(25) <<
          ". Loading entire map into memory...");
      }
      return false;
    }
  }
  return true;
}

bool IoUtils::isStreamableInput(const QString& url)
{
  bool result = false;
  std::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::createReader(url, true, Status::Unknown1);
  std::shared_ptr<ElementInputStream> eis =
    std::dynamic_pointer_cast<ElementInputStream>(reader);
  if (eis)
  {
    result = true;
  }
  return result;
}

bool IoUtils::isStreamableOutput(const QString& url)
{
  bool result = false;
  std::shared_ptr<OsmMapWriter> writer = OsmMapWriterFactory::createWriter(url);
  std::shared_ptr<ElementOutputStream> streamWriter =
    std::dynamic_pointer_cast<ElementOutputStream>(writer);
  if (streamWriter)
  {
    result = true;
  }
  return result;
}

bool IoUtils::areValidStreamingOps(const QStringList& ops)
{
  LOG_VARD(ops);
  // add visitor/criterion operations if any of the convert ops are visitors.
  foreach (QString opName, ops)
  {
    if (!opName.trimmed().isEmpty())
    {
      const QString unstreamableMsg =
        "Unable to stream I/O due to op: " + opName + ". Loading entire map...";

      if (Factory::getInstance().hasBase<ElementCriterion>(opName))
      {
        ElementCriterionPtr criterion =
          Factory::getInstance().constructObject<ElementCriterion>(opName);
        // when streaming we can't provide a reliable OsmMap.
        if (dynamic_cast<OsmMapConsumer*>(criterion.get()) != nullptr)
        {
          LOG_INFO(unstreamableMsg);
          return false;
        }
      }
      else if (Factory::getInstance().hasBase<ElementVisitor>(opName))
      {
        ElementVisitorPtr vis =
          Factory::getInstance().constructObject<ElementVisitor>(opName);
        // when streaming we can't provide a reliable OsmMap.
        if (dynamic_cast<OsmMapConsumer*>(vis.get()) != nullptr)
        {
          LOG_INFO(unstreamableMsg);
          return false;
        }
      }
      else if (Factory::getInstance().hasBase<ConstElementVisitor>(opName))
      {
        ConstElementVisitorPtr vis =
          Factory::getInstance().constructObject<ConstElementVisitor>(opName);
        // when streaming we can't provide a reliable OsmMap.
        if (dynamic_cast<OsmMapConsumer*>(vis.get()) != nullptr)
        {
          LOG_INFO(unstreamableMsg);
          return false;
        }
      }
      // OsmMapOperation isn't streamable.
      else
      {
        LOG_INFO(unstreamableMsg);
        return false;
      }
    }
  }

  return true;
}

ElementInputStreamPtr IoUtils::getFilteredInputStream(
  ElementInputStreamPtr streamToFilter, const QStringList& ops)
{
  if (ops.empty())
  {
    return streamToFilter;
  }

  ConfigUtils::checkForDuplicateElementCorrectionMismatch(ops);

  foreach (QString opName, ops)
  {
    LOG_VARD(opName);
    if (!opName.trimmed().isEmpty())
    {
      // Can this be cleaned up?

      if (Factory::getInstance().hasBase<ElementCriterion>(opName))
      {
        LOG_INFO("Initializing operation: " << opName << "...");
        ElementCriterionPtr criterion =
          Factory::getInstance().constructObject<ElementCriterion>(opName);

        std::shared_ptr<Configurable> critConfig;
        if (criterion.get())
        {
          critConfig = std::dynamic_pointer_cast<Configurable>(criterion);
        }
        LOG_VART(critConfig.get());
        if (critConfig.get())
        {
          critConfig->setConfiguration(conf());
        }

        streamToFilter = std::make_shared<ElementCriterionInputStream>(streamToFilter, criterion);
      }
      else if (Factory::getInstance().hasBase<ElementVisitor>(opName))
      {
        LOG_INFO("Initializing operation: " << opName << "...");
        ElementVisitorPtr visitor = Factory::getInstance().constructObject<ElementVisitor>(opName);

        std::shared_ptr<Configurable> visConfig;
        if (visitor.get())
        {
          visConfig = std::dynamic_pointer_cast<Configurable>(visitor);
        }
        LOG_VART(visConfig.get());
        if (visConfig.get())
        {
          visConfig->setConfiguration(conf());
        }

        streamToFilter = std::make_shared<ElementVisitorInputStream>(streamToFilter, visitor);
      }
      else
      {
        throw HootException(
          "An unsupported operation was passed to a streaming conversion: " + opName);
      }
    }
  }

  return streamToFilter;
}

void IoUtils::loadMap(
  const OsmMapPtr& map, const QString& path, bool useFileId, Status defaultStatus,
  const QString& translationScript, const int ogrFeatureLimit, const QString& jobSource,
  const int numTasks)
{
  const QStringList pathLayer = path.split(";");
  const QString justPath = pathLayer[0];
  LOG_VART(path);
  LOG_VART(pathLayer);
  LOG_VART(justPath);
  // We need to perform custom read logic for OGR inputs due to the fact they may have multiple
  // layers per input file or multiple files per directory.
  if (isSupportedOgrFormat(justPath, true))
  {
    OgrReader reader;
    reader.setConfiguration(conf());
    reader.setDefaultStatus(defaultStatus);
    if (ogrFeatureLimit > 0)
    {
      reader.setLimit(ogrFeatureLimit);
    }
    reader.setSchemaTranslationScript(translationScript);
    // This reader closes itself.
    reader.read(justPath, pathLayer.size() > 1 ? pathLayer[1] : "", map, jobSource, numTasks);
  }
  else
  {
    // This handles all non-OGR format reading.
    OsmMapReaderFactory::read(map, path, useFileId, defaultStatus);
  }
}

void IoUtils::loadMaps(
  const OsmMapPtr& map, const QStringList& paths, bool useFileId, Status defaultStatus)
{
  // TODO: it would be nice to allow this to take in Progress for updating
  for (int i = 0; i < paths.size(); i++)
  {
    loadMap(map, paths.at(i), useFileId, defaultStatus);
  }
}

void IoUtils::saveMap(const OsmMapPtr& map, const QString& path)
{
  // We could pass progress in here to get more granular write status feedback. Otherwise, this is
  // merely a wrapper.
  OsmMapWriterFactory::write(map, path);
}

void IoUtils::cropToBounds(
  OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepConnectedOobWays)
{
  cropToBounds(map, GeometryUtils::envelopeToPolygon(bounds), keepConnectedOobWays);
}

void IoUtils::cropToBounds(
  OsmMapPtr& map, const std::shared_ptr<geos::geom::Geometry>& bounds,
  const bool keepConnectedOobWays)
{
  LOG_INFO(
    "Applying bounds filtering to input data: ..." <<
    QString::fromStdString(bounds->toString()).right(50) << "...");
  LOG_VARD(keepConnectedOobWays);
  LOG_VARD(StringUtils::formatLargeNumber(map->getElementCount()));

  MapCropper cropper;
  cropper.setBounds(bounds);
  cropper.setKeepEntireFeaturesCrossingBounds(
    ConfigOptions().getBoundsKeepEntireFeaturesCrossingBounds());
  const bool strictBoundsHandling =
    ConfigOptions().getBoundsKeepOnlyFeaturesInsideBounds();
  cropper.setKeepOnlyFeaturesInsideBounds(strictBoundsHandling);
  cropper.setRemoveMissingElements(ConfigOptions().getBoundsRemoveMissingElements());

  // If we want to keep ways that are outside of the crop bounds but connected to a way that's
  // inside the bounds, we need to tag them before cropping and then tell the cropper to leave
  // them alone.
  ElementCriterionPtr inclusionCrit;
  if (keepConnectedOobWays)
  {
    // The two cropper config opts above will always be opposite of one another, so we'll just
    // determine bounds strictness off of one of them.
    ImmediatelyConnectedOutOfBoundsWayTagger tagger(strictBoundsHandling);
    tagger.setBounds(bounds);
    LOG_INFO(tagger.getInitStatusMessage());
    tagger.apply(map);
    LOG_DEBUG(tagger.getCompletedStatusMessage());
    inclusionCrit =
      std::make_shared<ChainCriterion>(
        std::make_shared<WayCriterion>(),
        std::make_shared<TagKeyCriterion>(MetadataTags::HootConnectedWayOutsideBounds()));
  }
  cropper.setInclusionCriterion(inclusionCrit);

  LOG_VARD(StringUtils::formatLargeNumber(map->getElementCount()));
  LOG_STATUS(cropper.getInitStatusMessage());
  cropper.apply(map);
  LOG_DEBUG(cropper.getCompletedStatusMessage());
  LOG_VARD(StringUtils::formatLargeNumber(map->getElementCount()));

  OsmMapWriterFactory::writeDebugMap(map, className(), "cropped-to-bounds");
}

std::shared_ptr<ElementVisitorInputStream> IoUtils::getVisitorInputStream(
  const QString& input, const QString& visitorClassName, const bool useDataSourceIds)
{
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->open(input);
  reader->initializePartial();

  return
    std::make_shared<ElementVisitorInputStream>(
      std::dynamic_pointer_cast<ElementInputStream>(reader),
      ElementVisitorPtr(Factory::getInstance().constructObject<ElementVisitor>(visitorClassName)));
}

bool IoUtils::isUrl(const QString& str)
{
  // this works in the hoot world
  return str.contains("://");
}

void IoUtils::writeOutputDir(const QString& dirName)
{
  QFileInfo outputInfo(dirName);
  LOG_VART(outputInfo.dir().absolutePath());
  const bool outputDirSuccess = FileUtils::makeDir(outputInfo.dir().absolutePath());
  LOG_VART(outputDirSuccess);
  if (!outputDirSuccess)
  {
    throw IllegalArgumentException("Unable to create output path for: " + dirName);
  }
}

QString IoUtils::getOutputUrlFromInput(
  const QString& inputUrl, const QString& appendText, const QString& outputFormat)
{
  if (DbUtils::isOsmApiDbUrl(inputUrl) || inputUrl.startsWith("http://"))
  {
    throw IllegalArgumentException(
      QString("--separate-output cannot be used with OSM API database (osmapidb://) or OSM API ") +
      QString("web (http://) inputs."));
  }
  // This check prevents the overwriting of the input URL.
  if (appendText.isEmpty() && outputFormat.isEmpty())
  {
    throw IllegalArgumentException(
      QString("Either text to append or an output format must be specified when generating an ") +
      QString("output URL from an input URL."));
  }

  LOG_VART(inputUrl);
  LOG_VART(outputFormat);

  // Get the base name of the input and the extension if its a file format or one of our custom
  // dir formats that requires an extension at the end of the dir name (gdb, shp, etc.).
  QString existingBaseName;
  QString existingExtension;
  if (DbUtils::isHootApiDbUrl(inputUrl))
  {
    existingBaseName = HootApiDb::getTableName(inputUrl);
  }
  else
  {
    QFileInfo urlInfo(inputUrl);
    LOG_VART(urlInfo.isFile());
    LOG_VART(urlInfo.isDir());
    if (urlInfo.isFile())
    {
      existingBaseName = urlInfo.baseName();
      existingExtension = urlInfo.completeSuffix();
    }
    else if (urlInfo.isDir())
    {
      QDir dir(inputUrl);
      existingBaseName = dir.dirName();
      // This is a little kludgy, and we may be able to make it more extensible going forward. We
      // want to get the extension from our custom dir formats as well in order to be able to write
      // the output URL correctly.
      if (existingBaseName.endsWith(ShapefileWriter().supportedFormats()))
      {
        existingExtension = ShapefileWriter().supportedFormats().replace(".", "");
        existingBaseName = existingBaseName.replace(ShapefileWriter().supportedFormats(), "");
      }
      else if (existingBaseName.endsWith("gdb"))
      {
        existingExtension = "gdb";
        existingBaseName = existingBaseName.replace(".gdb", "");
      }
    }
  }
  LOG_VART(existingBaseName);
  LOG_VART(existingExtension);

  if (!existingExtension.isEmpty() && !outputFormat.isEmpty() && existingExtension == outputFormat)
  {
    throw IllegalArgumentException(
      "Generated output URL for format: " + outputFormat + " will overwrite input URL: " + inputUrl);
  }

  // Create the output url to be the input url with the custom text appended to it. This prevents
  // us from overwriting any output.
  QString str = inputUrl;
  str = str.replace(existingBaseName, existingBaseName + appendText);
  // If a format was passed in, then the output format is potentially different than the input
  // format. If the input URL was a file or dir format with an extension, we need to swap that
  // extension out for the output format.
  if (!outputFormat.isEmpty() && !DbUtils::isHootApiDbUrl(outputFormat) &&
      !existingExtension.isEmpty())
  {
    str = str.replace(existingExtension, outputFormat);
  }
  return str;
}

}
