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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef FACTORY_H
#define FACTORY_H

// Boost
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

// Qt
#include <QMutex>

// Standard
#include <iostream>
#include <map>
#include <string>

#include "Exception.h"

namespace pp
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
    : _baseName(baseName), _name(name)
  {
  }

  virtual ~ObjectCreatorTemplate() { }

  /**
   * We cast it to "Base" so that the any pointer works as expected.
   */
  virtual boost::any create()
  {
    Base* b = new T();
    return dynamic_cast<Base*>(b);
  }

  std::string getBaseName() { return _baseName; }

  std::string getName() { return _name; }

private:
  std::string _baseName;
  std::string _name;
};

class Factory
{
public:
  virtual ~Factory();

  static Factory& getInstance() 
  { 
    if (_theInstance == 0)
    {
      _theInstance = new Factory();
    }
    return *_theInstance; 
  }

  /**
   * Checks to make sure the class works as expected. Nice for error checking.
   */
  template<class ExpectedBase>
  void checkClass(const std::string& name)
  {
    QMutexLocker locker(&_m);
    if (_creators.find(name) == _creators.end())
    {
      throw Exception("Could not find object to check. (" + name + ")");
    }
    if (_creators[name]->getBaseName() != ExpectedBase::className())
    {
      throw Exception("Class (" + name + ") does not have a base class of " +
        ExpectedBase::className());
    }
  }

  boost::any constructObject(const std::string& name)
  {
    if (_creators.find(name) == _creators.end())
    {
      throw Exception("Could not find object to construct. (" + name + ")");
    }
    boost::any result = _creators[name]->create();
    return result;
  }

  template<class T>
  T* constructObject(const std::string& name)
  {
    //cout << "Creating " << name << " with base type: " << typeid(T).name() << endl; cout.flush();
    boost::any a = constructObject(name);
    // The standard any_cast fails on RHEL 5.8, dunno if it is a version of g++ or what. It appears
    // that the failure is due to going across shared libraries. The any.hpp comments make mention
    // of this by the "unsafe_any_cast" function. This poor mans type checking should be good 
    // enough.
    //return any_cast<T*>(constructObject(name));
    if (strcmp(a.type().name(), typeid(T*).name()) == 0)
    {
      T* result = *boost::unsafe_any_cast<T*>(&a);
      return result;
    }
    else
    {
#     ifdef DEBUG
        std::cerr << "Creating " << name << " with base type: " << typeid(T).name() << std::endl;
        std::cerr.flush();
        std::cerr << "a.type().name() " << a.type().name() << " T name: " << typeid(T).name() << std::endl;
        std::cerr.flush();
#     endif
      throw boost::bad_any_cast();
    }
  }

  void registerCreator(boost::shared_ptr<ObjectCreator> oc);

private:
  Factory();

  static Factory* _theInstance;
  QMutex _m;

  std::map<std::string, boost::shared_ptr<ObjectCreator> > _creators;
};

template<class Base, class T>
class AutoRegister
{
public:
  /** Auto-register the suite factory in the global registry.
   */
  AutoRegister(std::string baseName, std::string name)
  {
    Factory::getInstance().registerCreator(boost::shared_ptr<ObjectCreator>(new ObjectCreatorTemplate<Base, T>(baseName, name)));
  }
};

#define PP_FACTORY_REGISTER(Base, ClassName)      \
  static pp::AutoRegister<Base, ClassName> ClassName##AutoRegister(Base::className(), \
  ClassName::className());


}

#endif // FACTORY_H
