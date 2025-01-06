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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef ROAD_CROSSING_LANDUSE_POLY_SPLIT_H
#define ROAD_CROSSING_LANDUSE_POLY_SPLIT_H

// Hoot
#include <hoot/core/conflate/highway/RoadCrossingPolyRule.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/validation/Validator.h>

namespace hoot
{
/**
 * Splits roads in instances where they cross over BUAs and is governed by a set of rules (see
 * RoadCrossingPolyRule section of the User documentation).
 */
class RoadCrossingLandusePolySplit : public OsmMapOperation, public Configurable
{
public:

    static QString className() { return "RoadCrossingLandusePolySplit"; }

    RoadCrossingLandusePolySplit();
    ~RoadCrossingLandusePolySplit() override = default;

    /**
     * @see OsmMapOperation
     */
    void apply(OsmMapPtr& map) override;

    /**
     * @see Configurable
     */
    void setConfiguration(const Settings& conf) override;

    QString getDescription() const override { return "Splitting roads at BUA boundaries"; }
    QString getName() const override { return className(); }
    QString getClassName() const override { return className(); }

    /**
     * @see FilteredByGeometryTypeCriteria
     */
    QStringList getCriteria() const override;

private:
    
    OsmMapPtr _map;

    QString _crossingRulesFile;
    QList<RoadCrossingPolyRule> _crossingRules;

    QSet<long> _nodeIds;
    
    QSet<ElementId> _markedRoads;
    QSet<ElementId> _markedBUAs;
    int _numRoads;
    int _numBUAs;

    int _taskStatusUpdateInterval;
};

}

#endif // ROAD_CROSSING_LANDUSE_POLY_SPLIT_H