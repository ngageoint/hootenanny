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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMMAPWRITERFACTORY_H
#define OSMMAPWRITERFACTORY_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

// Qt
#include <QString>

// tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
class OsmMapWriter;

/**
 * A factory for constructing writers based on the URL
 */
class OsmMapWriterFactory
{
public:

  static boost::shared_ptr<OsmMapWriter> createWriter(QString url);

  static bool hasElementOutputStream(QString url);

  static void write(const boost::shared_ptr<const OsmMap>& map, QString url,
                    const bool silent = false);

  static QString getWriterName(const QString url);

  static bool isSupportedFormat(const QString url);

  static void writeDebugMap(const ConstOsmMapPtr& map, const QString title = "");

private:

  static unsigned int _debugMapCount;
};

}

#endif // OSMMAPWRITERFACTORY_H
