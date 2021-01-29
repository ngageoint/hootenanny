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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementStreamer.h"

// Hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/io/ElementCriterionInputStream.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

bool ElementStreamer::isStreamableIo(const QString& input, const QString& output)
{
  QString writerName = ConfigOptions().getMapFactoryWriter();
  if (writerName.trimmed().isEmpty())
  {
    writerName = OsmMapWriterFactory::getWriterName(output);
  }
  LOG_VARD(writerName);
  LOG_VARD(OsmMapReaderFactory::hasElementInputStream(input));
  LOG_VARD(OsmMapWriterFactory::hasElementOutputStream(output));
  LOG_VARD(ConfigUtils::boundsOptionEnabled());
  LOG_VARD(ConfigOptions().getWriterXmlSortById());

  return
    OsmMapReaderFactory::hasElementInputStream(input) &&
    OsmMapWriterFactory::hasElementOutputStream(output) &&
    //the XML writer can't keep sorted output when streaming, so require an additional config
    //option be specified in order to stream when writing that format
    (writerName != "hoot::OsmXmlWriter" ||
    (writerName == "hoot::OsmXmlWriter" && !ConfigOptions().getWriterXmlSortById())) &&
    // No readers when using the bounds option are able to do streaming I/O at this point.
    !ConfigUtils::boundsOptionEnabled();
}

bool ElementStreamer::areStreamableIo(const QStringList& inputs, const QString& output)
{
  for (int i = 0; i < inputs.size(); i++)
  {
    if (!ElementStreamer::isStreamableIo(inputs.at(i), output))
    {
      LOG_INFO(
        "Unable to stream I/O due to input: " << inputs.at(i).right(25) << " and/or output: " <<
        output.right(25) << ". Loading entire map...");
      return false;
    }
  }
  return true;
}

bool ElementStreamer::areValidStreamingOps(const QStringList& ops)
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
        ElementCriterionPtr criterion(
          Factory::getInstance().constructObject<ElementCriterion>(opName));
        // when streaming we can't provide a reliable OsmMap.
        if (dynamic_cast<OsmMapConsumer*>(criterion.get()) != 0)
        {
          LOG_INFO(unstreamableMsg);
          return false;
        }
      }
      else if (Factory::getInstance().hasBase<ElementVisitor>(opName))
      {
        ElementVisitorPtr vis(
          Factory::getInstance().constructObject<ElementVisitor>(opName));
        // when streaming we can't provide a reliable OsmMap.
        if (dynamic_cast<OsmMapConsumer*>(vis.get()) != 0)
        {
          LOG_INFO(unstreamableMsg);
          return false;
        }
      }
      else if (Factory::getInstance().hasBase<ConstElementVisitor>(opName))
      {
        ConstElementVisitorPtr vis(
          Factory::getInstance().constructObject<ConstElementVisitor>(opName));
        // when streaming we can't provide a reliable OsmMap.
        if (dynamic_cast<OsmMapConsumer*>(vis.get()) != 0)
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

ElementInputStreamPtr ElementStreamer::getFilteredInputStream(
  ElementInputStreamPtr streamToFilter, const QStringList& ops)
{
  if (ops.size() == 0)
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
        ElementCriterionPtr criterion(
          Factory::getInstance().constructObject<ElementCriterion>(opName));

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

        streamToFilter.reset(new ElementCriterionInputStream(streamToFilter, criterion));
      }
      else if (Factory::getInstance().hasBase<ElementVisitor>(opName))
      {
        LOG_INFO("Initializing operation: " << opName << "...");
        ElementVisitorPtr visitor(Factory::getInstance().constructObject<ElementVisitor>(opName));

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

        streamToFilter.reset(new ElementVisitorInputStream(streamToFilter, visitor));
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

void ElementStreamer::stream(const QString& input, const QString& out, const QStringList& convertOps,
                             Progress progress)
{
  stream(QStringList(input), out, convertOps, progress);
}

void ElementStreamer::stream(const QStringList& inputs, const QString& out,
                             const QStringList& convertOps, Progress progress)
{
  QElapsedTimer timer;
  timer.start();

  std::shared_ptr<OsmMapWriter> writer = OsmMapWriterFactory::createWriter(out);
  writer->open(out);
  std::shared_ptr<ElementOutputStream> streamWriter =
    std::dynamic_pointer_cast<ElementOutputStream>(writer);
  std::shared_ptr<PartialOsmMapWriter> partialWriter =
    std::dynamic_pointer_cast<PartialOsmMapWriter>(writer);
  partialWriter->initializePartial();

  for (int i = 0; i < inputs.size(); i++)
  {
    const QString in = inputs.at(i);
    const QString message = "Streaming data conversion from ..." + in + " to ..." + out + "...";
    // Always check for a valid task weight and that the job was set to running. Otherwise, this is
    // just an empty progress object, and we shouldn't log progress.
    if (progress.getTaskWeight() != 0.0 && progress.getState() == Progress::JobState::Running)
    {
      progress.setFromRelative(
        (float)i / (float)inputs.size(), Progress::JobState::Running, message);
    }
    else
    {
      LOG_STATUS(message);
    }

    std::shared_ptr<OsmMapReader> reader =
      OsmMapReaderFactory::createReader(
        in, ConfigOptions().getReaderUseDataSourceIds(),
        Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
    reader->setConfiguration(conf());
    reader->open(in);

    // add visitor/criterion operations if any of the convert ops are visitors.
    LOG_VARD(convertOps);
    ElementInputStreamPtr streamReader =
      getFilteredInputStream(std::dynamic_pointer_cast<ElementInputStream>(reader), convertOps);

    ElementOutputStream::writeAllElements(*streamReader, *streamWriter);

    std::shared_ptr<PartialOsmMapReader> partialReader =
      std::dynamic_pointer_cast<PartialOsmMapReader>(reader);
    if (partialReader.get())
    {
      partialReader->finalizePartial();
    }
  }

  if (partialWriter.get())
  {
    partialWriter->finalizePartial();
  }

  LOG_INFO("Streaming element I/O took: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

}
