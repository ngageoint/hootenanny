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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/PartialOsmMapWriter.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

/**
 * @ingroup cmd
 * @page ConvertCmd Convert Command
 * @code
--convert (input) (output)
 * @endcode
 *
 * The convert command converts from one format to another.
 *
 * @param input The input file to be cleaned. Supports common geospatial file types.
 * @param output The output file. Supports common geospatial file types.
 *
 * @sa @ref convert.ops
 */
class ConvertCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ConvertCmd"; }

  ConvertCmd() { }

  virtual QString getName() const { return "convert"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();
    LOG_INFO("Converting " << args[0] << " to " << args[1] << "...");

    // This keeps the status and the tags.
    conf().set(ConfigOptions().getReaderUseFileStatusKey(), true);
    conf().set(ConfigOptions().getReaderKeepFileStatusKey(), true);

    if (OsmMapReaderFactory::getInstance().hasElementInputStream(args[0]) &&
        OsmMapWriterFactory::getInstance().hasElementOutputStream(args[1]) &&
        //TODO: Why can't we use convert ops with streaming?
        ConfigOptions().getConvertOps().size() == 0)
    {
      streamElements(args[0], args[1]);
    }
    else
    {
      OsmMapPtr map(new OsmMap());

      loadMap(map, args[0], true, Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));

      // Apply any user specified operations.
      NamedOp(ConfigOptions().getConvertOps()).apply(map);

      MapProjector::projectToWgs84(map);

      saveMap(map, args[1]);
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

  void streamElements(QString in, QString out)
  {
    LOG_INFO("Streaming data conversion from " << in << " to " << out << "...");

    boost::shared_ptr<OsmMapReader> reader = OsmMapReaderFactory::getInstance().createReader(in);
    reader->open(in);
    boost::shared_ptr<ElementInputStream> streamReader =
      boost::dynamic_pointer_cast<ElementInputStream>(reader);
    boost::shared_ptr<OsmMapWriter> writer = OsmMapWriterFactory::getInstance().createWriter(out);
    writer->open(out);
    boost::shared_ptr<ElementOutputStream> streamWriter =
      boost::dynamic_pointer_cast<ElementOutputStream>(writer);

    ElementOutputStream::writeAllElements(*streamReader, *streamWriter);

    boost::shared_ptr<PartialOsmMapReader> partialReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(reader);
    if (partialReader.get())
    {
      partialReader->finalizePartial();
    }
    boost::shared_ptr<PartialOsmMapWriter> partialWriter =
      boost::dynamic_pointer_cast<PartialOsmMapWriter>(writer);
    if (partialWriter.get())
    {
      partialWriter->finalizePartial();
    }
  }

};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

