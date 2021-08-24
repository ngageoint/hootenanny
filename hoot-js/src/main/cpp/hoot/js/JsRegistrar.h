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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef JSREGISTRAR_H
#define JSREGISTRAR_H

// hoot
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

class ClassInitializer
{
public:

  ClassInitializer() = default;
  virtual ~ClassInitializer() = default;

  virtual void Init(v8::Local<v8::Object> exports) = 0;

private:

  ClassInitializer(const ClassInitializer& oc);
  ClassInitializer& operator=(const ClassInitializer& oc);
};

template<class T>
class ClassInitializerTemplate : public ClassInitializer
{
public:

  ClassInitializerTemplate() = default;
  ~ClassInitializerTemplate() = default;

  void Init(v8::Local<v8::Object> exports) override
  {
    T::Init(exports);
  }
};

/**
 * (Singleton)
 */
class JsRegistrar
{
public:

  static JsRegistrar& getInstance();

  static void Init(v8::Local<v8::Object> exports);
  void initAll(v8::Local<v8::Object> exports);

  void registerInitializer(const std::shared_ptr<ClassInitializer>& ci);

private:

  std::vector<std::shared_ptr<ClassInitializer>> _initializers;

  /** Default constructor/destructor */
  JsRegistrar() = default;
  ~JsRegistrar() = default;
  /** Delete copy constructor and assignment operator */
  JsRegistrar(const JsRegistrar&) = delete;
  JsRegistrar& operator=(const JsRegistrar&) = delete;
};

template<class T>
class AutoJsRegister
{
public:

  /** Auto-register the suite factory in the global registry.
   */
  AutoJsRegister()
  {
    JsRegistrar::getInstance().registerInitializer(std::make_shared<ClassInitializerTemplate<T>>());
  }
};

#define HOOT_JS_REGISTER(ClassName)      \
  static hoot::AutoJsRegister<ClassName> ClassName##AutoJsRegister;

}

#endif // JSREGISTRAR_H
