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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef VISITORELEMENTINPUTSTREAM_H
#define VISITORELEMENTINPUTSTREAM_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/ElementInputStream.h>

#include <boost/shared_ptr.hpp>
#include <ogr_spatialref.h>

namespace hoot
{

class VisitorElementInputStream : public ElementInputStream
{
public:
  VisitorElementInputStream(ElementInputStreamPtr source, ElementVisitorPtr visitor);

  virtual ~VisitorElementInputStream() {}

  virtual void close() { _source->close(); }

  virtual bool hasMoreElements() { return _source->hasMoreElements(); }

  virtual ElementPtr readNextElement();

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

private:
  ElementInputStreamPtr _source;
  ElementVisitorPtr _visitor;
};

}

#endif // VISITORELEMENTINPUTSTREAM_H
