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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef IDSWAP_H
#define IDSWAP_H

#include <array>
#include <map>

//  Hoot
#include <hoot/core/elements/ElementId.h>

namespace hoot
{

class IdSwap
{
public:

  /** Helpful typedefs for container and iterators */
  typedef std::map<ElementId, ElementId> container;
  typedef typename container::iterator iterator;
  typedef typename container::const_iterator const_iterator;

  IdSwap();
  IdSwap(ElementId id_1, ElementId id_2) { add(id_1, id_2); }

  /**
   * @brief add
   * @param type
   * @param id_1
   * @param id_2
   */
  void add(ElementId id_1, ElementId id_2);
  /**
   * @brief begin Begin iterator
   * @return iterator pointing to the beginning of the set
   */
  iterator begin();
  /**
   * @brief end End iterator
   * @return iterator pointing off of the end of the set
   */
  iterator end();
  /**
   * @brief size Total number of elements to swap
   * @return count
   */
  size_t size();

private:

  /** Containers to hold ID swap information, forward and backward mappings */
  container _idMap;
  container _idMapReversed;
};

typedef std::shared_ptr<IdSwap> IdSwapPtr;

}

#endif  //  IDSWAP_H
