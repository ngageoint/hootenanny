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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTCRITERIONVISITORINPUTSTREAM_H
#define ELEMENTCRITERIONVISITORINPUTSTREAM_H

// Hoot
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Combination of ElementCriterionInputStream and ElementCriterionInputStream where a criterion
 * is first checked for each element, then one or more visitors are applied to the element.
 */
class ElementCriterionVisitorInputStream : public ElementInputStream
{
public:

  /**
   * Creates a filtered stream of elements
   *
   * @param elementSource source of the element stream
   * @param criterion criteria to filter the stream with
   * @param visitor an element visitor to filter the stream with
   */
  ElementCriterionVisitorInputStream(
    const ElementInputStreamPtr& elementSource, const ElementCriterionPtr& criterion,
    const ElementVisitorPtr& visitor);
  /**
   * Creates a filtered stream of elements
   *
   * @param elementSource source of the element stream
   * @param criterion riteria to filter the stream with
   * @param visitors an element visitor to filter the stream with
   */
  ElementCriterionVisitorInputStream(
    const ElementInputStreamPtr& elementSource, const ElementCriterionPtr& criterion,
    const QList<ElementVisitorPtr>& visitors);
  ~ElementCriterionVisitorInputStream() override;

  /**
   * @brief close
   * Invokes the close function on the source element input stream
   */
  void close() override;

  /**
   * Returns the source's projection.
   */
  std::shared_ptr<OGRSpatialReference> getProjection() const override;

  /**
   * @brief hasMoreElements
   * @return return value from call to source ElementInputStream's hasMoreElements() method
   */
  bool hasMoreElements() override;

  /**
   * @brief readNextElement
   * @return Pointer to an elemement that is read from elementSource and is satisfied by the
   *    criterion.
   */
  ElementPtr readNextElement() override;

  long getNumFeaturesTotal() const { return _numFeaturesTotal; }

  long getNumFeaturesPassingCriterion() const { return _numFeaturesPassingCriterion; }

private:

  ElementInputStreamPtr _elementSource;
  ElementCriterionPtr _criterion;

  long _numFeaturesTotal;
  long _numFeaturesPassingCriterion;

  QList<ElementVisitorPtr> _visitors;
};

}

#endif // ELEMENTCRITERIONVISITORINPUTSTREAM_H
