#ifndef __TAG_ANCESTOR_DIFFERENCER_H__
#define __TAG_ANCESTOR_DIFFERENCER_H__

#include "TagFilteredDifferencer.h"

#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class TagAncestorDifferencer : public TagFilteredDifferencer, public Configurable
{
public:
  static string className() { return "hoot::TagAncestorDifferencer"; }

  TagAncestorDifferencer() {}
  TagAncestorDifferencer(QString ancestor);

  virtual ~TagAncestorDifferencer();

  virtual void setConfiguration(const Settings& conf);

protected:
  QString _ancestor;

  virtual bool isValidTag(const SchemaVertex& sv) const;
};

}

#endif // __TAG_ANCESTOR_DIFFERENCER_H__
