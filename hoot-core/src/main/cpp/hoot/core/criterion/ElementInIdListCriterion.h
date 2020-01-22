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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
 
#ifndef ELEMENTINIDLISTCRITERION_H
#define ELEMENTINIDLISTCRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>

// Std
#include <set>

namespace hoot
{

/**
 * Identifies elements in a list of IDs
 *
 * @todo We could probably replace this with ElementIdCriterion.
 */
class ElementInIdListCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::ElementInIdListCriterion"; }

  // TODO: rename class
  ElementInIdListCriterion() {}
  explicit ElementInIdListCriterion(const ElementType& elementType, const std::vector<long>& ids);
  ElementInIdListCriterion(const std::vector<ElementId>& ids);

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new ElementInIdListCriterion()); }

  virtual QString getDescription() const { return "Filters elements with a list of IDs"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

private:

  std::set<ElementId> _ids;
};

}
#endif // ELEMENTINIDLISTCRITERION_H
