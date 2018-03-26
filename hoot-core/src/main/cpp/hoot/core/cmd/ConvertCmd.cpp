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

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class ConvertCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ConvertCmd"; }

  ConvertCmd() { }

  virtual QString getName() const { return "convert"; }

  virtual QString getDescription() const
  { return "Converts map data from one input format to another"; }

  /**
   * Return true if all the specified operations are valid streaming operations.
   *
   * There are some ops that require the whole map be available in RAM (e.g. remove duplicate
   * nodes). These operations are not applicable for streaming.
   */
  bool areValidStreamingOps(QStringList ops)
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

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();

    const QString input = args[0];
    const QString output = args[1];
    LOG_INFO("Converting " << input.right(100) << " to " << output.right(100) << "...");

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
        areValidStreamingOps(ConfigOptions().getConvertOps()) &&
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
      // Apply any user specified operations.
      NamedOp(ConfigOptions().getConvertOps()).apply(map);
      MapProjector::projectToWgs84(map);
      saveMap(map, output);
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
};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

