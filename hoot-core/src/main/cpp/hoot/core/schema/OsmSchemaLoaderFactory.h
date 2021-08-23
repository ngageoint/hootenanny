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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSMSCHEMALOADERFACTORY_H
#define OSMSCHEMALOADERFACTORY_H

// Qt


#include <memory>

namespace hoot
{

class OsmSchemaLoader;

/**
 * (Singleton)
 */
class OsmSchemaLoaderFactory
{
public:

  static OsmSchemaLoaderFactory& getInstance();

  std::shared_ptr<OsmSchemaLoader> createLoader(QString url) const;

private:

  /** Default constructor/destructor */
  OsmSchemaLoaderFactory() = default;
  ~OsmSchemaLoaderFactory() = default;
  /** Delete copy constructor and assignment operator */
  OsmSchemaLoaderFactory(const OsmSchemaLoaderFactory&) = delete;
  OsmSchemaLoaderFactory& operator=(const OsmSchemaLoaderFactory&) = delete;
};

}

#endif // OSMSCHEMALOADERFACTORY_H
