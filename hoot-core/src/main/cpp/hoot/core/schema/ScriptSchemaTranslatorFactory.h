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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SCRIPT_SCHEMA_TRANSLATOR_FACTORY_H
#define SCRIPT_SCHEMA_TRANSLATOR_FACTORY_H

namespace hoot
{

class ScriptSchemaTranslator;

/**
 * (Singleton)
 */
class ScriptSchemaTranslatorFactory
{
public:

  static ScriptSchemaTranslatorFactory& getInstance();

  std::shared_ptr<ScriptSchemaTranslator> createTranslator(const QString& scriptPath);

  /**
   * This can be used to register translators, but using the Factory methods are preferred. See
   * JavaScriptSchemaTranslator for an example.
   */
  void registerScript(const QString& className) { _translators.push_back(className); }

private:

  void _init();

  std::vector<QString> _translators;

  /** Default constructor/destructor */
  ScriptSchemaTranslatorFactory() = default;
  ~ScriptSchemaTranslatorFactory() = default;
  /** Delete copy constructor and assignment operator */
  ScriptSchemaTranslatorFactory(const ScriptSchemaTranslatorFactory&) = delete;
  ScriptSchemaTranslatorFactory& operator=(const ScriptSchemaTranslatorFactory&) = delete;
};

}

#endif // SCRIPT_SCHEMA_TRANSLATOR_FACTORY_H
