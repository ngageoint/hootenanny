#ifndef STATEDESCRIPTION_H
#define STATEDESCRIPTION_H

// Qt
#include <QList>

#include "VariableDescription.h"

namespace Tgs
{

class StateDescription
{
public:
  StateDescription();

  void addVariable(ConstVariableDescriptionPtr v) { _variables.append(v); }
  void addVariable(VariableDescription* v) { _variables.append(VariableDescriptionPtr(v)); }

  QList<ConstVariableDescriptionPtr> getVariables() const { return _variables; }

private:
  QList<ConstVariableDescriptionPtr> _variables;

};

typedef shared_ptr<StateDescription> StateDescriptionPtr;
typedef shared_ptr<const StateDescription> ConstStateDescriptionPtr;

}

#endif // STATEDESCRIPTION_H
