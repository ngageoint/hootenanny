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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef IMPLICIT_TAG_ELIGIBLE_POI_CRITERION_H
#define IMPLICIT_TAG_ELIGIBLE_POI_CRITERION_H

// hoot
#include <hoot/rnd/criterion/ImplicitTagEligibleCriterion.h>

namespace hoot
{

/**
 * Any POI whose name can be harvested to feed the POI implicit tagger will pass this criterion
 */
class ImplicitTagEligiblePoiCriterion : public ImplicitTagEligibleCriterion
{
public:

  static QString className() { return "ImplicitTagEligiblePoiCriterion"; }

  ImplicitTagEligiblePoiCriterion() = default;
  ~ImplicitTagEligiblePoiCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override
  { return std::make_shared<ImplicitTagEligiblePoiCriterion>(); }

  /**
   * Returns all tag key/value pairs which could be applied implicitly by an implicit POI tagger
   *
   * @param tags tags to examine
   * @return a list of key/value pairs (key=value)
   */
  QStringList getEligibleKvps(const Tags& tags) const override;
  /**
   * Returns true if the input tags contain at least one key/value pair which could be applied
   * implicitly by an implicit POI tagger
   *
   * @param tags tags to examine
   * @return true if any eligible tags are present; false otherwise
   */
  bool hasEligibleKvp(const Tags& tags) const override;

  QString getDescription() const override
  { return "Identifies POIs eligible for type tag addition"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}
#endif
