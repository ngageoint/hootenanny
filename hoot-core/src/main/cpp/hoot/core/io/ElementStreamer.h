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
#ifndef ELEMENTSTREAMER_H
#define ELEMENTSTREAMER_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Streams elements to output
 */
class ElementStreamer
{

public:

  /**
   *
   *
   * @param in
   * @param out
   * @param convertOps
   */
  static void stream(const QString in, const QString out,
                     const QStringList convertOps = QStringList());

  /**
   *
   *
   * @param input
   * @param output
   * @return
   */
  static bool isStreamableIo(const QString input, const QString output);

  /**
   * Return true if all the specified operations are valid streaming operations.
   *
   * There are some ops that require the whole map be available in RAM (e.g. remove duplicate
   * nodes). These operations are not applicable for streaming.
   *
   * @param ops
   * @return
   */
  static bool areValidStreamingOps(const QStringList ops);
};

}

#endif // ELEMENTSTREAMER_H
