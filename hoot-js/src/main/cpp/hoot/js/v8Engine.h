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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef V8ENGINE_H
#define V8ENGINE_H

#include "HootJsStable.h"

namespace hoot
{

/**
 * A singleton to initialize v8 JavaScript library. You should call getInstance() on this before any other
 * use of v8Engine. You can call getInstance multiple times without any ill effects.
 */
class v8Engine
{
public:

  /**
   * @brief getInstance - create or return the singleton object
   * @return reference to singleton object
   */
  static v8Engine& getInstance();

  /**
   * @brief init - A no-op that makes the code a little easier to read.
   */
  void init() {}

  ~v8Engine();

  static v8::Isolate* getIsolate();

  static void setPlatformInit(bool needsPlatform = true);

private:
  /** static pointer to the singleton instance */
  static boost::shared_ptr<v8Engine> _theInstance;
  /** static flag for platform initialization */
  static bool _needPlatform;

  /**
   * @brief v8Engine - private constructor
   */
  v8Engine();

  /** Creation parameters allocator for main isolate */
  boost::shared_ptr<v8::ArrayBuffer::Allocator> _allocator;
  /** Main isolate */
  v8::Isolate* _isolate;
  boost::shared_ptr<v8::Isolate::Scope> _isolateScope;
  /** In Hootenanny we own the Isolate, when called from Node we do not */
  bool _ownIsolate;
  /** Platform object */
  boost::shared_ptr<v8::Platform> _platform;
  boost::shared_ptr<v8::Locker> _locker;
  /** Main context */
  boost::shared_ptr<v8::Persistent<v8::Context> > _context;
  boost::shared_ptr<v8::Context::Scope> _scopeContext;

};

}

#endif // V8ENGINE_H
