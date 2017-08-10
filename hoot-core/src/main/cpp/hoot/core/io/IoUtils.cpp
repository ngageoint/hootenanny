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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "IoUtils.h"

#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

boost::shared_ptr<ElementCriterion> IoUtils::getStreamingCriterion()
{
  const QStringList ops = ConfigOptions().getConvertOps();
  if (ops.size() == 0)
  {
    return boost::shared_ptr<ElementCriterion>();
  }

  QStringList criterionNames;
  Factory& factory = Factory::getInstance();
  for (int i = 0; i < ops.size(); i++)
  {
    const QString opName = ops[i];
    LOG_VART(opName);
    if (!opName.trimmed().isEmpty())
    {
      if (factory.hasBase<ElementCriterion>(opName.toStdString()))
      {
        criterionNames.append(opName);
      }
    }
  }
  LOG_VART(criterionNames.size());

  if (criterionNames.size() == 0)
  {
    return boost::shared_ptr<ElementCriterion>();
  }
  else if (criterionNames.size() != 1)
  {
    //We eventually could apply more than one and allow some simple syntax for AND/OR operations,
    //but that hasn't been needed so far.
    throw HootException(
      "Only a single convert operation can be applied during a streaming write operation.");
  }

  const QString criterionName = criterionNames[0];
  boost::shared_ptr<ElementCriterion> criterion(
    Factory::getInstance().constructObject<ElementCriterion>(criterionName));
  Configurable* c = dynamic_cast<Configurable*>(criterion.get());
  if (c != 0)
  {
    c->setConfiguration(conf());
  }

  LOG_INFO("Filtering output with criterion: " << criterionName);

  return criterion;
}
}
