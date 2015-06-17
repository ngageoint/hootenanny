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
#ifndef OSMELEMENTSTREAMSOURCE_H
#define OSMELEMENTSTREAMSOURCE_H 

#include <boost/shared_ptr.hpp>
#include <QString>
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/OsmElementStream.h>

namespace hoot
{

/**
 * Class description
 */
class OsmElementStreamSource : public OsmElementStream
{

public:

  /**
   *
   */
  OsmElementStreamSource(
    void
  );

  /**
   *
   */
  virtual ~OsmElementStreamSource(
    void
  ) {}

  /**
   * 
   */
  virtual bool openSource(
    const QString	sourceIdentifier  	 
  ) = 0;

  /**
   * 
   */
  virtual bool isOpen(
    void 
  ) const { return mIsOpen; };

  /**
   * 
   */
  void close(
    void
  ) = 0;

  /**
   *
   */
  shared_ptr<Element> readNextElement(
     void
  ) = 0;

protected:
  bool mIsOpen;
};

}

#endif // OSMELEMENTSTREAMSOURCE_H
