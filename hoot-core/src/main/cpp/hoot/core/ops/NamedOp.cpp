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
}

void NamedOp::setConfiguration(const Settings& conf)
{
  _conf = &conf;
}

void NamedOp::apply(OsmMapPtr& map)
{
  Factory& f = Factory::getInstance();

  QElapsedTimer timer;
  LOG_VARD(_namedOps);
  int opCount = 1;
  foreach (QString s, _namedOps)
  {
    if (s.isEmpty())
    {
      return;
    }

    timer.restart();
    if (f.hasBase<OsmMapOperation>(s.toStdString()))
    {
      boost::shared_ptr<OsmMapOperation> t(
        Factory::getInstance().constructObject<OsmMapOperation>(s));
      boost::shared_ptr<OperationStatusInfo> statusInfo =
        boost::dynamic_pointer_cast<OperationStatusInfo>(t);

      QString initMessage = _getInitMessage(s, opCount, statusInfo);
      LOG_INFO(initMessage);
      LOG_DEBUG(
        "\tElement count before operation " << s << ": " <<
        StringUtils::formatLargeNumber(map->getElementCount()));

      Configurable* c = dynamic_cast<Configurable*>(t.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      t->apply(map);

      if (statusInfo.get() && !statusInfo->getCompletedStatusMessage().trimmed().isEmpty())
      {
        LOG_INFO(
          "\t" << statusInfo->getCompletedStatusMessage() + " in " +
          StringUtils::secondsToDhms(timer.elapsed()));
      }
    }
    else if (f.hasBase<ElementVisitor>(s.toStdString()))
    {
      boost::shared_ptr<ElementVisitor> t(
        Factory::getInstance().constructObject<ElementVisitor>(s));
      boost::shared_ptr<OperationStatusInfo> statusInfo =
        boost::dynamic_pointer_cast<OperationStatusInfo>(t);

      QString initMessage = _getInitMessage(s, opCount, statusInfo);
      LOG_INFO(initMessage);

      Configurable* c = dynamic_cast<Configurable*>(t.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      map->visitRw(*t);

      if (statusInfo.get() && !statusInfo->getCompletedStatusMessage().trimmed().isEmpty())
      {
        LOG_INFO(
          "\t" << statusInfo->getCompletedStatusMessage() + " in " +
          StringUtils::secondsToDhms(timer.elapsed()));
      }
    }
    else
    {
      throw HootException("Unexpected operation: " + s);
    }

    LOG_DEBUG(
      "\tElement count after operation " << s << ": " <<
      StringUtils::formatLargeNumber(map->getElementCount()));

    OsmMapWriterFactory::writeDebugMap(map, "after-" + s.replace("hoot::", ""));
    opCount++;
  }
}

QString NamedOp::_getInitMessage(const QString& message, int opCount, boost::shared_ptr<OperationStatusInfo> statusInfo)
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
    initMessage += ": " + message + " ...";
  }
  return initMessage;
}

}
