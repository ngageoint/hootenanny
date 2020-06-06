#ifndef ADDUSERIDVISITOR_H
#define ADDUSERIDVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

/**
 * Adds the username and user id as tags. If you want to limit the features that will be tagged then look
 * into FilteredVisitor.
 */
class AddUserIdVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::AddUserIdVisitor"; }

  AddUserIdVisitor();

  /**
   * Adds the user name and user id as tags to all valid elements.
   */
  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const { return "Adds the user name and user id to elements as tags"; }

  virtual std::string getClassName() const { return className(); }

//private:
};

}




#endif // ADDUSERIDVISITOR_H
