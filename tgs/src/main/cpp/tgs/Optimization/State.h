/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
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
