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
#ifndef REMOVEELEMENTSVISITOR_H
#define REMOVEELEMENTSVISITOR_H

// hoot
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Removes any elements where that satisfy a criterion
 */
class RemoveElementsVisitor : public MultipleCriterionConsumerVisitor, public OsmMapConsumer,
  public Configurable
{
public:

  static QString className() { return "hoot::RemoveElementsVisitor"; }

  RemoveElementsVisitor(bool negateCriteria = false);
  ~RemoveElementsVisitor() = default;

  void visit(const ElementPtr& e) override;

  void setConfiguration(const Settings& conf) override;

  void setOsmMap(OsmMap* map) override;
  void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  static void removeWays(const std::shared_ptr<OsmMap>& pMap, const ElementCriterionPtr& pCrit);

  QString getInitStatusMessage() const override { return "Removing elements..."; }
  QString getCompletedStatusMessage() const override
  {
    return
      "Removed " + StringUtils::formatLargeNumber(_numAffected) + " / " +
      StringUtils::formatLargeNumber(_numProcessed) + " elements.";
  }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes elements that satisfy a criterion"; }

  void setRecursive(bool recursive) { _recursive = recursive; }

private:

  OsmMap* _map;

  bool _recursive;
  long _startElementCount;
};


}

#endif // REMOVEELEMENTSVISITOR_H
