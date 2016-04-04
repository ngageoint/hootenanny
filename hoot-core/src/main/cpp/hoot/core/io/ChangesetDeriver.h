#ifndef CHANGESETDERIVER_H
#define CHANGESETDERIVER_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>

namespace hoot
{

class ChangesetDeriver : public ChangeSetProvider
{
public:
  ChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to);

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual ~ChangesetDeriver();

  virtual void close();

  virtual bool hasMoreChanges();

  virtual Change readNextChange();

private:
  ElementInputStreamPtr _from;
  ElementInputStreamPtr _to;
  Change _next;
  ElementPtr _fromE, _toE;

  Change _nextChange();
};

typedef shared_ptr<ChangesetDeriver> ChangesetDeriverPtr;

}

#endif // CHANGESETDERIVER_H
