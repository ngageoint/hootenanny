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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef RECURSIVEELEMENTREMOVER_H
#define RECURSIVEELEMENTREMOVER_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstElementConsumer.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>

// Standard
#include <set>

namespace hoot
{

class OsmMap;

/**
 * Removes an element and all its children assuming the following condition is met:
 * - The child element is only contained by this element or one of its children.
 * - The element being deleted isn't contained by another element.
 *
 * To perform the operation two steps are taken.
 * 1. Determine all the children of the provided element
 * 2. For each child
 *    a. Find all the direct parents of the child.
 *    b. If any of the direct parents are not also a child, remove this child from the delete list
 * 3. Delete the element and all children that may still be referenced.
 *
 * This method assumes there are no cyclical relation links.
 *
 * This approach is very thorough and effective, but there may be much more efficient approaches on
 * a case by case basis.
 */
class RecursiveElementRemover : public ConstOsmMapOperation, public ConstElementConsumer
{
public:

  static std::string className() { return "hoot::RecursiveElementRemover"; }

  /**
   * @param eid The element to recursively delete.
   * @param criterion If this is specified then only elements that return true to "isSatisfied"
   * will be deleted. Even if isSatisfied returns false the children of that element will still
   * be searched.
   */
  RecursiveElementRemover(ElementId eid, const ElementCriterion* criterion = 0);

  /**
   * It is expected that the eid will be populated with addElement after construction.
   */
  RecursiveElementRemover() : _criterion() {}

  virtual void addElement(const ConstElementPtr& e) { _eid = e->getElementId(); }

  /**
   * Removes an element as defined by this object.
   */
  virtual void apply(const std::shared_ptr<OsmMap>& map);

  virtual QString getDescription() const { return "Recursively removes elements from a map"; }

  virtual QString getInitStatusMessage() const
  { return "Removing elements..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " elements"; }

  virtual std::string getClassName() const { return className(); }

private:

  ElementId _eid;
  const ElementCriterion* _criterion;

  void _remove(const std::shared_ptr<OsmMap>& map, ElementId eid,
               const std::set<ElementId>& removeSet);
};

}

#endif // RECURSIVEELEMENTREMOVER_H
