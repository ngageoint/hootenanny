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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __SCHEMA_CHECKER_H__
#define __SCHEMA_CHECKER_H__

// Hoot
#include <hoot/core/schema/SchemaVertex.h>

namespace hoot
{

class OsmSchema;

/**
 * The SchemaChecker checks if SchemaVertex is an unknown type or has no geometries listed.
 */
class SchemaChecker
{
public:

  static QString className() { return "SchemaChecker"; }

  static int logWarnCount;

  SchemaChecker(const OsmSchema& osmSchema);
  ~SchemaChecker() = default;

  /**
   * Print out error message if schemavertex is unkonw vertex type.
   */

  void checkUnknownVertexType();

  /**
   * Print out error message if schemavertex has empty geometry or geometries are empty
   */
  void check();

  /**
   * Retrun true if schemavertex is unknown vertex type.
   */
  static bool isUnknownVertexType(const SchemaVertex& schemaVertex)
  { return schemaVertex.isValid(); }
  /**
   * Return true if schemavertex has an empty geometry.
   */
  static bool isEmptyGeometry(const SchemaVertex& schemaVertex)
  { return schemaVertex.getGeometries() > 0; }

private:

  std::vector<SchemaVertex> _schemaVertexList;

};

}

#endif // __SCHEMA_CHECKER_H__
