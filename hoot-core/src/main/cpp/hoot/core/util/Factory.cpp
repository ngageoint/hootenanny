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

#include "Factory.h"

// Hoot
#include <hoot/core/util/SignalCatcher.h>

// Standard
#include <iostream>
using namespace std;

namespace hoot
{

boost::any Factory::constructObject(const QString& name)
{
  QString nameToUse = name;
  QMutexLocker locker(&_mutex);
  if (_creators.find(nameToUse) == _creators.end())
  {
    // This backward compatible check for the prepended namespace is in place to support legacy
    // clients that may be adding it, as well as FeatureExtractors (for now?).
    if (nameToUse.contains(MetadataTags::HootNamespacePrefix()))
    {
      nameToUse = nameToUse.remove(MetadataTags::HootNamespacePrefix());
    }
    else
    {
      nameToUse = MetadataTags::HootNamespacePrefix() + nameToUse;
    }
    if (_creators.find(nameToUse) == _creators.end())
    {
      throw HootException("Could not find object to construct. (" + name + ")");
    }
  }
  std::shared_ptr<ObjectCreator> c = _creators[nameToUse];
  locker.unlock();

  return c->create();
}

Factory& Factory::getInstance()
{
  //  Local static singleton instance
  static Factory instance;
  return instance;
}

vector<QString> Factory::getObjectNamesByBase(const QString& baseName)
{
  QMutexLocker locker(&_mutex);
  vector<QString> result;

  LOG_VART(baseName);
  for (std::map<QString, std::shared_ptr<ObjectCreator>>::const_iterator it = _creators.begin();
       it != _creators.end(); ++it)
  {
    std::shared_ptr<ObjectCreator> c = it->second;
    if (c->getBaseName() == baseName)
    {
      result.push_back(c->getName());
    }
  }
  return result;
}

bool Factory::hasClass(const QString& name) const
{
  return _creators.find(name) != _creators.end();
}

void Factory::registerCreator(const std::shared_ptr<ObjectCreator>& oc, bool baseClass)
{
  QMutexLocker locker(&_mutex);
  if (baseClass == false && oc->getBaseName() == oc->getName())
  {
    throw HootException(
      "Base name and class name are the same. Did you forget to implement className() in "
      "your class? If this is intentional, then set baseClass to true, or use the "
      "HOOT_FACTORY_REGISTER_BASE macro.  Highly unusual. (" + oc->getName() + ")");
  }
  if (_creators.find(oc->getName()) == _creators.end())
  {
    LOG_TRACE("Registering: " << oc->getName());
    _creators[oc->getName()] = oc;
  }
  else
  {
    throw HootException("A class got registered multiple times. " + oc->getName());
  }
}

}
