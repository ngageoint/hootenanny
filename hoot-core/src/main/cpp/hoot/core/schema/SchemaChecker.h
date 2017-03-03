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
#ifndef __SCHEMA_CHECKER_H__
#define __SCHEMA_CHECKER_H__


// Hoot
#include <hoot/core/schema/SchemaVertex.h>

// Std
#include <vector>

namespace hoot
{

class OsmSchema;

using namespace std;

/**
 * The SchemaChecker checks if SchemaVertex is an unknown type or has no geometries listed.
 */
class SchemaChecker
{
public:

  static std::string className() { return "hoot::SchemaChecker"; }

  static unsigned int logWarnCount;

  SchemaChecker(OsmSchema& osmSchema);

  ~SchemaChecker() {}

  /**
   * Print out error message if schemavertex is unkonw vertex type.
   */

  void checkUnknownVertexType();

  /**
   * Print out error message if schemavertex has empty geometry.
   */
  void checkEmptyGeometry();

  /**
   * Print out error message if schemavertex has empty geometry or geometries are empty
   */
  void check();

  /**
   * Retrun true if schemavertex is unkonw vertex type.
   */

  static bool isUnknownVertexType(const SchemaVertex& schemaVertex) { return schemaVertex.isValid(); }

  /**
   * Return true if schemavertex has empty geometry.
   */
  static bool isEmptyGeometry(const SchemaVertex& schemaVertex) { return schemaVertex.geometries > 0; }

private:

  vector<SchemaVertex> _schemaVertexList;
};

}

#endif // __SCHEMA_CHECKER_H__
