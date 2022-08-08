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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef ELEMENTITERATOR_H
#define ELEMENTITERATOR_H

// Standard
#include <list>

namespace hoot
{

class Element;

/**
 * Iterator concept for element traversal.
 */
class ElementIterator
{
public:

  ElementIterator() = default;
  virtual ~ElementIterator() = default;

  /**
   * Returns the next available Element
   *
   * @return an Element
   */
  const std::shared_ptr<Element>& next();

  /**
   * Determines if the iterator has any more available Elements
   *
   * @return true if another Element is available; false otherwise
   */
  bool hasNext();

  /**
   * Resets the element iterator
   */
  virtual void resetIterator() = 0;

protected:

  /**
   * A method to be used by the inheriting class to add new available elements.
   */
  void _addElement(const std::shared_ptr<Element>& e) { _pending.push_back(e); }

  /**
   * A method to be overridden by the implementing class. This method will be called periodically
   * by the ElementProvider to queue up elements. As long as the provider is not at the end then
   * _addElement should be called at least once each time _next is called.
   */
  virtual void _next() = 0;

private:

  std::shared_ptr<Element> _current;
  std::list<std::shared_ptr<Element>> _pending;
};

}

#endif // ELEMENTITERATOR_H
