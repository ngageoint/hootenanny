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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PLUGINCONTEXT_H
#define PLUGINCONTEXT_H

// hoot
#include <hoot/js/util/StreamUtilsJs.h>

// Qt
#include <QString>

// Standard
#include <memory>

// tgs
#include <tgs/SharedPtr.h>

// v8
#include <v8.h>

namespace hoot
{
using namespace v8;

/**
 * This is the context from which a single plugin is run. Each plugin gets its own context and
 * therefore shouldn't stomp on any other plugin's name's, etc.
 */
class PluginContext
{
public:
  static const double UNSPECIFIED_DEFAULT;

  PluginContext();
  ~PluginContext();

  /**
   * Method that simplifies calling a function.
   * @param obj - Object that owns the function (maybe global?)
   * @param name - Name of the function
   * @param args - List of arguments to the function. Converted using "toValue"
   */
  Local<Value> call(Handle<Object> obj, QString name, QList<QVariant> args = QList<QVariant>());

  Local<Value> eval(QString e);

  Persistent<Context> getContext() { return _context; }


  bool hasFunction(QString name);

  /**
   * Returns true if the object v contains the key specified.
   */
  bool hasKey(Handle<Value> v, QString key);

  Local<Object> loadScript(QString filename, QString loadInto = "");

  // Will refacter this later to get rid of duplication
  Local<Object> loadText(QString text, QString loadInto = "", QString scriptName = "<Unknown>");

  /**
   * Returns the specified key. If defaultValue is not UNSPECIFIED_DEFAULT then it will be used if
   * the key doesn't exist. If you need to use UNSPECIFIED_DEFAULT as the default value, use
   * another method :P.
   */
  double toNumber(Handle<Value> v, QString key, double defaultValue = UNSPECIFIED_DEFAULT);

  Local<Value> toValue(QVariant v);

private:

  Persistent<Context> _context;
};

}

#endif // PLUGINCONTEXT_H
