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
#include "ScriptSchemaTranslatorFactory.h"

// hoot
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>

using namespace std;

namespace hoot
{

ScriptSchemaTranslatorFactory& ScriptSchemaTranslatorFactory::getInstance()
{
  //  Local static singleton instance
  static ScriptSchemaTranslatorFactory instance;
  return instance;
}

bool CompareSt(
  const std::shared_ptr<ScriptSchemaTranslator> st1,
  const std::shared_ptr<ScriptSchemaTranslator> st2)
{
  return st1->order() < st2->order();
}

std::shared_ptr<ScriptSchemaTranslator> ScriptSchemaTranslatorFactory::createTranslator(
  const QString& scriptPath)
{
  LOG_VARD(scriptPath);

  _init();

  vector<std::shared_ptr<ScriptSchemaTranslator>> st;
  for (size_t i = 0; i < _translators.size(); ++i)
  {
    LOG_VART(_translators[i]);
    st.push_back(Factory::getInstance().constructObject<ScriptSchemaTranslator>(_translators[i]));
  }
  sort(st.begin(), st.end(), CompareSt);

  std::shared_ptr<ScriptSchemaTranslator> result;
  for (size_t i = 0; i < st.size(); ++i)
  {
    try
    {
      st[i]->setScript(scriptPath);
      LOG_VART(st[i]->isValidScript());
      if (!result && st[i]->isValidScript())
      {
        result = st[i];
        LOG_TRACE("Found a valid translator: " + _translators[i]);
        break;
      }
      LOG_VART(result);
    }
    catch (...)
    {
      LOG_WARN("isValidScript shouldn't throw an exception.");
    }
  }

  if (!result)
  {
    throw HootException("Unable to find an appropriate scripting language for: " + scriptPath);
  }

  return result;
}

void ScriptSchemaTranslatorFactory::_init()
{
  if (_translators.empty())
  {
    _translators = Factory::getInstance().getObjectNamesByBase(ScriptSchemaTranslator::className());
  }
}

}

