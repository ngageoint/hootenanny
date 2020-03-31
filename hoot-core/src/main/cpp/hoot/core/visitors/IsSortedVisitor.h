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
 * @copyright Copyright (C) 2018, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef IS_SORTED_VISITOR_H
#define IS_SORTED_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ElementId.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Determines if OSM elements are sorted by type, then ID.  Keeps track of the most recent element
 * parsed and returns a negative result as soon as one out of order element is found.
 */
class IsSortedVisitor : public ConstElementVisitor
{
public:

  static std::string className() { return "hoot::IsSortedVisitor"; }

  IsSortedVisitor();

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const
  { return "Determines if elements are sorted to the OSM standard"; }

  bool getIsSorted() const { return _isSorted; }

  virtual std::string getClassName() const { return className(); }

private:

  ElementId _lastElementId;
  bool _isSorted;
};

}

#endif // IS_SORTED_VISITOR_H
