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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __WAY_JOINER_OP_H__
#define __WAY_JOINER_OP_H__

//  Hoot
#include <hoot/core/algorithms/WayJoiner.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * @see WayJoiner
 *
 * Don't *believe* we need to make conflatable element checks in way joiners due to the fact that
 * all the code that writes parent IDs (what causes rejoins) already uses conflatable element checks
 * and/or FilteredByGeometryTypeCriteria to prevent rejoining features not configured to be
 * conflatable during a conflate operation (@see description of this behavior in ElementVisitor).
 */
class WayJoinerOp : public OsmMapOperation, public Configurable
{
public:

  WayJoinerOp();
  ~WayJoinerOp() override = default;

  static QString className() { return "WayJoinerOp"; }

  /**
   * Apply the way joiner to the specified map
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override
  { return "Rejoining ways split during conflation..."; }
  QString getCompletedStatusMessage() const override
  {
    return "Rejoined " + StringUtils::formatLargeNumber(_wayJoiner->getNumJoined()) + " pairs of ways.";
  }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Rejoins ways split during conflation"; }

  std::shared_ptr<WayJoiner> getWayJoiner() const { return _wayJoiner; }

private:

  std::shared_ptr<WayJoiner> _wayJoiner;
};

}

#endif  //  __WAY_JOINER_OP_H__
