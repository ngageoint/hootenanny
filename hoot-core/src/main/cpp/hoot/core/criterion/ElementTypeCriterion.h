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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTTYPECRITERION_H
#define ELEMENTTYPECRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ElementType.h>

namespace hoot
{

class ElementTypeCriterion : public ElementCriterion
{

public:

  static std::string className() { return "hoot::ElementTypeCriterion"; }

  ElementTypeCriterion() {}
  ElementTypeCriterion(ElementType::Type eType) : _elementType(eType) {}

  virtual ~ElementTypeCriterion() {}

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new ElementTypeCriterion(_elementType)); }

  virtual QString getDescription() const { return "Filters based on element type"; }

protected:

  ElementType::Type _elementType;
};

// Helper classes for data driven stats
class NodeTypeCriterion : public ElementTypeCriterion
{
public:
  static std::string className() { return "hoot::NodeTypeCriterion"; }
  NodeTypeCriterion() : ElementTypeCriterion(ElementType::Node) {}
};

class WayTypeCriterion : public ElementTypeCriterion
{
public:
  static std::string className() { return "hoot::WayTypeCriterion"; }
  WayTypeCriterion() : ElementTypeCriterion(ElementType::Way) {}
};

class RelationTypeCriterion : public ElementTypeCriterion
{
public:
  static std::string className() { return "hoot::RelationTypeCriterion"; }
  RelationTypeCriterion() : ElementTypeCriterion(ElementType::Relation) {}
};

}

#endif // ELEMENTTYPECRITERION_H
