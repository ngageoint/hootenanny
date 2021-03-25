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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ID_TAG_MATCHES_ID_H
#define ID_TAG_MATCHES_ID_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

class IdTagMatchesId : public ElementCriterion
{
public:

  static QString className() { return "hoot::IdTagMatchesId"; }

  IdTagMatchesId() = default;
  ~IdTagMatchesId() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;

  QString getDescription() const override
  { return "Determines whether an element's " + MetadataTags::HootId() + " tag matches its ID"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

  ElementCriterionPtr clone() override { return ElementCriterionPtr(new IdTagMatchesId()); }
};

}

#endif // ID_TAG_MATCHES_ID_H
