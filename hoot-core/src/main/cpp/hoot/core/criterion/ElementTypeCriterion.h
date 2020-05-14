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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTTYPECRITERION_H
#define ELEMENTTYPECRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/Configurable.h>

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

  virtual QString getDescription() const
  { return "Identifies elements based on the specified type"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

protected:

  ElementType::Type _elementType;
};

// Helper classes for data driven stats
class NodeCriterion : public ElementTypeCriterion
{
public:

  static std::string className() { return "hoot::NodeCriterion"; }

  NodeCriterion() : ElementTypeCriterion(ElementType::Node) {}

  virtual QString getDescription() const { return "Identifies nodes"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }
};

class WayCriterion : public ElementTypeCriterion
{
public:

  static std::string className() { return "hoot::WayCriterion"; }

  WayCriterion() : ElementTypeCriterion(ElementType::Way) {}

  virtual QString getDescription() const { return "Identifies ways"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }
};

class RelationCriterion : public ElementTypeCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::RelationCriterion"; }

  RelationCriterion() : ElementTypeCriterion(ElementType::Relation) {}
  RelationCriterion(const QString& type) :
    ElementTypeCriterion(ElementType::Relation), _type(type.trimmed()) {}

  virtual bool isSatisfied(const ConstElementPtr& e) const override
  {
    const bool typeMatch = ElementTypeCriterion::isSatisfied(e);
    if (typeMatch)
    {
      if (_type.isEmpty())
      {
        return true;
      }
      else
      {
        return std::dynamic_pointer_cast<const Relation>(e)->getType() == _type;
      }
    }
    return false;
  }

  virtual QString getDescription() const { return "Identifies relations"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

  virtual void setConfiguration(const Settings& conf)
  {
    _type = ConfigOptions(conf).getRelationCriterionType();
  }

private:

  QString _type;
};

}

#endif // ELEMENTTYPECRITERION_H
