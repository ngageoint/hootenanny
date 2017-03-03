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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RequireJs.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/PluginContext.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QDir>
#include <QFile>

namespace hoot
{
using namespace v8;

HOOT_JS_REGISTER(RequireJs)

RequireJs::RequireJs()
{
}

RequireJs::~RequireJs() {}

void RequireJs::Init(Handle<Object> exports)
{
  exports->Set(String::NewSymbol("require"), FunctionTemplate::New(jsRequire)->GetFunction());
}

Handle<Value> RequireJs::jsRequire(const Arguments& args)
{
  try
  {
    HandleScope scope;
    Context::Scope context_scope(Context::GetCurrent());

    if (args.Length() != 1)
    {
      throw IllegalArgumentException("Expected exactly one argument to 'require'.");
    }

    /*
    The new Hoot "include" files are all under $HOOT_HOME/plugins & $HOOT_HOME/plugins_local
    */

    const QString hootHome = QString(getenv("HOOT_HOME"));
    if (hootHome.isEmpty())
    {
      throw HootException("$HOOT_HOME is empty.");
    }

    Settings conf;
    QStringList libPath = ConfigOptions(conf).getJavascriptTranslatorPath();

    QString scriptName = toCpp<QString>(args[0]);
    QString fullPath; // the final script path

    // Check for things like "/home/smurf" or "smurf.js"
    if (scriptName != QFileInfo(scriptName).baseName())
    {
      throw HootException("Error: Script name is a path: " + scriptName);
    }


    for (int i = 0; i < libPath.size(); i++)
    {
      QString trimmed = libPath[i].trimmed();
      if (trimmed.isEmpty() == false)
      {
        // This probably should be put into a config variable...
        fullPath = hootHome + libPath[i].trimmed() + "/" + scriptName + ".js";

        QFileInfo info(fullPath);
        if (info.exists())
        {
          break;
        }

        fullPath = "";
      }
    }

    if (fullPath.isEmpty())
    {
      throw IllegalArgumentException("Unable to load required file: " + scriptName);
    }

    QFile fp(fullPath);
    if (fp.open(QFile::ReadOnly) == false)
    {
      //      throw HootException("Error opening script: " + fullPath);
      throw HootException("Error opening script: " + fullPath);
    }

    Handle<String> source;
    Handle<Script> jsScript;

    LOG_TRACE("Loading script: " << fullPath);

    source = String::New(fp.readAll().data());

    TryCatch try_catch;
    // Compile the source code.
    jsScript = Script::Compile(source, String::New(fullPath.toUtf8().data()));

    if (jsScript.IsEmpty())
    {
      HootExceptionJs::throwAsHootException(try_catch);
    }

    // Run the script to get the result.
    HootExceptionJs::checkV8Exception(jsScript->Run(), try_catch);

    // Debug: Dump the Object
    //  Handle<Object> tObj = Context::GetCurrent()->Global();
    //  cout << "jsRequire" << endl;
    //  cout << "tObj Properties: " << tObj->GetPropertyNames() << endl;
    //  cout << endl;

    return scope.Close(Undefined());
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

}

