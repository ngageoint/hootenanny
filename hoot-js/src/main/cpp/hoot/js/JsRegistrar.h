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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef JSREGISTRAR_H
#define JSREGISTRAR_H

// Boost
#include <boost/shared_ptr.hpp>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Standard
#include <vector>

namespace hoot
{

using namespace std;
using namespace v8;
using namespace boost;

class ClassInitializer
{
public:

  ClassInitializer() {}

  virtual ~ClassInitializer() { }

  virtual void Init(Handle<Object> exports) = 0;

private:
  ClassInitializer(const ClassInitializer& oc);
  ClassInitializer& operator=(const ClassInitializer& oc);
};

template<class T>
class ClassInitializerTemplate : public ClassInitializer
{
public:
  ClassInitializerTemplate()
  {
  }

  virtual ~ClassInitializerTemplate() { }

  virtual void Init(Handle<Object> exports)
  {
    T::Init(exports);
  }

private:
};

class JsRegistrar
{
public:
  JsRegistrar();

  static JsRegistrar& getInstance();

  static void Init(Handle<Object> exports);

  void initAll(Handle<Object> exports);

  void registerInitializer(shared_ptr<ClassInitializer> ci);

private:

  vector< shared_ptr<ClassInitializer> > _initializers;
  static JsRegistrar* _theInstance;
};

template<class T>
class AutoJsRegister
{
public:

  /** Auto-register the suite factory in the global registry.
   */
  AutoJsRegister()
  {
    shared_ptr< ClassInitializerTemplate<T> > p(new ClassInitializerTemplate<T>());
    JsRegistrar::getInstance().registerInitializer(p);
  }
};

#define HOOT_JS_REGISTER(ClassName)      \
  static hoot::AutoJsRegister<ClassName> ClassName##AutoJsRegister;


}

#endif // JSREGISTRAR_H
