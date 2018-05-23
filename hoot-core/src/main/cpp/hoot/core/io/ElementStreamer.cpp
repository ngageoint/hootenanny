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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementStreamer.h"

// Hoot
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/ElementCriterionInputStream.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

void ElementStreamer::stream(const QString in, const QString out, const QStringList convertOps)
{
  LOG_INFO("Streaming data conversion from " << in << " to " << out << "...");

  boost::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::getInstance().createReader(
      in, ConfigOptions().getReaderUseDataSourceIds(),
      Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
  reader->open(in);
  boost::shared_ptr<ElementInputStream> streamReader =
    boost::dynamic_pointer_cast<ElementInputStream>(reader);

  boost::shared_ptr<OsmMapWriter> writer = OsmMapWriterFactory::getInstance().createWriter(out);
  writer->open(out);
  boost::shared_ptr<ElementOutputStream> streamWriter =
    boost::dynamic_pointer_cast<ElementOutputStream>(writer);
  boost::shared_ptr<PartialOsmMapWriter> partialWriter =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(writer);

  // add visitor/criterion operations if any of the convert ops are visitors.
  //this check is a little out of place in this class but not hurting anything...may be able to
  //move it back to ConvertCmd somehow at some point
  QStringList convertOpsToUse;
  if (convertOps.isEmpty())
  {
    convertOpsToUse = ConfigOptions().getConvertOps();
  }
  else
  {
    convertOpsToUse = convertOps;
  }
  foreach (QString opName, convertOpsToUse)
  {
    if (!opName.trimmed().isEmpty())
    {
      if (Factory::getInstance().hasBase<ElementCriterion>(opName.toStdString()))
      {
        LOG_INFO("Filtering input with: " << opName);
        ElementCriterionPtr criterion(
          Factory::getInstance().constructObject<ElementCriterion>(opName));

        streamReader.reset(new ElementCriterionInputStream(streamReader, criterion));
      }
      else if (Factory::getInstance().hasBase<ElementVisitor>(opName.toStdString()))
      {
        LOG_INFO("Visiting input with: " << opName);
        ElementVisitorPtr visitor(Factory::getInstance().constructObject<ElementVisitor>(opName));

        streamReader.reset(new ElementVisitorInputStream(streamReader, visitor));
      }
      else
      {
        throw HootException("An unsupported operation was passed to a streaming conversion.");
      }
    }
  }

  ElementOutputStream::writeAllElements(*streamReader, *streamWriter);

  boost::shared_ptr<PartialOsmMapReader> partialReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(reader);
  if (partialReader.get())
  {
    partialReader->finalizePartial();
  }

  if (partialWriter.get())
  {
    partialWriter->finalizePartial();
  }
}

}
