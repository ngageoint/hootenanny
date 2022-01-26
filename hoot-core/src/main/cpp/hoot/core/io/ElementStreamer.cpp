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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ElementStreamer.h"

// Hoot
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

ElementStreamer::ElementStreamer(const QString& translationScript)
  : _translationScript(translationScript)
{
}

void ElementStreamer::stream(const QStringList& inputs, const QString& out, const QStringList& convertOps, Progress progress)
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

  std::shared_ptr<PartialOsmMapReader> partialReader;
  long numFeaturesWritten = 0;
  for (int i = 0; i < inputs.size(); i++)
  {
    QString input = inputs.at(i);
    const QString message = "Streaming data conversion from ..." + input + " to ..." + out + "...";
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
        input, ConfigOptions().getReaderUseDataSourceIds(),
        Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
    reader->setConfiguration(conf());
    partialReader = std::dynamic_pointer_cast<PartialOsmMapReader>(reader);
    partialReader->initializePartial();
    std::shared_ptr<OgrReader> ogrReader = std::dynamic_pointer_cast<OgrReader>(reader);
    std::shared_ptr<OgrWriter> ogrWriter = std::dynamic_pointer_cast<OgrWriter>(writer);

    // #4899 would relax this to only require a translation if only one or the other of the
    // input/output was OGR and not require one when both are.
    if ((ogrReader || ogrWriter) && _translationScript.isEmpty())
      throw IllegalArgumentException("No translation script specified.");

    // Need to run separate logic for streaming from an OGR source with layers in order to support
    // the layer syntax.
    if (ogrReader && input.contains(";"))
      numFeaturesWritten += _streamFromOgr(*ogrReader, input, *streamWriter);
    else
    {
      reader->open(input);
      // Add visitor/criterion operations if any of the convert ops are visitors.
      LOG_VARD(convertOps);
      ElementInputStreamPtr streamReader =
        IoUtils::getFilteredInputStream(
          std::dynamic_pointer_cast<ElementInputStream>(reader), convertOps);
      numFeaturesWritten += ElementOutputStream::writeAllElements(*streamReader, *streamWriter);
    }
  }

  if (partialWriter.get())
    partialWriter->finalizePartial();
  if (partialReader.get())
    partialReader->finalizePartial();

  LOG_INFO(
    "Streamed " << StringUtils::formatLargeNumber(numFeaturesWritten) << " elements in: " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

long ElementStreamer::_streamFromOgr(const OgrReader& reader, QString& input, ElementOutputStream& writer,
                                     const QStringList& convertOps, Progress /*progress*/) const
{
  // TODO: run a separate progress for _streamFromOgr as part of #4856?

  reader.setSchemaTranslationScript(_translationScript);

  QStringList layers;
  if (input.contains(";"))
  {
    QStringList list = input.split(";");
    input = list.at(0);
    layers.append(list.at(1));
  }
  else
    layers = reader.getFilteredLayerNames(input);

  if (layers.empty())
  {
    LOG_WARN("Could not find any valid layers to read from in " + input + ".");
  }

  const QList<ElementVisitorPtr> ops = IoUtils::toStreamingOps(convertOps);

  long numFeaturesWritten = 0;
  for (const auto& layer : qAsConst(layers))
  {
    LOG_TRACE("Reading: " << input + " " << layer << "...");

    std::shared_ptr<ElementIterator> iterator(reader.createIterator(input, layer));
    while (iterator->hasNext())
    {
      std::shared_ptr<Element> e = iterator->next();

      // Run any convert ops present.
      for (const auto& op : ops)
        op->visit(e);

      if (e)
      {
        writer.writeElement(e);
        numFeaturesWritten++;
      }
    }
  }
  return numFeaturesWritten;
}

}
