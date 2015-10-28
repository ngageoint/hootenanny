#include "TagCategoryDifferencer.h"

#include <hoot/core/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(TagDifferencer, TagCategoryDifferencer)

TagCategoryDifferencer::TagCategoryDifferencer(OsmSchemaCategory category) :
  _category(category)
{
  if (_category.toStringList().size() != 1)
  {
    throw HootException("The TagCategoryDifferencer can only filter on a single category.");
  }
}

TagCategoryDifferencer::~TagCategoryDifferencer()
{
}

bool TagCategoryDifferencer::isValidTag(const SchemaVertex& sv) const
{
  if (_category.getEnum() == OsmSchemaCategory::Empty)
  {
    throw IllegalArgumentException("You must specify exactly one category to the "
      "TagCategoryDifferencer.");
  }
  return sv.categories.contains(_category.toString());
}

void TagCategoryDifferencer::setConfiguration(const Settings& conf)
{
  _category = OsmSchemaCategory::fromString(ConfigOptions(conf).getTagCategoryDifferencerName());
  if (_category.toStringList().size() != 1)
  {
    throw HootException("The TagCategoryDifferencer can only filter on a single category.");
  }
}

}
