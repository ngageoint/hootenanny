#ifndef EXTRACTNODESVISITOR_H
#define EXTRACTNODESVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Node.h>

// Qt
#include <QList>

namespace hoot
{

class ExtractNodesVisitor :  public ElementVisitor
{
public:

  ExtractNodesVisitor(QList<ConstNodePtr>& n) : _n(n) {}

  virtual void visit(const shared_ptr<const Element>& e)
  {
    if (e->getElementType() == ElementType::Node)
    {
      ConstNodePtr n = dynamic_pointer_cast<const Node>(e);
      _n.append(n);
    }
  }

private:
  QList<ConstNodePtr>& _n;
};

}

#endif // EXTRACTNODESVISITOR_H
