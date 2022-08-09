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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SETTINGSJS_H
#define SETTINGSJS_H

// hoot
#include <hoot/js/HootBaseJs.h>

namespace hoot
{

class SettingsJs : public HootBaseJs
{
public:

  ~SettingsJs() override = default;

  static void Init(v8::Local<v8::Object> target);

private:

  SettingsJs() = default;

  /**
   * @brief clear Removes all user defined settings. This is most useful for unit testing.
   * @param args
   */
  static void clear(const v8::FunctionCallbackInfo<v8::Value>& args);

  /**
   * @brief push Pushes a copy of the current settings onto an internal stack,
   *              leaving current settings unchanged.
   * @param args
   */
  static void push(const v8::FunctionCallbackInfo<v8::Value>& args);

  /**
   * @brief pop Pops settings off the stack, and overwrites the current settings.
   *              if the stack is empty, pop will do nothing.
   * @param args
   */
  static void pop(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void get(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void listContains(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void set(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void appendToList(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void prependToList(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void placeAfterInList(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void removeFromList(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void replaceInList(const v8::FunctionCallbackInfo<v8::Value>& args);
};


}

#endif // SETTINGSJS_H
