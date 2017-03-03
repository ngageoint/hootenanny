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
#ifndef ELEMENTINPUTSTREAM_H
#define ELEMENTINPUTSTREAM_H

#include <ogr_spatialref.h>

#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 *
 */
class ElementInputStream
{

public:

  /**
   *
   */
  ElementInputStream() {}

  /**
   * @brief getProjection
   * @return
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  /**
   * @brief ~ElementInputStream
   *
   * If the stream is open when the destructor is called, closeStream must be called in the destructor
   */
  virtual ~ElementInputStream() {}

  /**
   * @brief closeStream
   *
   * Releases all resources associated with the stream, if any
   */
  virtual void close() = 0;

  /**
   *
   */
  virtual bool hasMoreElements() = 0;

  /**
   *
   */
  virtual ElementPtr readNextElement() = 0;
};

typedef boost::shared_ptr<ElementInputStream> ElementInputStreamPtr;

}

#endif // ELEMENTINPUTSTREAM_H
