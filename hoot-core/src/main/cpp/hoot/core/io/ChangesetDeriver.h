#ifndef CHANGESETDERIVER_H
#define CHANGESETDERIVER_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>

namespace hoot
{

/**
 * Derives a difference changeset between two input element streams
 */
class ChangesetDeriver : public ChangeSetProvider
{

public:

  ChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to);

  /**
   * @see ChangeSetProvider
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual ~ChangesetDeriver();

  /**
   * @see ChangeSetProvider
   */
  virtual void close();

  /**
   * @see ChangeSetProvider
   */
  virtual bool hasMoreChanges();

  /**
   * @see ChangeSetProvider
   */
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
