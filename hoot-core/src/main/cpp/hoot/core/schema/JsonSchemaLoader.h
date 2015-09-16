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

#ifndef JSONSCHEMALOADER_H
#define JSONSCHEMALOADER_H

// Standard
#include <set>

#include "OsmSchema.h"

namespace hoot
{
  class JsonSchemaLoaderData;

/**
 * Loads the specified schema into OsmSchema. This is broken out to isolate the JSON code from the
 * boost includes. The two do not play nicely together.
 */
class JsonSchemaLoader
{
public:
  JsonSchemaLoader(OsmSchema& schema);

  virtual ~JsonSchemaLoader();

  void load(QString path);

  /**
   * Returns a list of the absolute paths of files that are loaded into this schema. If "import"
   * statements are used in the json file then all imported files are included in the dependency
   * list.
   */
  set<QString> getDependencies();

  static void load(OsmSchema& schema, QString path);

private:
  JsonSchemaLoaderData* _d;
};

}

#endif // JSONSCHEMALOADER_H
