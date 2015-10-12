#include "TagFilteredDifferencer.h"

#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

TagFilteredDifferencer::TagFilteredDifferencer()
{
}

double TagFilteredDifferencer::diff(const ConstOsmMapPtr&, const ConstElementPtr& e1,
  const ConstElementPtr& e2) const
{
  OsmSchema& schema = OsmSchema::getInstance();

  vector<SchemaVertex> v1 = schema.getUniqueSchemaVertices(e1->getTags());
  vector<SchemaVertex> v2 = schema.getUniqueSchemaVertices(e2->getTags());

  double result = 1;

  for (size_t i = 0; i < v1.size(); ++i)
  {
    if (isValidTag(v1[i]))
    {
      for (size_t j = 0; j < v2.size(); ++j)
      {
        if (isValidTag(v2[j]))
        {
          result = min(1 - schema.score(v1[i], v2[j]), result);
        }
      }
    }
  }

  return result;
}


}
