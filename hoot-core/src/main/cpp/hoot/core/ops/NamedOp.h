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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NAMEDOP_H
#define NAMEDOP_H

// hoot
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ProgressReporter.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Applies a list of named operations to the given map. The named operations must implement either
 * OsmMapOperation or ConstElementVisitor and must be registered with the factory.
 */
class NamedOp : public OsmMapOperation, public Configurable, public ProgressReporter
{
public:

  static std::string className() { return "hoot::NamedOp"; }

  NamedOp();
  NamedOp(QStringList namedOps);

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const override { return ""; }

  virtual void setProgress(Progress progress) override { _progress = progress; }
  virtual unsigned int getNumSteps() const override { return _namedOps.size(); }

  virtual std::string getClassName() const { return className(); }

private:

  const Settings* _conf;
  QStringList _namedOps;
  Progress _progress;

  /*
   * If an op is made of a list of other ops, then this will substitute those ops in from the list.
   * This makes it easier to handle progress updates for ops. The only op that fits this case
   * currently is MapCleaner. If more are created like MapCleaner, then we may want to rethink using
   * this.
   */
  void _substituteForContainingOps();

  QString _getInitMessage(const QString& message, 
                          const std::shared_ptr<OperationStatusInfo>& statusInfo) const;
  void _updateProgress(const int currentStep, const QString& message);
};

}

#endif // NAMEDOP_H
