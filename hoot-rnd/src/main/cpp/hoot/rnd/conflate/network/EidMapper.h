#ifndef EIDMAPPER_H
#define EIDMAPPER_H

namespace hoot
{

class EidMapper
{
public:
  /**
   * Map from one eid to another. This is most useful when EIDs are replaced during the merge
   * process.
   */
  virtual ElementId mapEid(const ElementId& from) const { return from; }
};

}

#endif // EIDMAPPER_H
