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
#include <hoot/core/util/Progress.h>

// Qt
#include <QStringList>

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
   * @brief ElementStreamer TODO
   * @param translationScript
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
  void stream(
    const QStringList& inputs, const QString& out, const QStringList& convertOps = QStringList(),
    Progress progress = Progress());

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
  void stream(
    const QString& input, const QString& out, const QStringList& convertOps = QStringList(),
    Progress progress = Progress());

private:

  QString _translationScript;

  /**
   * @brief _streamOgr TODO
   * @param reader
   * @param input
   * @param writer
   * @param convertOps
   * @param progress
   */
  void _streamOgr(
    const OgrReader& reader, QString& input, ElementOutputStream& writer,
    const QStringList& convertOps = QStringList(), Progress progress = Progress());
};

}

#endif // ELEMENTSTREAMER_H
