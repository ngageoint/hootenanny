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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef EMPTY_WAY_CRITERION_H
#define EMPTY_WAY_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Ways with no nodes; will see this from time to time as a result of cut and replace, since we
 * allow element references point to missing elements to persist
 *
 * @todo implement OperationStatus
 */
class EmptyWayCriterion : public ElementCriterion
{
public:

  static QString className() { return "hoot::EmptyWayCriterion"; }

  EmptyWayCriterion();
  virtual ~EmptyWayCriterion() = default;

  /**
   * @see ElementCriterion
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() override
  { return ElementCriterionPtr(new EmptyWayCriterion()); }

  virtual QString getDescription() const override
  { return "Identifies ways with no nodes"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }
};

}

#endif // EMPTY_WAY_CRITERION_H
