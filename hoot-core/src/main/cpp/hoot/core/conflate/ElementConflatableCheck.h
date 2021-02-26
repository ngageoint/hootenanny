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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENT_CONFLATABLE_CHECK_H
#define ELEMENT_CONFLATABLE_CHECK_H

// Qt
#include <QStringList>

namespace hoot
{

/**
 * TODO
 *
 * Note that this is not a true interface in that parts are implemented. If we ever have
 * multiple inheritance issues with children, it can be converted to a true interface.
 *
 * @see ElementVisitor
 */
class ElementConflatableCheck
{
public:

  ElementConflatableCheck() : _checkConflatable(false) {}
  virtual ~ElementConflatableCheck() = default;

  /**
   * @see ElementConflatableCheck
   */
  virtual bool getCheckConflatable() const { return _checkConflatable; }
  /**
   * @see ElementConflatableCheck
   */
  virtual void setCheckConflatable(const bool checkConflatable)
  { _checkConflatable = checkConflatable; }

protected:

  bool _checkConflatable;
};

}

#endif // ELEMENT_CONFLATABLE_CHECK_H
