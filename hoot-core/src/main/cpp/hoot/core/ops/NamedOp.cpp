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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NamedOp.h"

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/VisitorOp.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/MapCleaner.h>

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
  const QString mapCleanerName = QString::fromStdString(MapCleaner::className());
  if (_namedOps.contains(mapCleanerName))
  {
    int cleaningOpIndex = _namedOps.indexOf(mapCleanerName);
    LOG_VART(cleaningOpIndex);
    const QStringList mapCleanerTransforms = ConfigOptions().getMapCleanerTransforms();
    LOG_VART(mapCleanerTransforms);
    for (int i = 0; i < mapCleanerTransforms.size(); i++)
    {
      LOG_VART(i);
      LOG_VART(cleaningOpIndex);
      LOG_VART(mapCleanerTransforms.at(i));
      _namedOps.insert(cleaningOpIndex, mapCleanerTransforms.at(i));
      cleaningOpIndex++;
    }
    _namedOps.removeAll(mapCleanerName);
    LOG_VART(_progress.getTaskWeight());
    LOG_VART(_progress.getState());
    LOG_VART(_namedOps);
    LOG_VART(_progress.getTaskWeight());
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

    boost::shared_ptr<OperationStatusInfo> statusInfo;
    if (f.hasBase<OsmMapOperation>(s.toStdString()))
    {
      boost::shared_ptr<OsmMapOperation> t(
        Factory::getInstance().constructObject<OsmMapOperation>(s));
      statusInfo = boost::dynamic_pointer_cast<OperationStatusInfo>(t);

      // TODO: move into templated method

      LOG_INFO(_getInitMessage(s, opCount, statusInfo));

      LOG_VART(_progress.getTaskWeight());
      LOG_VART(_progress.getState());
      LOG_VART(_progress.getPercentComplete());
      if (_progress.getTaskWeight() != 0.0 && _progress.getState() == "RUNNING")
      {
        _progress.setFromRelative(
          (float)(opCount - 1) / (float)_namedOps.size(), "Running", false,
          _getInitMessage2(s, statusInfo));
      }

      Configurable* c = dynamic_cast<Configurable*>(t.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      // end TODO

      t->apply(map);
    }
    else if (f.hasBase<ElementVisitor>(s.toStdString()))
    {
      boost::shared_ptr<ElementVisitor> t(
        Factory::getInstance().constructObject<ElementVisitor>(s));
      statusInfo = boost::dynamic_pointer_cast<OperationStatusInfo>(t);

      // TODO: move into templated method

      LOG_INFO(_getInitMessage(s, opCount, statusInfo));

      LOG_VART(_progress.getTaskWeight());
      LOG_VART(_progress.getState());
      LOG_VART(_progress.getPercentComplete());
      if (_progress.getTaskWeight() != 0.0 && _progress.getState() == "RUNNING")
      {
        _progress.setFromRelative(
          (float)(opCount - 1) / (float)_namedOps.size(), "Running", false,
          _getInitMessage2(s, statusInfo));
      }

      Configurable* c = dynamic_cast<Configurable*>(t.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      // end TODO

      map->visitRw(*t);
    }
    else
    {
      throw HootException("Unexpected operation: " + s);
    }

    LOG_DEBUG(
      "\tElement count after operation " << s << ": " <<
      StringUtils::formatLargeNumber(map->getElementCount()));
    if (statusInfo.get() && !statusInfo->getCompletedStatusMessage().trimmed().isEmpty())
    {
      LOG_INFO(
        "\t" << statusInfo->getCompletedStatusMessage() + " in " +
        StringUtils::secondsToDhms(timer.elapsed()));
    }

    opCount++;
    OsmMapWriterFactory::writeDebugMap(map, "after-" + s.replace("hoot::", ""));
  }
}

QString NamedOp::_getInitMessage(const QString& message, int opCount,
                                 boost::shared_ptr<OperationStatusInfo> statusInfo) const
{
  QString initMessage =
    QString("Applying operation %1 / %2")
    .arg(QString::number(opCount))
    .arg(QString::number(_namedOps.size()));
  if (statusInfo.get() && !statusInfo->getInitStatusMessage().trimmed().isEmpty())
  {
    initMessage += ": " + statusInfo->getInitStatusMessage();
  }
  else
  {
    initMessage += ": " + message + "...";
  }
  return initMessage;
}

QString NamedOp::_getInitMessage2(const QString& message,
                                  boost::shared_ptr<OperationStatusInfo> statusInfo) const
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
  return initMessage;
}

}
