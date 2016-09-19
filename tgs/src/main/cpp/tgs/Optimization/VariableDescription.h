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
#ifndef VARIABLEDESCRIPTION_H
#define VARIABLEDESCRIPTION_H

// Qt
#include <QString>

// Tgs
#include <tgs/SharedPtr.h>

namespace Tgs
{
using namespace boost;

class VariableDescription
{
public:
  typedef enum
  {
    Real,
    Integer
  } VariableType;

  VariableDescription(QString name, VariableType type, double min, double max);

  VariableType getType() const { return _type; }

  double getMin() const { return _min; }

  int getMinInt() const { return (int)(_min + 0.5); }

  double getMax() const { return _max; }

  int getMaxInt() const { return (int)(_max + 0.5); }

  double getRange() const { return _max - _min; }

  QString getName() const { return _name; }

private:
  QString _name;
  double _min, _max;
  VariableType _type;
};

typedef shared_ptr<VariableDescription> VariableDescriptionPtr;
typedef shared_ptr<const VariableDescription> ConstVariableDescriptionPtr;

}

#endif // VARIABLEDESCRIPTION_H
