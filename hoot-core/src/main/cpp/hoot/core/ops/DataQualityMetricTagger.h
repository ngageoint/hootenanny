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
#ifndef _DATA_QUALITY_METRIC_TAGGER_H_
#define _DATA_QUALITY_METRIC_TAGGER_H_

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Boundable.h>

namespace hoot
{

/**
 * TODO
 *
 * There is likely overlap between this and the integrated JOSM validation. This was primarily
 * created as a quick way to diagnose C&R output quality and may go away or be merged with other
 * capability at some point.
 */
class DataQualityMetricTagger : public OsmMapOperation, public Boundable
{
public:

  static std::string className() { return "hoot::DataQualityMetricTagger"; }

  DataQualityMetricTagger();
  virtual ~DataQualityMetricTagger() = default;

  /**
   * TODO
   */
  virtual void apply(OsmMapPtr& map);

  virtual std::string getClassName() const { return className(); }

  virtual QString getInitStatusMessage() const
  { return "TODO..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return "TODO";
  }

  virtual QString getDescription() const
  { return "TODO"; }

  int getNumOrphanedNodesInOutput() const { return _orphanedNodes; }
  int getNumDisconnectedWaysInOutput() const { return _disconnectedWays; }
  int getNumEmptyWaysInOutput() const { return _emptyWays; }
  int getNumDuplicateElementPairsInOutput() const { return _duplicateElementPairs; }

private:

  // the number of orphaned nodes found in the final output if _tagQualityIssues=true
  int _orphanedNodes;
  // the number of disconnected ways found in the final output if _tagQualityIssues=true
  int _disconnectedWays;
  // the number of empty ways found in the final output if _tagQualityIssues=true
  int _emptyWays;
  // the number of duplicated elements found in the final output if _tagQualityIssues=true
  int _duplicateElementPairs;
};

}

#endif // _DATA_QUALITY_METRIC_TAGGER_H_