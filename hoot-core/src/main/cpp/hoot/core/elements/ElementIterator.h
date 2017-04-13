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

#ifndef ELEMENTPROVIDER_H
#define ELEMENTPROVIDER_H


// Standard
#include <list>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

class Element;

using namespace std;

/**
 * Implements the iterator concept and provides some simple methods for the inheriter to use.
 *
 * The inheriting class will need to implement "_next" and call "_addElement".
 */
class ElementIterator
{
public:
  ElementIterator() {}

  virtual ~ElementIterator() {}

  const boost::shared_ptr<Element>& next();

  bool hasNext();

protected:

  /**
   * A method to be used by the inheriting class to add new available elements.
   */
  void _addElement(const boost::shared_ptr<Element>& e) { _pending.push_back(e); }

  /**
   * A method to be overridden by the implementing class. This method will be called periodically
   * by the ElementProvider to queue up elements. As long as the provider is not at the end then
   * _addElement should be called at least once each time _next is called.
   */
  virtual void _next() = 0;

private:
  boost::shared_ptr<Element> _current;
  list< boost::shared_ptr<Element> > _pending;
};

}

#endif // ELEMENTPROVIDER_H
