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
#ifndef PBFELEMENTITERATOR_H
#define PBFELEMENTITERATOR_H

// hoot
#include <hoot/core/elements/ElementIterator.h>
#include "OsmPbfReader.h"

namespace hoot
{

class OsmPbfReader;

class PbfElementIterator : public ElementIterator
{
public:
  /**
   * Constructs an element reader using the specified istream. This takes ownership of the istream.
   */
  PbfElementIterator(istream* in);

  /**
   * Similar to above, but opens the given path using an fstream.
   */
  PbfElementIterator(QString path);

private:
  auto_ptr<istream> _in;
  auto_ptr<OsmPbfReader> _reader;
  vector<OsmPbfReader::BlobLocation> _blobs;
  boost::shared_ptr<OsmMap> _map;
  int _blobIndex;

  void _init(istream* in);

  virtual void _next();
};

}

#endif // PBFELEMENTITERATOR_H
