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

#include "Factory.h"

// Hoot Includes
#include <hoot/core/util/Log.h>
#include <hoot/core/util/SignalCatcher.h>

// Standard Include
#include <iostream>
using namespace std;

#include "Exception.h"

namespace hoot
{

Factory* Factory::_theInstance = NULL;

Factory::Factory()
{
}

Factory::~Factory()
{
  QMutexLocker locker(&_mutex);
  for (std::map<std::string, ObjectCreator*>::const_iterator it = _creators.begin();
       it != _creators.end(); ++it)
  {
    delete it->second;
  }
}

any Factory::constructObject(const std::string& name)
{
  QMutexLocker locker(&_mutex);
  if (_creators.find(name) == _creators.end())
  {
    throw HootException("Could not find object to construct. (" + name + ")");
  }
  ObjectCreator* c = _creators[name];
  locker.unlock();

  return c->create();
}

Factory& Factory::getInstance()
{
  if (_theInstance == NULL)
  {
    _theInstance = new Factory();
  }
  return *_theInstance;
}

vector<std::string> Factory::getObjectNamesByBase(const std::string& baseName)
{
  QMutexLocker locker(&_mutex);
  vector<std::string> result;

  for (std::map<std::string, ObjectCreator*>::const_iterator it = _creators.begin();
       it != _creators.end(); ++it)
  {
    ObjectCreator* c = it->second;
    if (c->getBaseName() == baseName)
    {
      result.push_back(c->getName());
    }
  }
  return result;
}

bool Factory::hasClass(const std::string& name)
{
  return _creators.find(name) != _creators.end();
}

void Factory::registerCreator(ObjectCreator* oc, bool baseClass)
{
  QMutexLocker locker(&_mutex);
  if (baseClass == false && oc->getBaseName() == oc->getName())
  {
    throw HootException(
      "Base name and class name are the same. Did you forget to imlement className() in "
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
    throw Exception("A class got registered multiple times. " +
                    QString::fromStdString(oc->getName()));
  }
}

}
