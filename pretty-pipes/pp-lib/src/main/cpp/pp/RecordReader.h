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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RECORDREADER_H
#define RECORDREADER_H

// Pretty Pipes
#include <pp/MapContextConsumer.h>
#include <pp/InputSplit.h>

namespace pp
{

class RecordReader : public HadoopPipes::RecordReader, public pp::MapContextConsumer
{
public:

  static std::string className() { return "pp::RecordReader"; }

  RecordReader() {}

  virtual ~RecordReader() {}

  virtual void setMapContext(HadoopPipes::MapContext& context);

  /**
   * This is called with the input split and context. Ownership of split is transfered to the
   * class that implements this method.
   */
  virtual void initialize(InputSplit* split, HadoopPipes::MapContext& context) = 0;

  /**
   * This is primarily useful in unit testing. It is called by setMapContext.
   */
  virtual void readInputSplit(const std::string& is);

protected:
  HadoopPipes::MapContext* _context;
};

}

#endif // RECORDREADER_H
