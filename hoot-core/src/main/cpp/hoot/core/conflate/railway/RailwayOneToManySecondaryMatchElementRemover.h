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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef RAILWAY_ONE_TO_MANY_SECONDARY_MATCH_ELEMENT_REMOVER_H
#define RAILWAY_ONE_TO_MANY_SECONDARY_MATCH_ELEMENT_REMOVER_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

namespace hoot
{

/**
 * @brief The RailwayOneToManySecondaryMatchElementRemover class removes secondary features involved
 * in railway one to many matches
 */
class RailwayOneToManySecondaryMatchElementRemover : public RemoveElementsVisitor
{
public:

  static QString className() { return "RailwayOneToManySecondaryMatchElementRemover"; }

  RailwayOneToManySecondaryMatchElementRemover();
  ~RailwayOneToManySecondaryMatchElementRemover() override = default;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Removes secondary features involved in a one to many railway match."; }
};

}

#endif // RAILWAY_ONE_TO_MANY_SECONDARY_MATCH_ELEMENT_REMOVER_H
