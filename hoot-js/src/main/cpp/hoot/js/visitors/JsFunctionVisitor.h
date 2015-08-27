#ifndef JSFUNCTIONVISITOR_H
#define JSFUNCTIONVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/js/util/JsFunctionConsumer.h>
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

/**
 * A filter that will either keep or remove matches.
 */
class JsFunctionVisitor : public ElementVisitor, public JsFunctionConsumer
{
public:

  static string className() { return "hoot::JsFunctionVisitor"; }

  JsFunctionVisitor() {}

  virtual void addFunction(Persistent<Function> func) { _func = Persistent<Function>(func); }

  virtual void visit(const ConstElementPtr& e);

private:
  Persistent<Function> _func;
};

}

#endif // JSFUNCTIONVISITOR_H
