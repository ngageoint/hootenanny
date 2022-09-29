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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTSTREAMER_H
#define ELEMENTSTREAMER_H

// Hoot
#include <hoot/core/util/Progress.h>

namespace hoot
{

class OgrReader;
class ElementOutputStream;

/**
 * Streams elements from input to output
 */
class ElementStreamer
{

public:

  /**
   * @brief ElementStreamer Constructor
   * @param translationScript optional translation script used to translated elements
   */
  ElementStreamer(const QString& translationScript = "");

  /**
   * Streams data sources from input to output.
   *
   * Associated readers/writers must implemented the partial map interfaces.
   *
   * @param inputs data sources
   * @param out data destination
   * @param convertOps a list of map ops/visitors to perform against the data during conversion
   * @param progress optional for tracking I/O job progress
   */
  void stream(const QStringList& inputs, const QString& out, const QStringList& convertOps = QStringList(),
              Progress progress = Progress()) const;

private:

  QString _translationScript;

  /**
   * @brief _streamFromOgr streams an OGR input to output
   * @param reader an OgrReader
   * @param input path to the input data
   * @param writer a streamable writer
   * @param convertOps conversion operations to apply to data
   * @return the number of elements written
   */
  long _streamFromOgr(const OgrReader& reader, QString& input, ElementOutputStream& writer,
                      const QStringList& convertOps = QStringList(), Progress progress = Progress()) const;
};

}

#endif // ELEMENTSTREAMER_H
