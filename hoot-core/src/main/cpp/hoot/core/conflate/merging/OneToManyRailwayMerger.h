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
#ifndef ONE_TO_MANY_RAILWAY_MERGER_H
#define ONE_TO_MANY_RAILWAY_MERGER_H

// hoot
#include <hoot/core/conflate/merging/MergerBase.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * @brief The RelationMerger class TODO
 */
class OneToManyRailwayMerger : public MergerBase, public Configurable
{
public:

  static QString className() { return "OneToManyRailwayMerger"; }

  OneToManyRailwayMerger() = default;
  ~OneToManyRailwayMerger() override = default;

  /**
   * @see Merger
   */
  void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override { return "Removing duplicate name tags..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " duplicate name tags"; }

  QString getDescription() const override { return "Removes duplicate name tags from a feature"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:


};

}

#endif // ONE_TO_MANY_RAILWAY_MERGER_H
