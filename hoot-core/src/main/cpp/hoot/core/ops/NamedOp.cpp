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
#include "NamedOp.h"

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/ops/VisitorOp.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/MemoryUsageChecker.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, NamedOp)

NamedOp::NamedOp() :
_conf(&conf())
{
}

NamedOp::NamedOp(QStringList namedOps) :
_conf(&conf()),
_namedOps(namedOps)
{
  LOG_VART(_namedOps);
  _substituteForContainingOps();
  LOG_VART(_namedOps);
}

void NamedOp::setConfiguration(const Settings& conf)
{
  _conf = &conf;
}

void NamedOp::_substituteForContainingOps()
{
  const QString mapCleanerName = MapCleaner::className();
  if (_namedOps.contains(mapCleanerName))
  {
    int cleaningOpIndex = _namedOps.indexOf(mapCleanerName);
    const QStringList mapCleanerTransforms = ConfigOptions().getMapCleanerTransforms();
    for (int i = 0; i < mapCleanerTransforms.size(); i++)
    {
      _namedOps.insert(cleaningOpIndex, mapCleanerTransforms.at(i));
      cleaningOpIndex++;
    }
    _namedOps.removeAll(mapCleanerName);
  }
}

void NamedOp::apply(OsmMapPtr& map)
{
  Factory& f = Factory::getInstance();
  QElapsedTimer timer;

  int opCount = 1;
  foreach (QString s, _namedOps)
  {
    LOG_VARD(s);
    if (s.isEmpty())
    {
      return;
    }

    timer.restart();
    LOG_DEBUG(
      "\tElement count before operation " << s << ": " <<
      StringUtils::formatLargeNumber(map->getElementCount()));
    LOG_TRACE("Projection before " << s << ": " << MapProjector::toWkt(map->getProjection()));

    // We could benefit from passing progress into some of the ops to get more granular feedback.

    std::shared_ptr<OperationStatus> statusInfo;
    if (f.hasBase<OsmMapOperation>(s))
    {
      std::shared_ptr<OsmMapOperation> op(
        Factory::getInstance().constructObject<OsmMapOperation>(s));
      statusInfo = std::dynamic_pointer_cast<OperationStatus>(op);

      if (_progress.getState() == Progress::JobState::Running)
      {
        _updateProgress(opCount - 1, _getInitMessage(s, statusInfo));
      }
      else
      {
        // In case the caller isn't using progress, we still want to get status logging.
        LOG_STATUS(_getInitMessage(s, statusInfo));
      }

      Configurable* c = dynamic_cast<Configurable*>(op.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      op->apply(map);

      _appliedOps[op->getName()] = op;
    }
    else if (f.hasBase<ElementVisitor>(s))
    {
      std::shared_ptr<ElementVisitor> vis(
        Factory::getInstance().constructObject<ElementVisitor>(s));
      statusInfo = std::dynamic_pointer_cast<OperationStatus>(vis);

      Configurable* c = dynamic_cast<Configurable*>(vis.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      if (_progress.getState() == Progress::JobState::Running)
      {
        _updateProgress(opCount - 1, _getInitMessage(s, statusInfo));
      }
      else
      {
        // In case the caller isn't using progress, we still want to get status logging.
        LOG_STATUS(_getInitMessage(s, statusInfo));
      }

      // The ordering of setting the config before the map here seems to make sense, but all
      // ElementVisitors implementing OsmMapConsumer will need to be aware of it.

      OsmMapConsumer* mapConsumer = dynamic_cast<OsmMapConsumer*>(vis.get());
      if (mapConsumer)
      {
        mapConsumer->setOsmMap(map.get());
      }

      map->visitRw(*vis);

      _appliedVis[vis->getName()] = vis;
    }
    else
    {
      throw HootException("Unexpected operation: " + s);
    }

    MemoryUsageChecker::getInstance().check();

    LOG_DEBUG(
      "\tElement count after operation " << s << ": " <<
      StringUtils::formatLargeNumber(map->getElementCount()));
    if (statusInfo.get() && !statusInfo->getCompletedStatusMessage().trimmed().isEmpty())
    {
      LOG_INFO(
        "\t" << statusInfo->getCompletedStatusMessage() + " in " +
        StringUtils::millisecondsToDhms(timer.elapsed()));
    }

    opCount++;
    OsmMapWriterFactory::writeDebugMap(map, "after-" + s.replace("hoot::", ""));

    LOG_TRACE("Projection after " << s << ": " << MapProjector::toWkt(map->getProjection()));
  }
}

void NamedOp::_updateProgress(const int currentStep, const QString& message)
{
  // Always check for a valid task weight and that the job was set to running. Otherwise, this is
  // just an empty progress object, and we shouldn't log progress.
  LOG_VART(_progress.getTaskWeight());
  LOG_VART(_progress.getState());
  if (_progress.getTaskWeight() != 0.0 && _progress.getState() == Progress::JobState::Running)
  {
    _progress.setFromRelative(
      (float)currentStep / (float)getNumSteps(), Progress::JobState::Running, message);
  }
}

QString NamedOp::_getInitMessage(const QString& message,
                                 const std::shared_ptr<OperationStatus>& statusInfo) const
{
  QString initMessage;
  if (statusInfo.get() && !statusInfo->getInitStatusMessage().trimmed().isEmpty())
  {
    initMessage += statusInfo->getInitStatusMessage();
  }
  else
  {
    initMessage += message + "...";
  }
  LOG_VART(initMessage);
  return initMessage;
}

}
