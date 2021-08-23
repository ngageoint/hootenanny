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
#ifndef PLUGINCONTEXT_H
#define PLUGINCONTEXT_H

// hoot
#include <hoot/js/io/StreamUtilsJs.h>

// v8
#include <hoot/js/HootJsStable.h>

namespace hoot
{

/**
 * This is the context from which a single plugin is run. Each plugin gets its own context and
 * therefore shouldn't stomp on any other plugin's name's, etc.
 */
class PluginContext
{
public:

  static const double UNSPECIFIED_DEFAULT;

  PluginContext();
  ~PluginContext() = default;

  /**
   * Method that simplifies calling a function.
   * @param obj - Object that owns the function (maybe global?)
   * @param name - Name of the function
   * @param args - List of arguments to the function. Converted using "toValue"
   */
  v8::Local<v8::Value> call(
    v8::Local<v8::Object> obj, QString name, QList<QVariant> args = QList<QVariant>());

  v8::Local<v8::Value> eval(QString e);

  v8::Local<v8::Context> getContext(v8::Isolate* isolate);

  bool hasFunction(QString name);

  v8::Local<v8::Object> loadScript(QString filename, QString loadInto = "");

  v8::Local<v8::Object> loadText(
    QString text, QString loadInto = "", QString scriptName = "<Unknown>");

  /**
   * Returns the specified key. If defaultValue is not UNSPECIFIED_DEFAULT then it will be used if
   * the key doesn't exist. If you need to use UNSPECIFIED_DEFAULT as the default value, use
   * another method :P.
   */
  double toNumber(
    v8::Local<v8::Value> v, QString key, double defaultValue = UNSPECIFIED_DEFAULT) const;

  v8::Local<v8::Value> toValue(QVariant v) const;

private:

  v8::Persistent<v8::Context> _context;
};

}

#endif // PLUGINCONTEXT_H
