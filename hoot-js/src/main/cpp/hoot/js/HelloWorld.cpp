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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "HelloWorld.h"

// hoot
#include <hoot/core/util/Factory.h>

#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/elements/OsmMapJs.h>

using namespace std;
using namespace v8;

namespace hoot
{

/**
 * This class is used to determine if the HootJs library has already been loaded or not. This may
 * occur when using hoot from node.js. Unlike other classnames this string may be hard coded in
 * other spots. Be sure and do a thorough search before you change the class name.
 */
class HootJsLoaded
{
public:
  static QString className() { return "HootJsLoaded"; }
};

HOOT_FACTORY_REGISTER_BASE(HootJsLoaded)

void init(Local<Object> exports)
{
  JsRegistrar::Init(exports);
}

NODE_MODULE(HootJs, init)

}
