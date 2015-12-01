
#include "SchemaChecker.h"

// hoot
#include <hoot/core/util/Log.h>

namespace hoot
{

SchemaChecker::SchemaChecker(OsmSchema& osmSchema)
{
  _schemaVertexList = osmSchema.getAllTags();
}

void SchemaChecker::checkUnknownVertexType()
{
  for (unsigned int i = 0; i < _schemaVertexList.size(); i++)
  {
    SchemaVertex schemaVertex = _schemaVertexList[i];
    if (schemaVertex.isValid() == false)
    {
      LOG_WARN("Error: unknown type. " << schemaVertex.name);
    }
  }
}

void SchemaChecker::checkEmptyGeometry()
{
  for (unsigned int i = 0; i < _schemaVertexList.size(); i++)
  {
    SchemaVertex schemaVertex = _schemaVertexList[i];
    if (schemaVertex.geometries == 0)
    {
      LOG_WARN("Error: empty geometries. " << schemaVertex.name);
    }
  }
}

void SchemaChecker::check()
{
  checkUnknownVertexType();
  checkEmptyGeometry();
}

}
