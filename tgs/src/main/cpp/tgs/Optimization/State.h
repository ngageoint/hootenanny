#ifndef STATE_H
#define STATE_H

// Qt
#include <QMap>
#include <QSet>

#include "StateDescription.h"

namespace Tgs
{

class State
{
public:
  State();

  double get(QString name) const { return _values[name]; }

  QMap<QString, double> getAllValues() const { return _values; }

  int getInt(QString name) const { return (int)(_values[name] + 0.5); }

  double getScore() const { return _score; }

  void set(ConstVariableDescriptionPtr k, double v) { _values[k->getName()] = v; }

  void setScore(double s) { _score = s; }

  QString toString() const;

private:
  ConstStateDescriptionPtr _desc;
  double _score;

  QMap<QString, double> _values;
};

typedef shared_ptr<State> StatePtr;
typedef shared_ptr<const State> ConstStatePtr;

inline uint qHash(const ConstStatePtr& s)
{
  return qHash(s->toString());
}

}

#endif // STATE_H
