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

#ifndef _HOOT_FACTORY_H
#define _HOOT_FACTORY_H

// Boost
#include <boost/any.hpp>

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QMutex>

// Standard
#include <map>
#include <string>
#include <vector>

namespace hoot
{

class ObjectCreator
{
public:

  ObjectCreator() {}

  virtual ~ObjectCreator() { }

  virtual boost::any create() = 0;

  virtual std::string getBaseName() = 0;

  virtual std::string getName() = 0;

private:

  ObjectCreator(const ObjectCreator& oc);
  ObjectCreator& operator=(const ObjectCreator& oc);
};

template<class Base, class T>
class ObjectCreatorTemplate : public ObjectCreator
{
public:

  ObjectCreatorTemplate(std::string baseName, std::string name)
    : _name(name), _baseName(baseName)
  {
  }

  virtual ~ObjectCreatorTemplate() { }

  /**
   * We cast it to "Base" so that the any pointer works as expected.
   */
  boost::any create() override
  {
    Base* b = new T();
    return dynamic_cast<Base*>(b);
  }

  std::string getBaseName() override { return _baseName; }

  std::string getName() override { return _name; }

private:

  std::string _name, _baseName;
};

/**
 * (Singleton)
 */
class Factory
{
public:

  virtual ~Factory();

  static Factory& getInstance();

  /**
   * Returns true if the specified class has the ExpectedBase.
   *
   * If the class isn't found an exception is thrown.
   */
  template<class ExpectedBase>
  bool hasBase(const std::string& name)
  {
    QMutexLocker locker(&_mutex);
    if (_creators.find(name) == _creators.end())
    {
      throw HootException("Could not find object to check. (" + name + ")");
    }
    if (_creators[name]->getBaseName() != ExpectedBase::className())
    {
      return false;
    }
    return true;
  }

  /**
   * Checks to make sure the class works as expected. Nice for error checking.
   */
  template<class ExpectedBase>
  void checkClass(const std::string& name)
  {
    QMutexLocker locker(&_mutex);
    if (_creators.find(name) == _creators.end())
    {
      throw HootException("Could not find object to check. (" + name + ")");
    }
    if (_creators[name]->getBaseName() != ExpectedBase::className())
    {
      throw HootException("Class (" + name + ") does not have a base class of " +
        ExpectedBase::className());
    }
  }

  boost::any constructObject(const std::string& name);

  template<class T>
  T* constructObject(const QString& name)
  {
    std::string n = name.toStdString();
    return constructObject<T>(n);
  }

  template<class T>
  T* constructObject(const std::string& name)
  {
    return boost::any_cast<T*>(constructObject(name));
  }

  std::vector<std::string> getObjectNamesByBase(const std::string& baseName);

  bool hasClass(const QString& name) { return hasClass(name.toStdString()); }

  bool hasClass(const std::string& name);

  /**
   * Register an object creator.
   *
   * @param baseClass If set to true, then the object creator may implement the base class. This is
   *  very unusual, but accidentally forgetting to implement "string className()" is quite common.
   */
  void registerCreator(boost::shared_ptr<ObjectCreator> oc, bool baseClass = false);

private:

  Factory();

  static Factory* _theInstance;

  QMutex _mutex;

  std::map<std::string, boost::shared_ptr<ObjectCreator> > _creators;
};

template<class Base, class T>
class AutoRegister
{
public:

  /** Auto-register the suite factory in the global registry.
   */
  AutoRegister(std::string baseName, std::string name, bool baseClass = false)
  {
    Factory::getInstance().registerCreator(boost::shared_ptr<ObjectCreatorTemplate<Base, T> >(
      new ObjectCreatorTemplate<Base, T>(baseName, name)), baseClass);
  }
};

#define HOOT_FACTORY_REGISTER(Base, ClassName)      \
  static hoot::AutoRegister<Base, ClassName> ClassName##AutoRegister(Base::className(), \
    ClassName::className());

/**
 * It is very unusual to register the base class, so you have to call this method to do it.
 * Otherwise you'll get a nasty exception.
 */
#define HOOT_FACTORY_REGISTER_BASE(Base)      \
  static hoot::AutoRegister<Base, Base> Base##AutoRegister(Base::className(), \
    Base::className(), true);
}

#endif // _HOOT_FACTORY_H
