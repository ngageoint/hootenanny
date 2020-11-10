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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RECORDWRITERPROVIDER_H
#define RECORDWRITERPROVIDER_H

// Hadoop Pipes
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#pragma GCC diagnostic warning "-Wunused-parameter"

namespace pp
{

class RecordWriterProvider
{
public:
  virtual ~RecordWriterProvider() {}

  /**
   * Returns a record writer. The record writer is owned by the RecordWriterProvider.
   */
  virtual HadoopPipes::RecordWriter* getRecordWriter() const = 0;
};

}

#endif // RECORDWRITERPROVIDER_H
