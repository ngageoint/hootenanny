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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REPORTMISSINGELEMENTSVISITOR_H
#define REPORTMISSINGELEMENTSVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/review/ReviewMarker.h>

namespace hoot
{

/**
 * Reports references to missing elements in a given map. If removeMissing is set to true, then
 * all missing references are removed. The option also exists to either mark elements with missing
 * children as needing review or add a custom tag to the elements.
 */
class ReportMissingElementsVisitor : public ConstElementVisitor, public OsmMapConsumer,
  public Configurable
{
public:

  static std::string className() { return "hoot::ReportMissingElementsVisitor"; }

  ReportMissingElementsVisitor(const bool removeMissing = false,
                               const Log::WarningLevel& logLevel = Log::Trace,
                               const int maxReport = Log::getWarnMessageLimit());
  virtual ~ReportMissingElementsVisitor() = default;

  virtual void visit(const ConstElementPtr& e);

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Reports references to missing elements in a map"; }

  virtual QString getInitStatusMessage() const { return "Reporting missing elements..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Reported " + StringUtils::formatLargeNumber(_missingCount) + " missing elements."; }

  virtual std::string getClassName() const { return className(); }

  int getMissingCount() const { return _missingCount; }

  int getNumWaysMarkedForReview() const { return _numWaysMarkedForReview; }
  int getNumRelationsMarkedForReview() const { return _numRelationsMarkedForReview; }
  int getNumWaysTagged() const { return _numWaysTagged; }
  int getNumRelationsTagged() const { return _numRelationsTagged; }

  void setMaxReport(int maxReport) { _maxReport = maxReport; }
  void setMarkWaysForReview(bool mark) { _markWaysForReview = mark; }
  void setMarkRelationsForReview(bool mark) { _markRelationsForReview = mark; }
  void setWayKvp(QString kvp) { _wayKvp = kvp; }
  void setRelationKvp(QString kvp) { _relationKvp = kvp; }

protected:

  OsmMap* _map;

  Log::WarningLevel _logLevel;

  int _maxReport;
  int _missingCount;
  bool _removeMissing;

  // allows for marking any elements with missing children as needing review
  bool _markWaysForReview;
  int _numWaysMarkedForReview;
  bool _markRelationsForReview;
  int _numRelationsMarkedForReview;
  ReviewMarker _reviewMarker;

  // allows for custom tagging any elements with missing children as needing review
  QString _wayKvp;
  int _numWaysTagged;
  QString _relationKvp;
  int _numRelationsTagged;

  virtual void _reportMissing(ElementId referer, ElementId missing);
  virtual void _visitAndReport(ElementType type, long id);
  virtual void _visitAndRemove(ElementType type, long id);

  void _updateWay(const WayPtr& way, const QStringList& missingChildIds);
  void _updateRelation(const RelationPtr& relation, const QStringList& missingChildIds);
};

}

#endif // REPORTMISSINGELEMENTSVISITOR_H
