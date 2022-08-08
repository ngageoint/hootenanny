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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef PARTIALOSMMAPREADER_H
#define PARTIALOSMMAPREADER_H

#include <hoot/core/elements/Element.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/ElementInputStream.h>

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

  virtual ~PartialOsmMapReader() = default;

  long getMaxElementsPerMap() const { return _maxElementsPerMap; }
  void setMaxElementsPerMap(long maxElements) { _maxElementsPerMap = maxElements; }

  /**
   * The default reads the map and then calls finalizePartial();
   */
  void read(const OsmMapPtr& map) override;

  /**
   * Reads all the entries in the OsmMap.
   *
   * The default read function reads nodes, ways, then relations.
   */
  virtual void readPartial(const OsmMapPtr& map);

  /**
   * Perform any necessary initialization after the data source is opened.
   */
  virtual void initializePartial() = 0;

  /**
   * Finalize the reading of partial data. Any connections/files should be closed when this is
     complete.
   */
  virtual void finalizePartial() = 0;

protected:

  long _elementsRead;
  OsmMapPtr _partialMap;

private:

  long _maxElementsPerMap;
};

}

#endif // PARTIALOSMMAPREADER_H
