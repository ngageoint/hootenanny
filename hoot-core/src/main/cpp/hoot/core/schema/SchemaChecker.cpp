
#include "SchemaChecker.h"

// hoot
#include <hoot/core/util/Log.h>

namespace hoot
{

SchemaChecker::SchemaChecker(OsmSchema& osmSchema)
{
  _osmSchema = osmSchema;
}

void SchemaChecker::checkUnknownVertexType()
{
  vector<SchemaVertex> schemaVerTexList = getOsmSchema().getAllTags();
  for (unsigned int i = 0; i < schemaVerTexList.size(); i++)
  {
    SchemaVertex schemaVertex = schemaVerTexList[i];
    if (schemaVertex.isValid() == false)
    {
      LOG_WARN("Error: unknown type. " << schemaVertex.toString());
    }
  }
}

void SchemaChecker::checkEmptyGeometry()
{
  vector<SchemaVertex> schemaVerTexList = getOsmSchema().getAllTags();
  for (unsigned int i = 0; i < schemaVerTexList.size(); i++)
  {
    SchemaVertex schemaVertex = schemaVerTexList[i];
    if (schemaVertex.geometries == 0)
    {
      LOG_WARN("Error: empty geometries. " << schemaVertex.toString());
    }
  }
}


}
