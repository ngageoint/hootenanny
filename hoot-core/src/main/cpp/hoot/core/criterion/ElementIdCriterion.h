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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ELEMENT_ID_CRITERION_H
#define ELEMENT_ID_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Identifies elements by type and ID
 */
class ElementIdCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::ElementIdCriterion"; }

  ElementIdCriterion();
  ElementIdCriterion(const ElementId& id);
  ElementIdCriterion(const std::set<ElementId>& ids);
  ElementIdCriterion(const ElementType& elementType, const std::set<long>& ids);

  /**
   * @see ElementCriterion
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new ElementIdCriterion()); }

  virtual QString getDescription() const { return "Identifies elements by type and ID"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

private:

  std::set<ElementId> _ids;
};

}

#endif // ELEMENT_ID_CRITERION_H
