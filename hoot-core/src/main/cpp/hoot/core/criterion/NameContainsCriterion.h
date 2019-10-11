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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef NAME_CONTAINS_CRITERION_H
#define NAME_CONTAINS_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Identifies elements which have a name containing specified text
 */
class NameContainsCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::NameContainsCriterion"; }

  NameContainsCriterion();
  NameContainsCriterion(const QStringList& names);

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new NameContainsCriterion()); }

  virtual QString getDescription() const
  { return "Identifies elements which have a name containing specified text"; }

  virtual void setConfiguration(const Settings& conf);

  void setCaseSensitive(bool caseSens) { _caseSensitive = caseSens; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

private:

  QStringList _names;
  bool _caseSensitive;
};

}

#endif // NAME_CONTAINS_CRITERION_H
