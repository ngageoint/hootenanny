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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RequireJs.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/PluginContext.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QDir>
#include <QFile>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(RequireJs)

void RequireJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  exports->Set(String::NewFromUtf8(current, "require"),
               FunctionTemplate::New(current, jsRequire)->GetFunction());
}

void RequireJs::jsRequire(const FunctionCallbackInfo<Value>& args)
{
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope scope(current);
    Context::Scope context_scope(current->GetCurrentContext());
  try
  {

    if (args.Length() != 1)
    {
      throw IllegalArgumentException("Expected exactly one argument to 'require'.");
    }

    /*
    The new Hoot "include" files are all under $HOOT_HOME/translations & $HOOT_HOME/translations_local
    */

    const QString hootHome = QString(getenv("HOOT_HOME"));
    if (hootHome.isEmpty())
    {
      throw HootException("$HOOT_HOME is empty.");
    }

    Settings conf;
    QStringList libPath = ConfigOptions(conf).getJavascriptSchemaTranslatorPath();

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

    source = String::NewFromUtf8(current, fp.readAll().data());

    TryCatch try_catch;
    // Compile the source code.
    jsScript = Script::Compile(source, String::NewFromUtf8(current, fullPath.toUtf8().data()));

    if (jsScript.IsEmpty())
    {
      HootExceptionJs::throwAsHootException(try_catch);
    }

    Local<Value> oldExports = current->GetCurrentContext()->Global()->Get(String::NewFromUtf8(current, "exports"));

    Handle<Object> exports(Object::New(current));
    current->GetCurrentContext()->Global()->Set(String::NewFromUtf8(current, "exports"), exports);

    Handle<Value> result = jsScript->Run();

    current->GetCurrentContext()->Global()->Set(String::NewFromUtf8(current, "exports"), oldExports);

    // Run the script to get the result.
    HootExceptionJs::checkV8Exception(result, try_catch);

    // Debug: Dump the Object
    //  Handle<Object> tObj = current->GetCurrentContext()->Global();
    //  cout << "jsRequire" << endl;
    //  cout << "tObj Properties: " << tObj->GetPropertyNames() << endl;
    //  cout << endl;

    args.GetReturnValue().Set(exports);
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

}

