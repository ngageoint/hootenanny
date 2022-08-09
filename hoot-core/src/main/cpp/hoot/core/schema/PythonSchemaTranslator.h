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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef PYTHON_SCHEMA_TRANSLATOR_H
#define PYTHON_SCHEMA_TRANSLATOR_H

// Hoot
#include <hoot/core/schema/ScriptSchemaTranslator.h>

namespace hoot
{

class Tags;

class PythonSchemaTranslator : public ScriptSchemaTranslator
{
public:

  static QString className() { return "PythonSchemaTranslator"; }

  PythonSchemaTranslator() = default;
  ~PythonSchemaTranslator() override;

  bool isValidScript() override;

  /**
   * don't evaluate this first.
   */
  int order() const override { return 100; }

protected:

  void _init() override;
  void _finalize() override;
  void _translateToOsm(Tags& t, const char* layerName, const char* geomType) override;

private:

  // avoid including Python.h in this header file.
  void* _translateFunction;
};

}

#endif // PYTHON_SCHEMA_TRANSLATOR_H
