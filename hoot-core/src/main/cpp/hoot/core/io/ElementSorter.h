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
#ifndef ELEMENTSORTER_H
#define ELEMENTSORTER_H

// hoot
#include <hoot/core/OsmMap.h>

#include "ElementInputStream.h"

namespace hoot
{

/**
 * An element stream that returns elements in the order of node, way, then relation, sorted by
 * element ID
 */
class ElementSorter : public ElementInputStream
{

public:

  ElementSorter(ConstOsmMapPtr map);

  /**
   * @see ElementInputStream
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual ~ElementSorter() {}

  /**
   * @see ElementInputStream
   */
  virtual void close() {}

  /**
   * @see ElementInputStream
   */
  virtual bool hasMoreElements();

  /**
   * @see ElementInputStream
   */
  virtual ElementPtr readNextElement();

private:

  ConstOsmMapPtr _source;

  vector<long> _nodeIds, _wayIds, _relationIds;
  size_t _nodeIndex, _wayIndex, _relationIndex;

};

typedef shared_ptr<ElementSorter> ElementSorterPtr;

}

#endif // ELEMENTSORTER_H
