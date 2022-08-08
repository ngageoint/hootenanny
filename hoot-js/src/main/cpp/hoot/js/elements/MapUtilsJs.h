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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef MAP_UTILS_JS_H
#define MAP_UTILS_JS_H

// Hoot
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/HootBaseJs.h>

namespace hoot
{

/**
 * @brief The MapUtilsJs class is a JS wrapper around the MapUtils class
 */
class MapUtilsJs : public HootBaseJs
{
public:

  ~MapUtilsJs() override = default;

  static void Init(v8::Local<v8::Object> target);

  /**
   * @brief see MapUtils::getFirstElementWithTag
   * @param args 0: map, 1: tag key, 2: tag value, 3 (optional): element type string
   */
  static void getFirstElementWithTag(const v8::FunctionCallbackInfo<v8::Value>& args);
  /**
   * @brief see MapUtils::getFirstElementWithNote
   * @param args 0: map, 1: note text, 2: (optional) element type string
   */
  static void getFirstElementWithNote(const v8::FunctionCallbackInfo<v8::Value>& args);

private:

  MapUtilsJs() = default;
};

}


#endif // MAP_UTILS_JS_H
