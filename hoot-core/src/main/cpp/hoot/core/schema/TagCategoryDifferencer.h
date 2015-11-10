#ifndef TAGCATEGORYDIFFERENCER_H
#define TAGCATEGORYDIFFERENCER_H

#include "TagFilteredDifferencer.h"

#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Compare tags that have the specified category. Ignore the other tags that don't have the
 * same category.
 */
class TagCategoryDifferencer : public TagFilteredDifferencer, public Configurable
{
public:
  static string className() { return "hoot::TagCategoryDifferencer"; }

  TagCategoryDifferencer() {}
  TagCategoryDifferencer(OsmSchemaCategory category);

  virtual ~TagCategoryDifferencer();

  virtual void setConfiguration(const Settings& conf);

protected:

  OsmSchemaCategory _category;

  virtual bool isValidTag(const SchemaVertex& sv) const;
};

}

#endif // TAGCATEGORYDIFFERENCER_H
