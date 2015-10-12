#ifndef TAGFILTEREDDIFFERENCER_H
#define TAGFILTEREDDIFFERENCER_H

#include "TagDifferencer.h"

#include <hoot/core/schema/SchemaVertex.h>

namespace hoot
{

class TagFilteredDifferencer : public TagDifferencer
{
public:
  TagFilteredDifferencer();

  virtual double diff(const ConstOsmMapPtr& map, const ConstElementPtr& e1,
    const ConstElementPtr& e2) const;

protected:

  virtual bool isValidTag(const SchemaVertex& sv) const = 0;
};

}

#endif // TAGFILTEREDDIFFERENCER_H
