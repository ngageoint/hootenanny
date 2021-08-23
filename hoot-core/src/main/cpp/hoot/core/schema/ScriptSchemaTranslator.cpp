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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "ScriptSchemaTranslator.h"

// hoot



// Standard
#include <assert.h>

namespace hoot
{

ScriptSchemaTranslator::ScriptSchemaTranslator()
{
  _initialized = false;
  _strict = StrictOn;
}

ScriptSchemaTranslator::~ScriptSchemaTranslator()
{
  // the child class should call close since _finalize is virtual.
  assert (_initialized == false);
}

void ScriptSchemaTranslator::close()
{
  if (_initialized)
  {
    _finalize();
    _initialized = false;
  }
}

const QString& ScriptSchemaTranslator::_saveMemory(const QString& s)
{
  if (!_strings.contains(s))
  {
    _strings[s] = s;
  }
  return _strings[s];
}

void ScriptSchemaTranslator::strictError(const QString& s) const
{
  if (_strict == StrictOn)
  {
    throw HootException(s);
  }
  else if (_strict == StrictWarn)
  {
    LOG_WARN(s);
  }
}

void ScriptSchemaTranslator::translateToOsm(Tags& tags, const char* layerName, const char* geomType)
{
  if (!_initialized)
  {
    _init();
    _initialized = true;
  }
  _translateToOsm(tags, layerName, geomType);
}
}
