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
 * @copyright Copyright (C) 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef RELATION_CRITERION_H
#define RELATION_CRITERION_H

// Hoot
#include <hoot/core/criterion/ElementTypeCriterion.h>

namespace hoot
{

/*
 * Relations are conflatable, so RelationCriterion could inherit from ConflatableElementCriterion,
 * but it hasn't been needed so far and prevents us from multiple inheritance here.
 */
class RelationCriterion : public ElementTypeCriterion, public Configurable
{
public:

  static QString className() { return "RelationCriterion"; }

  RelationCriterion();
  RelationCriterion(const QString& type);
  ~RelationCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;

  void setConfiguration(const Settings& conf) override;

  QString getDescription() const override { return "Identifies relations"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  QString _type;
};

}

#endif // RELATION_CRITERION_H
