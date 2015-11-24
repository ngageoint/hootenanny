#ifndef __SCHEMA_CHECKER_H__
#define __SCHEMA_CHECKER_H__

// hoot
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

/**
 * The SchemaChecker checks if SchemaVertex is an unknown type or has no geometries listed.
 */
class SchemaChecker
{
public:
  SchemaChecker(OsmSchema& osmSchema);

  ~SchemaChecker() {}

  /**
   * Print out error if schemavertex is unkonw vertex type.
   */

  void checkUnknownVertexType();

  /**
   * Print out if schemavertex has empty geometry.
   */
  void checkEmptyGeometry();

  OsmSchema getOsmSchema() { return _osmSchema; }

private:
  OsmSchema _osmSchema;
};

}

#endif // __SCHEMA_CHECKER_H__
