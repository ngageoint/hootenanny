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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */
#ifndef MANIPULATIONDETAILS_H
#define MANIPULATIONDETAILS_H

// hoot
#include <hoot/core/elements/ElementId.h>

// Qt
#include <QString>
#include <QVariant>

// Standard
#include <map>

namespace hoot
{

/**
 * An interface that provides some deep dive details on a given manipulation. This is useful for
 * determining why a manipulation has made a given decision and possibly in experimenting with
 * reweighting the manipulation through experimentation.
 */
class ManipulationDetails
{
public:

  virtual ~ManipulationDetails() {}

  /**
   * Returns a set of eids that have been matched as being equivalent or partially equivalent by
   * this manipulation.
   */
  virtual std::set<ElementId> getMatchedElements() const = 0;
};

}

#endif // MANIPULATIONDETAILS_H
