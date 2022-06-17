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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Python redefines these two macros from /usr/include/features.h
// undefine them in this source file to remove warnings
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif
// Python requires that it be included before other files. Ugh.
// See http://docs.python.org/c-api/intro.html#includes
#include <Python.h>

#include "PythonSchemaTranslator.h"

// hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>

// Python version before 2.4 don't have a Py_ssize_t typedef.
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
using Py_ssize_t = int;
#endif

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ScriptSchemaTranslator, PythonSchemaTranslator)

PythonSchemaTranslator::~PythonSchemaTranslator()
{
  close();
}

void PythonSchemaTranslator::_init()
{
  LOG_DEBUG("Initializing Python");

  QString hootHome = ConfPath::getHootHome();

  QStringList pythonPath;
  QString moduleName;

  LOG_VART(_scriptPath);
  if (_scriptPath.endsWith(".py"))
  {
    QFileInfo info(_scriptPath);
    if (info.exists() == false)
      throw HootException("Unable to find translation module: " + _scriptPath);

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
      pythonPath.append(hootHome + "/translations");
  }
  LOG_INFO("Using translation script: " << _scriptPath);

  LOG_DEBUG("Python path: " << pythonPath.join(":"));
  setenv("PYTHONPATH", pythonPath.join(":").toUtf8().constData(), 1);
  Py_Initialize();

  const char* data = moduleName.toLatin1().data();
  PyObject* python_module = PyImport_ImportModule(data);

  if (python_module == nullptr)
  {
    PyErr_Print();
    throw HootException("Error loading module " + _scriptPath);
  }

  _translateFunction = PyObject_GetAttrString(python_module, "translateToOsm");
  if (_translateFunction == nullptr)
    throw HootException("Error retrieving 'translateToOsm'");
  if (PyCallable_Check((PyObject*)_translateFunction) == 0)
    throw HootException("Error: 'translateToOsm' isn't callable");

  Py_DECREF(python_module);
}

bool PythonSchemaTranslator::isValidScript()
{
  if (!_initialized)
  {
    try
    {
      _init();
      _initialized = true;
    }
    catch (const HootException& e)
    {
      LOG_DEBUG(e.getWhat());
    }
  }

  return _initialized;
}

void PythonSchemaTranslator::_finalize()
{
  LOG_DEBUG("Finalizing Python");
  Py_DECREF((PyObject*)_translateFunction);
  Py_Finalize();
}

void PythonSchemaTranslator::_translateToOsm(Tags& tags, const char* layerName, const char* geomType)
{
  PyObject* layerNamePy = PyString_FromString(layerName);
  PyObject* geomTypePy = PyString_FromString(geomType);
  PyObject* attrs = PyDict_New();

  for (auto it = tags.begin(); it != tags.end(); ++it)
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

  if (pyResult == nullptr)
  {
    PyErr_Print();
    throw HootException("Python call failed.");
  }
  else if (pyResult == Py_None)
  {
    // leave the translated result as empty.
  }
  else if (PyDict_Check(pyResult) == 0)
  {
    Py_DECREF(pyResult);
    throw HootException("Expected a dict as a return type.");
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

      if (keyUnicode == nullptr || valueUnicode == nullptr)
        throw HootException("Both the key and value in the return translation must be convertable to strings.");

      const Py_UNICODE* keyUnicodeData = PyUnicode_AsUnicode(keyUnicode);
      const Py_UNICODE* valueUnicodeData = PyUnicode_AsUnicode(valueUnicode);
      QString qKey, qValue;
#       if (Py_UNICODE_SIZE == 4)
      {
        qKey = QString::fromUcs4(keyUnicodeData);
        qValue = QString::fromUcs4(valueUnicodeData);
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
