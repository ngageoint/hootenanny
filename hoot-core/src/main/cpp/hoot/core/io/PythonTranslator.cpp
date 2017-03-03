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

// Python
// Python requires that it be included before other files. Ugh.
// See http://docs.python.org/c-api/intro.html#includes
#include <Python.h>

#include "PythonTranslator.h"

// hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDir>
#include <QFileInfo>

// Python version before 2.4 don't have a Py_ssize_t typedef.
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#endif

namespace hoot
{

HOOT_FACTORY_REGISTER(ScriptTranslator, PythonTranslator)

PythonTranslator::PythonTranslator()
{
}

PythonTranslator::~PythonTranslator()
{
  close();
}

void PythonTranslator::_init()
{
  LOG_INFO("Initializing Python");

  QString hootHome(getenv("HOOT_HOME"));

  QStringList pythonPath;
  QString moduleName;

  if (_scriptPath.endsWith(".py"))
  {
    QFileInfo info(_scriptPath);
    if (info.exists() == false)
    {
      throw Exception("Unable to find translation module: " + _scriptPath);
    }
    moduleName = info.baseName();
    pythonPath.append(info.dir().absolutePath());
    pythonPath.append("translations");
    pythonPath.append(QString("PYTHONPATH"));
  }
  else
  {
    moduleName = _scriptPath;
    pythonPath.append("translations");
    pythonPath.append(QString("PYTHONPATH"));
    if (hootHome.isEmpty() == false)
    {
      pythonPath.append(hootHome + "/translations");
    }
  }

  LOG_INFO("Python path: " << pythonPath.join(":"));
  setenv("PYTHONPATH", pythonPath.join(":").toUtf8().constData(), 1);
  Py_Initialize();

  char* data = moduleName.toAscii().data();
  PyObject* module = PyImport_ImportModule(data);

  if (module == NULL)
  {
    PyErr_Print();
    throw Exception("Error loading module " + _scriptPath);
  }

  _translateFunction = PyObject_GetAttrString(module, "translateAttributes");
  if (_translateFunction == NULL)
  {
    throw Exception("Error retrieving 'translateAttributes'");
  }
  if (PyCallable_Check((PyObject*)_translateFunction) == 0)
  {
    throw Exception("Error: 'translateAttributes' isn't callable");
  }
  Py_DECREF(module);
}

bool PythonTranslator::isValidScript()
{
  if (!_initialized)
  {
    try
    {
      _init();
      _initialized = true;
    }
    catch (const Exception& e)
    {
      // pass
    }
  }

  return _initialized;
}

void PythonTranslator::_finalize()
{
  LOG_INFO("Finalizing Python");
  Py_DECREF((PyObject*)_translateFunction);
  Py_Finalize();
}

void PythonTranslator::_translateToOsm(Tags& tags, const char* layerName, const char* geomType)
{
  PyObject* layerNamePy = PyString_FromString(layerName);
  PyObject* geomTypePy = PyString_FromString(geomType);
  PyObject* attrs = PyDict_New();

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); it++)
  {
    QByteArray keyUtf8 = it.key().toUtf8();
    QByteArray valueUtf8 = it.value().toUtf8();
    PyObject* key = PyUnicode_DecodeUTF8(keyUtf8.data(), keyUtf8.size(), "replace");
    PyObject* value = PyUnicode_DecodeUTF8(valueUtf8.data(), valueUtf8.size(), "replace");
    PyDict_SetItem(attrs, key, value);
    Py_DECREF(key);
    Py_DECREF(value);
  }

  PyObject* args = PyTuple_New(3);
  PyTuple_SetItem(args, 0, attrs);
  PyTuple_SetItem(args, 1, layerNamePy);
  PyTuple_SetItem(args, 2, geomTypePy);

  PyObject* pyResult = PyObject_CallObject((PyObject*)_translateFunction, args);

  Py_DECREF(args);

  tags.clear();

  if (pyResult == NULL)
  {
    PyErr_Print();
    throw Exception("Python call failed.");
  }
  else if (pyResult == Py_None)
  {
    // leave the translated result as empty.
  }
  else if (PyDict_Check(pyResult) == 0)
  {
    Py_DECREF(pyResult);
    throw Exception("Expected a dict as a return type.");
  }
  // we got a valid result.
  else
  {
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    while (PyDict_Next(pyResult, &pos, &key, &value))
    {
      PyObject* keyUnicode = PyUnicode_FromObject(key);
      PyObject* valueUnicode = PyUnicode_FromObject(value);

      if (keyUnicode == 0 || valueUnicode == 0)
      {
        throw Exception("Both the key and value in the return translation must be "
                            "convertable to strings.");
      }

      Py_UNICODE* keyUnicodeData = PyUnicode_AsUnicode(keyUnicode);
      Py_UNICODE* valueUnicodeData = PyUnicode_AsUnicode(valueUnicode);
      QString qKey, qValue;
#       if (Py_UNICODE_SIZE == 4)
      {
        qKey = QString::fromUcs4((const uint*)keyUnicodeData);
        qValue = QString::fromUcs4((const uint*)valueUnicodeData);
      }
#       elif (Py_UNICODE_SIZE == 2)
      {
#         warning "Untested unicode size."
        qKey = QString::fromWCharArray(keyUnicodeData);
        qValue = QString::fromWCharArray(valueUnicodeData);
      }
#       else
#         error "Invalid Unicode Size."
#       endif

      Py_DECREF(keyUnicode);
      Py_DECREF(valueUnicode);

      if (qValue.isEmpty() == false)
      {
        qKey = _saveMemory(qKey);
        qValue = _saveMemory(qValue);

        tags[qKey] = qValue;
      }
    }
  }
  Py_DECREF(pyResult);
}

}
