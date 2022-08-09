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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef V8ENGINE_H
#define V8ENGINE_H

#include <hoot/js/HootJsStable.h>

namespace hoot
{

/**
 * A singleton to initialize v8 JavaScript library. You should call getInstance() on this before
 * using any JS conflate scripts or translations.
 */
class v8Engine
{
public:

  /**
   * @brief getInstance - create or return the singleton object
   * @return reference to singleton object
   */
  static v8Engine& getInstance();

private:

  /** Creation parameters allocator for main isolate */
  std::shared_ptr<v8::ArrayBuffer::Allocator> _allocator;
  /** Main isolate */
  v8::Isolate* _isolate;
  std::shared_ptr<v8::Isolate::Scope> _isolateScope;
  /** Platform object */
  std::unique_ptr<v8::Platform> _platform;
  std::shared_ptr<v8::Locker> _locker;
  /** Main context */
  std::shared_ptr<v8::Persistent<v8::Context>> _context;
  std::shared_ptr<v8::Context::Scope> _scopeContext;

  /** private constructor/destructor */
  v8Engine();
  ~v8Engine();
  /** Delete copy constructor and assignment operator */
  v8Engine(const v8Engine&) = delete;
  v8Engine& operator=(const v8Engine&) = delete;
};

}

#endif // V8ENGINE_H
