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
#ifndef ELEMENTVISITORINPUTSTREAM_H
#define ELEMENTVISITORINPUTSTREAM_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

class ElementVisitorInputStream : public ElementInputStream
{
public:

  /**
   * Creates a filtered stream of elements
   *
   * @param elementSource source of the element stream
   * @param visitor an element visitor to filter the stream with
   */
  ElementVisitorInputStream(const ElementInputStreamPtr& elementSource, const ElementVisitorPtr& visitor);
  ~ElementVisitorInputStream() override = default;

  /**
   * @brief close
   * Invokes the close function on the source element input stream
   */
  void close() override { _elementSource->close(); }

  /**
   * Returns the source's projection.
   */
  std::shared_ptr<OGRSpatialReference> getProjection() const override;

  /**
   * @brief hasMoreElements
   * @return return value from call to source ElementInputStream's hasMoreElements() method
   */
  bool hasMoreElements() override { return _elementSource->hasMoreElements(); }

  /**
   * @brief readNextElement
   * @return Pointer to an elemement which will have been read from the source elementinputstream
   *    AND had the visitor's visit method applied to it before it is returned
   */
  ElementPtr readNextElement() override;

  ElementVisitorPtr getVisitor() const { return _visitor; }

private:

  ElementInputStreamPtr _elementSource;
  ElementVisitorPtr _visitor;
};

}

#endif // ELEMENTVISITORINPUTSTREAM_H
