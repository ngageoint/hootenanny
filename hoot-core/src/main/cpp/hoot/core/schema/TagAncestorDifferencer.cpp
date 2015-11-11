#include "TagAncestorDifferencer.h"

#include <hoot/core/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(TagDifferencer, TagAncestorDifferencer)

TagAncestorDifferencer::TagAncestorDifferencer(QString ancestor) :
  _ancestor(ancestor)
{
  if (OsmSchema::getInstance().getTagVertex(ancestor).isEmpty())
  {
    throw IllegalArgumentException("Please specify a valid ancestor tag.");
  }
}

TagAncestorDifferencer::~TagAncestorDifferencer()
{
}

bool TagAncestorDifferencer::isValidTag(const SchemaVertex& sv) const
{
  if (_ancestor.isEmpty())
  {
    throw IllegalArgumentException("Please specify a valid ancestor tag.");
  }
  return OsmSchema::getInstance().isAncestor(sv.name, _ancestor);
}

void TagAncestorDifferencer::setConfiguration(const Settings& conf)
{
  _ancestor = ConfigOptions(conf).getTagAncestorDifferencerName();
  if (OsmSchema::getInstance().getTagVertex(_ancestor).isEmpty())
  {
    throw IllegalArgumentException("Please specify a valid ancestor tag.");
  }
}

}
