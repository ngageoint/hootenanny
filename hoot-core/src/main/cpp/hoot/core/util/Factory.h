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

#ifndef _HOOT_FACTORY_H
#define _HOOT_FACTORY_H

// Boost
#include <boost/any.hpp>

// Hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

// Standard
#include <map>
#include <mutex>
#include <vector>

namespace hoot
{

class ObjectCreator
{
public:

  ObjectCreator() = default;
  virtual ~ObjectCreator() = default;

  virtual boost::any create() = 0;
  virtual QString getBaseName() const = 0;
  virtual QString getName() const = 0;

private:

  ObjectCreator(const ObjectCreator& oc);
  ObjectCreator& operator=(const ObjectCreator& oc);
};

template<class Base, class T>
class ObjectCreatorTemplate : public ObjectCreator
{
public:

  ObjectCreatorTemplate(QString baseName, QString name)
    : _name(name), _baseName(baseName)
  {
  }
  virtual ~ObjectCreatorTemplate() = default;

  /**
   * We cast it to "Base" so that the any pointer works as expected.
   */
  boost::any create() override
  {
    std::shared_ptr<Base> b = std::make_shared<T>();
    return std::dynamic_pointer_cast<Base>(b);
  }

  QString getBaseName() const override { return _baseName; }
  QString getName() const override { return _name; }

private:

  QString _name;
  QString _baseName;
};

/**
 * (Singleton)
 */
class Factory
{
public:

  static Factory& getInstance();

  boost::any constructObject(const QString& name);
  template<class T>
  std::shared_ptr<T> constructObject(const QString& name)
  {
    return boost::any_cast<std::shared_ptr<T>>(constructObject(name));
  }
  /**
   * Register an object creator.
   *
   * @param baseClass If set to true, then the object creator may implement the base class. This is
   * very unusual, but accidentally forgetting to implement "string className()" is quite common.
   */
  void registerCreator(const std::shared_ptr<ObjectCreator>& oc, bool baseClass = false);

  /**
   * Returns true if the specified class has the ExpectedBase.
   *
   * If the class isn't found an exception is thrown.
   */
  template<class ExpectedBase>
  bool hasBase(const QString& name)
  {
    std::lock_guard<std::mutex> locker(_mutex);
    if (_creators.find(name) == _creators.end())
      throw HootException("Could not find object to construct. (" + name + ")");
    if (_creators[name]->getBaseName() != ExpectedBase::className())
      return false;
    return true;
  }
  bool hasClass(const QString& name) const;

  /**
   * Checks to make sure the class works as expected. Nice for error checking.
   */
  template<class ExpectedBase>
  void checkClass(const QString& name)
  {
    std::lock_guard<std::mutex> locker(_mutex);
    if (_creators.find(name) == _creators.end())
      throw HootException("Could not find object to construct. (" + name + ")");
    if (_creators[name]->getBaseName() != ExpectedBase::className())
      throw HootException("Class (" + name + ") does not have a base class of " + ExpectedBase::className());
  }

  std::vector<QString> getObjectNamesByBase(const QString& baseName);

private:

  std::mutex _mutex;

  std::map<QString, std::shared_ptr<ObjectCreator>> _creators;

  /** Default constructor/destructor */
  Factory() = default;
  ~Factory() = default;
  /** Delete copy constructor and assignment operator */
  Factory(const Factory&) = delete;
  Factory& operator=(const Factory&) = delete;
};

template<class Base, class T>
class AutoRegister
{
public:

  /** Auto-register the suite factory in the global registry.
   */
  AutoRegister(QString baseName, QString name, bool baseClass = false)
  {
    Factory::getInstance().registerCreator(
      std::make_shared<ObjectCreatorTemplate<Base, T>>(baseName, name), baseClass);
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
