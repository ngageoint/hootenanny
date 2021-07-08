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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CHANGE_H
#define CHANGE_H

#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * @brief The Change class represents an individual OSM change in a changeset.
 */
class Change
{

public:

  /**
   * The allowable changeset types
   */
  enum ChangeType
  {
    Create = 0,
    Modify = 1,
    Delete = 2,
    Unknown = 3
  };

  Change();
  Change(ChangeType type, ConstElementPtr element);
  Change(ChangeType type, ConstElementPtr element, ConstElementPtr pe);

  static QString changeTypeToString(const ChangeType changeType);

  QString toString() const;

  ChangeType getType() const { return _type; }
  ConstElementPtr getElement() const { return _element; }
  void clearElement() { _element.reset(); }
  ConstElementPtr getPreviousElement() const { return _previousElement; }

private:

  ChangeType _type;
  ConstElementPtr _element;
  ConstElementPtr _previousElement;
};

}

#endif // CHANGE_H
