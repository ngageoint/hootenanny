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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTOUTPUTSTREAM_H
#define ELEMENTOUTPUTSTREAM_H

#include <hoot/core/elements/Element.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

class ElementInputStream;

/**
 * Element output stream interface.
 */
class ElementOutputStream
{
public:

  ElementOutputStream() = default;
  virtual ~ElementOutputStream() = default;

  /**
   * Close the output stream. It can safely be expected that if close isn't called explicitly it
   * will be called by the destructor.
   */
  virtual void close() = 0;

  /**
   * @brief writeElement
   */
  virtual void writeElement(ElementPtr& element) = 0;

  /**
   * Read elements from the input stream and write to the output stream. There may be a better place
   * for this to live, but it works for now.
   */
  static void writeAllElements(ElementInputStream& eis, ElementOutputStream& eos);
};

}

#endif // ELEMENTOUTPUTSTREAM_H
