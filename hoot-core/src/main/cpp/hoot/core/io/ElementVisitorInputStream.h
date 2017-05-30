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
#ifndef ELEMENTVISITORINPUTSTREAM_H
#define ELEMENTVISITORINPUTSTREAM_H

#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

class ElementVisitor;

class ElementVisitorInputStream : public ElementInputStream
{
public:
  /**
   * @brief ElementVisitorInputStream
   * @param elementSource The stream used to read elements from
   * @param visitor The operation that should be performed on elements that are read from the input source before being returned
   */
  ElementVisitorInputStream(const boost::shared_ptr<ElementInputStream>& elementSource,
                            const boost::shared_ptr<ElementVisitor>& visitor);

  /**
   * @brief close
   * Invokes the close function on the source element input stream
   */
  virtual void close() { _elementSource->close(); }

  /**
   * @brief hasMoreElements
   * @return return value from call to source ElementInputStream's hasMoreElements() method
   */
  virtual bool hasMoreElements() { return _elementSource->hasMoreElements(); }

  /**
   * @brief readNextElement
   * @return Pointer to an elemement which will have been read from the source elementinputstream
   *    AND had the visitor's visit method applied to it before it is returned
   */
  virtual ElementPtr readNextElement();


private:
  boost::shared_ptr<ElementInputStream> _elementSource;
  boost::shared_ptr<ElementVisitor> _visitor;
};

}

#endif // ELEMENTVISITORINPUTSTREAM_H
