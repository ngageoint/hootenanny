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

#ifndef TUNNEL_CRITERION_H
#define TUNNEL_CRITERION_H

// hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>

namespace hoot
{

/**
 * Identifies tunnels
 */
class TunnelCriterion : public GeometryTypeCriterion
{
public:

  static QString className() { return "TunnelCriterion"; }

  TunnelCriterion() = default;
  ~TunnelCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<TunnelCriterion>(); }

  GeometryType getGeometryType() const override { return GeometryType::Line; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override { return "Identifies tunnels"; }
};

}

#endif // BRIDGE_CRITERION_H
