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

#ifndef CONFLATECMD_H
#define CONFLATECMD_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/cmd/BoundedCommand.h>

// Standard
#include <fstream>

// Qt
#include <QString>

namespace hoot
{

class SingleStat;

class ConflateCmd : public BoundedCommand
{

public:

  static std::string className() { return "hoot::ConflateCmd"; }

  static const QString JOB_SOURCE;

  ConflateCmd();

  virtual QString getName() const override { return "conflate"; }

  virtual QString getDescription() const override
  { return "Conflates two maps into a single map"; }

  void printStats(const QList<SingleStat>& stats);

  virtual int runSimple(QStringList& args) override;

  void setFilterOps(bool filter) { _filterOps = filter; }

private:

  int _numTotalTasks;
  bool _filterOps;

  void _updateConfigOptionsForAttributeConflation();
  void _disableRoundaboutRemoval();
  void _checkForTagValueTruncationOverride();

  /*
   * Removes any operators specified in conflate pre/post ops that aren't associated with the
   * configured matchers
   */
  void _removeSuperfluousOps();
  /*
   * Returns all ElementCriterion class names associated with the current matcher config
   */
  QSet<QString> _getMatchCreatorCrits();
  /*
   * Given a matcher configuration determines which operators should be disabled
   *
   * @param matcherCrits a list of all ElementCriterion class names associated with the current
   * matcher config
   * @param ops a list of operator class names to be filtered
   * @param removedOps a list of the operators removed
   * @return the input operator list with unnecessary ops filtered out
   */
  QStringList _filterOutUnneededOps(
    const QSet<QString>& matcherCrits, const QStringList& ops, QSet<QString>& removedOps);

  float _getJobPercentComplete(const int currentTaskNum) const;
  float _getTaskWeight() const;

  friend class TestUtils;
};

}

#endif // CONFLATECMD_H
