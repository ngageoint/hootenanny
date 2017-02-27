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
#ifndef OSM_SCHEMAFILELOADER_H
#define OSM_SCHEMAFILELOADER_H

#include "OsmSchema.h"

namespace hoot
{

class OsmSchemaLoader
{
public:
  static string className() { return "hoot::OsmSchemaLoader"; }

  OsmSchemaLoader() {}

  virtual ~OsmSchemaLoader() {}

  /**
   * Returns true if the specified path is likely supported. This should be a quick operation like
   * verifying a URL, or looking at a file extension.
   */
  virtual bool isSupported(QString path) const = 0;

  /**
   * Loads the specified path (and any referenced files) into the schema s. s may already contain
   * some content in which case the new content will be added to s, or overwrite values in s as
   * appropriate.
   */
  virtual void load(QString path, OsmSchema& s) = 0;

  /**
   * Returns a list of the absolute paths of files that are loaded into this schema. If "import"
   * statements are used in the json file then all imported files are included in the dependency
   * list.
   */
  virtual set<QString> getDependencies() = 0;
};

}

#endif // OSM_SCHEMAFILELOADER_H
