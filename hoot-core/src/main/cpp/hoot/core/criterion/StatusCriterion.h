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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef STATUSCRITERION_H
#define STATUSCRITERION_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/Status.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * Matches when an element's status matches the status provided.
 */
class StatusCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "StatusCriterion"; }

  StatusCriterion();
  StatusCriterion(Status s);
  ~StatusCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<StatusCriterion>(_status); }

  void setConfiguration(const Settings& conf) override;

  QString getDescription() const override { return "Identifies elements with a particular status"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override;

private:

  Status _status;
};

}

#endif // STATUSCRITERION_H
