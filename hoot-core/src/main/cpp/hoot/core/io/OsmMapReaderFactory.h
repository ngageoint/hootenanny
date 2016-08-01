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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMMAPREADERFACTORY_H
#define OSMMAPREADERFACTORY_H

// Qt
#include <QString>

// tgs
#include <tgs/SharedPtr.h>

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{
class OsmMap;
class OsmMapReader;

/**
 * A factory for constructing readers based on the URL.
 */
class OsmMapReaderFactory
{

public:

  OsmMapReaderFactory();

  shared_ptr<OsmMapReader> createReader(QString url, bool useFileId = true,
                                        Status defaultStatus = Status::Invalid);

  static OsmMapReaderFactory& getInstance();

  /**
   * Returns true if a partial reader is available for the given URL.
   */
  bool hasPartialReader(QString url);

  bool hasElementInputStream(QString url);

  bool hasReader(QString url);

  static void read(shared_ptr<OsmMap> map, QString url, bool useFileId = true,
                   Status defaultStatus = Status::Invalid);

private:

  static shared_ptr<OsmMapReaderFactory> _theInstance;
};

}

#endif // OSMMAPREADERFACTORY_H
