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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMapConsumer.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/visitors/ProjectToGeographicVisitor.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class ConvertCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ConvertCmd"; }

  ConvertCmd() : _logWarnCount(0) {}

  virtual QString getName() const { return "convert"; }

  virtual QString getDescription() const
  { return "Converts map data from one input format to another"; }

  virtual int runSimple(QStringList args)
  {
    //2 base args + 3 optional args each with a -- identifier in front (x2)
    if (args.size() != 2 && args.size() != 4 && args.size() != 6 && args.size() != 8)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two to five parameters.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();

    const QString input = args[0];
    const QString output = args[1];

    QString translation;
    if (args.size() > 2 && args.contains("--trans"))
    {
      translation = args.at(args.indexOf("--trans") + 1);
      if (translation.trimmed().isEmpty())
      {
        throw HootException("Invalid translation specified.");
      }
      //TODO: Would a translation ever apply to shape file output?
      if (!IoUtils::isSupportedOgrFormat(input) && !IoUtils::isSupportedOgrFormat(output))
      {
        throw HootException("A translation can only be specified when converting to/from OGR formats.");
      }
    }

    QStringList cols;
    if (args.size() > 2 && args.contains("--cols"))
    {
      cols = args.at(args.indexOf("--cols") + 1).split(",", QString::SkipEmptyParts);
      //TODO: We may be able to relax the restriction here of requiring the input be an OSM
      //format, but since cols were originally only used with osm2shp, let's keep it here for now.
      if (!cols.isEmpty() && !output.toLower().endsWith(".shp") &&
          !IoUtils::isSupportedOsmFormat(input))
      {
        throw HootException(
          "Columns may only be specified when converting from an OSM format to the shape file format.");
      }
    }

    int featureReadLimit = 0;
    if (args.size() > 2 && args.contains("--limit"))
    {
      bool ok;
      featureReadLimit = args.at(args.indexOf("--limit") + 1).toInt(&ok);
      if (!ok)
      {
        throw HootException("Invalid input specified for limit: " +
                            args.at(args.indexOf("--limit") + 1));
      }
      //TODO: Can this eventually be removed?
      if (!IoUtils::isSupportedOgrFormat(input))
      {
        throw HootException("Limit may only be specified when converting OGR inputs.");
      }
    }

    LOG_INFO("Converting " << input.right(100) << " to " << output.right(100) << "...");

    //TODO: try to simplify this logic down to as few if/elses as possible

    //osm2shp
    if (output.toLower().endsWith(".shp") && IoUtils::isSupportedOsmFormat(input) &&
        !cols.isEmpty())
    {
      LOG_DEBUG("osm2shp");

      OsmMapPtr map(new OsmMap());
      loadMap(map, input, true);

      ShapefileWriter writer;
      writer.setColumns(cols);
      writer.write(map, output);
    }
    //osm2ogr
    else if (IoUtils::isSupportedOsmFormat(input) && IoUtils::isSupportedOgrFormat(output) &&
             !translation.isEmpty())
    {
      LOG_DEBUG("osm2ogr");

      boost::shared_ptr<OgrWriter> writer(new OgrWriter());
      if (ConfigOptions().getElementCacheSizeNode() > 0 &&
          ConfigOptions().getElementCacheSizeWay() > 0 &&
          ConfigOptions().getElementCacheSizeRelation() > 0)
      {
        writer->setCacheCapacity(
          ConfigOptions().getElementCacheSizeNode(), ConfigOptions().getElementCacheSizeWay(),
          ConfigOptions().getElementCacheSizeRelation());
      }
      writer->setScriptPath(translation);
      writer->open(output);

      OsmMapReaderFactory readerFactory = OsmMapReaderFactory::getInstance();
      if (readerFactory.hasElementInputStream(input) &&
          //TODO: deal with these ops
          ConfigOptions().getOsm2ogrOps().size() == 0 &&
          //none of the convert bounding box supports are able to do streaming I/O at this point
          !ConfigUtils::boundsOptionEnabled())
      {
        boost::shared_ptr<OsmMapReader> reader =
          OsmMapReaderFactory::getInstance().createReader(input);
        reader->open(input);
        boost::shared_ptr<ElementInputStream> streamReader =
          boost::dynamic_pointer_cast<ElementInputStream>(reader);
        boost::shared_ptr<ElementOutputStream> streamWriter =
          boost::dynamic_pointer_cast<ElementOutputStream>(writer);

        boost::shared_ptr<OGRSpatialReference> projection = streamReader->getProjection();
        ProjectToGeographicVisitor visitor;
        bool notGeographic = !projection->IsGeographic();

        if (notGeographic)
          visitor.initialize(projection);

        while (streamReader->hasMoreElements())
        {
          ElementPtr element = streamReader->readNextElement();
          if (notGeographic)
            visitor.visit(element);
          streamWriter->writeElement(element);
        }
      }
      else
      {
        OsmMapPtr map(new OsmMap());
        loadMap(map, input, true);

        //TODO: deal with these ops
        NamedOp(ConfigOptions().getOsm2ogrOps()).apply(map);
        MapProjector::projectToWgs84(map);
        writer->write(map);
      }
    }
    //ogr2osm
    else if (IoUtils::isSupportedOgrFormat(input) && IoUtils::isSupportedOsmFormat(output) &&
             !translation.isEmpty())
    {
      LOG_DEBUG("ogr2osm");

      OsmMapPtr map(new OsmMap());
      OgrReader reader;
      if (featureReadLimit > 0)
      {
        reader.setLimit(featureReadLimit);
      }
      reader.setTranslationFile(translation);

      ConfigOptions configOptions;

      //TODO: move to top
      Progress progress(getName());
      progress.setReportType(configOptions.getProgressReportingFormat());
      progress.setState("Running");

      LOG_INFO("Reading layers...");
      QStringList inputs = input.split(" ");
      for (int i = 0; i < inputs.size(); i++)
      {
        QString input = inputs[i];
        LOG_VART(input);

        if (input == "")
        {
          LOG_WARN("Got an empty layer, skipping.");
          continue;
        }

        QStringList layers;
        if (input.contains(";"))
        {
          QStringList list = input.split(";");
          input = list.at(0);
          layers.append(list.at(1));
        }
        else
        {
          layers = reader.getFilteredLayerNames(input);
          layers.sort();
        }

        if (layers.size() == 0)
        {
          if (_logWarnCount < ConfigOptions().getLogWarnMessageLimit())
          {
            LOG_WARN("Could not find any valid layers to read from in " + input + ".");
          }
          else if (_logWarnCount == ConfigOptions().getLogWarnMessageLimit())
          {
            LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          _logWarnCount++;
        }

        // process the completion status report information first
        long featureCountTotal = 0;
        int undefinedCounts = 0;
        vector<float> progressWeights;
        for (int i = 0; i < layers.size(); i++)
        {
          LOG_VART(layers[i]);
          // simply open the file, get the meta feature count value, and close
          int featuresPerLayer = reader.getFeatureCount(input, layers[i]);
          progressWeights.push_back((float)featuresPerLayer);
          // cover the case where no feature count available efficiently
          if(featuresPerLayer == -1) undefinedCounts++;
          else featureCountTotal += featuresPerLayer;
        }

        int definedCounts = layers.size() - undefinedCounts;

        // determine weights for 3 possible cases
        if (undefinedCounts == layers.size())
        {
          for (int i=0;i<layers.size();i++) progressWeights[i]=1./(float)layers.size();
        }
        else if(definedCounts == layers.size())
        {
          for (int i=0;i<layers.size();i++) progressWeights[i] /= (float)featureCountTotal;
        }
        else
        {
          for (int i=0;i<layers.size();i++)
            if(progressWeights[i] == -1)
            {
              progressWeights[i] = (1./(float)definedCounts) * featureCountTotal;
            }
          // reset featurecount total and recalculate
          featureCountTotal = 0;
          for (int i=0;i<layers.size();i++) featureCountTotal += progressWeights[i];
          for (int i=0;i<layers.size();i++) progressWeights[i] /= (float)featureCountTotal;
        }

        // read each layer's data
        for (int i = 0; i < layers.size(); i++)
        {
          if (Log::getInstance().getLevel() == Log::Info)
          {
            std::cout << ".";
            std::cout.flush();
          }
          LOG_VART(input);
          LOG_VART(layers[i]);
          progress.setTaskWeight(progressWeights[i]);
          reader.read(input, layers[i], map, progress);
        }
      }

      if (map->getNodes().size() == 0)
      {
        progress.set(1.0, "Failed", true, "After translation the map is empty.  Aborting.");
        throw HootException("After translation the map is empty. Aborting.");
      }

      MapProjector::projectToPlanar(map);
      NamedOp(ConfigOptions().getOgr2osmOps()).apply(map);
      MapProjector::projectToWgs84(map);
      saveMap(map, output);

      progress.set(1.0, "Successful", true, "Finished successfully.");
    }
    else
    {
      LOG_DEBUG("convert");

      // This keeps the status and the tags.
      conf().set(ConfigOptions().getReaderUseFileStatusKey(), true);
      conf().set(ConfigOptions().getReaderKeepStatusTagKey(), true);

//    QString readerName = ConfigOptions().getOsmMapReaderFactoryReader();
//    if (readerName.trimmed().isEmpty())
//    {
//      readerName = OsmMapReaderFactory::getReaderName(input);
//    }
//    LOG_VARD(readerName);
      LOG_VARD(OsmMapReaderFactory::getInstance().hasElementInputStream(input));

      QString writerName = ConfigOptions().getOsmMapWriterFactoryWriter();
      if (writerName.trimmed().isEmpty())
      {
        writerName = OsmMapWriterFactory::getWriterName(args[1]);
      }
      LOG_VARD(writerName);
      LOG_VARD(OsmMapWriterFactory::getInstance().hasElementOutputStream(output));
      LOG_VARD(ConfigUtils::boundsOptionEnabled());

      if (OsmMapReaderFactory::getInstance().hasElementInputStream(input) &&
          OsmMapWriterFactory::getInstance().hasElementOutputStream(output) &&
          _areValidStreamingOps(ConfigOptions().getConvertOps()) &&
          //the XML writer can't keep sorted output when streaming, so require an additional config
          //option be specified in order to stream when writing that format
          (writerName != "hoot::OsmXmlWriter" ||
           (writerName == "hoot::OsmXmlWriter" && !ConfigOptions().getWriterXmlSortById())) &&
          //none of the convert bounding box supports are able to do streaming I/O at this point
          !ConfigUtils::boundsOptionEnabled())
      {
        ElementStreamer::stream(input, output);
      }
      else
      {
        OsmMapPtr map(new OsmMap());
        loadMap(
          map, input, ConfigOptions().getReaderUseDataSourceIds(),
          Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
        NamedOp(ConfigOptions().getConvertOps()).apply(map);
        MapProjector::projectToWgs84(map);
        saveMap(map, output);
      }
    }

    LOG_DEBUG("Convert operation complete.");
    QString msg = "Convert operation took ";
    const qint64 timeElapsed = timer.elapsed();
    if (timeElapsed > 60000)
    {
      msg += QString::number(timeElapsed / 1000 / 60) + " minutes";
    }
    else
    {
      msg += QString::number(timeElapsed / 1000) + " seconds";
    }
    LOG_DEBUG(msg);

    return 0;
  }

private:

  //this should be static, but there's no header file
  unsigned int _logWarnCount;

  /*
   * Return true if all the specified operations are valid streaming operations.
   *
   * There are some ops that require the whole map be available in RAM (e.g. remove duplicate
   * nodes). These operations are not applicable for streaming.
   */
  bool _areValidStreamingOps(QStringList ops)
  {
    // add visitor/criterion operations if any of the convert ops are visitors.
    foreach (QString opName, ops)
    {
      if (!opName.trimmed().isEmpty())
      {
        if (Factory::getInstance().hasBase<ElementCriterion>(opName.toStdString()))
        {
          ElementCriterionPtr criterion(
            Factory::getInstance().constructObject<ElementCriterion>(opName));
          // when streaming we can't provide a reliable OsmMap.
          if (dynamic_cast<OsmMapConsumer*>(criterion.get()) != 0)
          {
            return false;
          }
        }
        else if (Factory::getInstance().hasBase<ElementVisitor>(opName.toStdString()))
        {
          // good, pass
        }
        else
        {
          return false;
        }
      }
    }

    return true;
  }
};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

