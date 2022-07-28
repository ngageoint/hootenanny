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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "TagMergerFactory.h"

// hoot
#include <hoot/core/schema/TagMerger.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

TagMergerFactory& TagMergerFactory::getInstance()
{
  //  Local static singleton instance
  static TagMergerFactory instance;
  if (!instance._default)
    instance.getDefaultPtr();
  return instance;
}

std::shared_ptr<TagMerger> TagMergerFactory::getDefaultPtr()
{
  if (!_default)
  {
    QString defaultName = ConfigOptions().getTagMergerDefault();
    LOG_DEBUG("Default tag merger is: " << defaultName);
    _default = getMergerPtr(defaultName);
  }
  return _default;
}

std::shared_ptr<TagMerger> TagMergerFactory::getMergerPtr(const QString& name)
{
  LOG_VART(name);
  std::shared_ptr<TagMerger> result;
  auto it = _mergers.find(name);
  if (it == _mergers.end())
  {
    result = Factory::getInstance().constructObject<TagMerger>(name);
    std::shared_ptr<Configurable> configurable = std::dynamic_pointer_cast<Configurable>(result);
    LOG_VART(configurable.get());
    if (configurable)
      configurable->setConfiguration(conf());
    _mergers.insert(name, result);
  }
  else
    result = it.value();
  return result;
}

Tags TagMergerFactory::mergeTags(const Tags& t1, const Tags& t2, ElementType et)
{
  return getInstance().getDefault().mergeTags(t1, t2, et);
}

void TagMergerFactory::reset()
{
  _default.reset();
  _mergers.clear();
}

QString TagMergerFactory::toString() const
{
  return QString("{ Default: %1\n  Mergers: %2 }").arg(_default ? _default->getName() : "null", _mergers.keys().join(",\n"));
}

}
