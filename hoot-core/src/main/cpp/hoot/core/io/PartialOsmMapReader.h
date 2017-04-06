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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PARTIALOSMMAPREADER_H
#define PARTIALOSMMAPREADER_H

#include <hoot/core/elements/Element.h>
#include "OsmMapReader.h"
#include "ElementInputStream.h"

namespace hoot
{

/**
 * Partial reading will be initialized by a call to OsmMapReader::open. The partial reader should
 * be able to read arbitrarily large data sets (AKA not memory bound).
 */
class PartialOsmMapReader : public OsmMapReader, public ElementInputStream
{

public:

  PartialOsmMapReader();

  virtual ~PartialOsmMapReader() {}

  long getMaxElementsPerMap() { return _maxElementsPerMap; }

  void setMaxElementsPerMap(long maxElements) { _maxElementsPerMap = maxElements; }

  /**
   * The default reads the map and then calls finalizePartial();
   */
  virtual void read(boost::shared_ptr<OsmMap> map);

  /**
   * Reads all the entries in the OsmMap.
   *
   * The default read function reads nodes, ways, then relations.
   */
  virtual void readPartial(boost::shared_ptr<OsmMap> map);

  /**
   * Finalize the reading of partial data. Any connections/files should be closed when this is
     complete.
   */
  virtual void finalizePartial() = 0;

  virtual bool hasMoreElements() = 0;

  virtual ElementPtr readNextElement() = 0;

protected:

  long _maxElementsPerMap;
  long _elementsRead;
  boost::shared_ptr<OsmMap> _partialMap;

};

}

#endif // PARTIALOSMMAPREADER_H
