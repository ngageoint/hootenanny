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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTCRITERIONVISITORINPUTSTREAM_H
#define ELEMENTCRITERIONVISITORINPUTSTREAM_H

#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

class ConstElementVisitor;

/**
 * Combination of ElementCriterionInputStream and ElementCriterionInputStream where a criterion
 * is first checked for each element, then a visitor is applied to the element.
 */
class ElementCriterionVisitorInputStream : public ElementInputStream
{
public:

  /**
   * @brief ElementCriterionInputStream
   * @param elementSource The stream used to read elements from
   * @param criterion If this criterion is satisfied then the element is included, otherwise the
   *    element is dropped.
   * @param visitor The operation that should be performed on elements that are read from the input source before being returned
   */
  ElementCriterionVisitorInputStream(const ElementInputStreamPtr& elementSource,
                                     const ElementCriterionPtr& criterion,
                                     const ElementVisitorPtr& visitor);

  /**
   * @brief close
   * Invokes the close function on the source element input stream
   */
  virtual void close() { _elementSource->close(); }

  /**
   * Returns the source's projection.
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  /**
   * @brief hasMoreElements
   * @return return value from call to source ElementInputStream's hasMoreElements() method
   */
  virtual bool hasMoreElements() { return _elementSource->hasMoreElements(); }

  /**
   * @brief readNextElement
   * @return Pointer to an elemement that is read from elementSource and is satisfied by the
   *    criterion.
   */
  virtual ElementPtr readNextElement();


private:

  ElementInputStreamPtr _elementSource;
  ElementCriterionPtr _criterion;
  ElementVisitorPtr _visitor;
};

}

#endif // ELEMENTCRITERIONVISITORINPUTSTREAM_H
