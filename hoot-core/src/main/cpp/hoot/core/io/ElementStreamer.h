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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTSTREAMER_H
#define ELEMENTSTREAMER_H

// Hoot
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/util/Progress.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Streams elements to output
 */
class ElementStreamer
{

public:

  /**
   * Streams data sources from input to output.
   *
   * Associated readers/writers must implemented the partial map interfaces
   *
   * @param inputs data sources
   * @param out data destination
   * @param convertOps a list of map ops/visitors to perform against the data during conversion
   * @param progress optional for tracking I/O job progress
   */
  static void stream(const QStringList& inputs, const QString& out,
                     const QStringList& convertOps = QStringList(), Progress progress = Progress());

  /**
   * Streams a data source from input to output.
   *
   * Associated readers/writers must implemented the partial map interfaces
   *
   * @param input data source
   * @param out data destination
   * @param convertOps a list of map ops/visitors to perform against the data during conversion
   * @param progress optional for tracking I/O job progress
   */
  static void stream(const QString& input, const QString& out,
                     const QStringList& convertOps = QStringList(), Progress progress = Progress());

  /**
   * Determines whether both input and output are streamable data sources (associated
   * readers/writers must implemented the partial map interfaces)
   *
   * @param input data source
   * @param output data destination
   * @return true if both formats are streamable; false otherwise
   */
  static bool isStreamableIo(const QString& input, const QString& output);

  /**
   * Determines whether both inputs and output are streamable data sources (associated
   * readers/writers must implemented the partial map interfaces)
   *
   * @param inputs data sources
   * @param output data destination
   * @return true if all formats are streamable; false otherwise
   */
  static bool areStreamableIo(const QStringList& inputs, const QString& output);

  /**
   * Return true if all the specified operations are valid streaming operations.
   *
   * There are some ops that require the whole map be available in RAM (e.g. remove duplicate
   * nodes). These operations are not applicable for streaming.
   *
   * @param ops
   * @return
   */
  static bool areValidStreamingOps(const QStringList& ops);

  /**
   * Get an input stream set up to be filtered by operations
   *
   * @param streamToFilter the stream to be filtered
   * @param ops a list of Hoot operation class names to use for inline filtering on the input stream
   * @return an input stream
   */
  static ElementInputStreamPtr getFilteredInputStream(
    ElementInputStreamPtr streamToFilter, const QStringList& ops);
};

}

#endif // ELEMENTSTREAMER_H
