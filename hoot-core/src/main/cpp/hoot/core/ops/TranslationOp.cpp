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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TranslationOp.h"

// hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/io/ScriptTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/TranslationVisitor.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, TranslationOp)

TranslationOp::TranslationOp()
{
  setConfiguration(conf());
}

void TranslationOp::apply(boost::shared_ptr<OsmMap>& map)
{
  if (_translator.get() == 0)
  {
    throw Exception("The translation script must be set before the op can be applied. See "
                    "the configuration key: " + ConfigOptions().getTranslationScriptKey());
  }

  TranslationVisitor v(*_translator, _toOgr, map.get());
  map->visitRw(v);
}

void TranslationOp::setConfiguration(const Settings& conf)
{
  ConfigOptions c(conf);
  if (conf.hasKey(c.getTranslationScriptKey()) && c.getTranslationScript() != "")
  {
    setPath(c.getTranslationScript());
  }
  QString dir = c.getTranslationDirection();
  if (dir == "toogr")
  {
    _toOgr = true;
  }
  else if (dir == "toosm")
  {
    _toOgr = false;
  }
  else
  {
    throw HootException("Expected a translation.direction of 'toogr' or 'toosm'.");
  }
}

void TranslationOp::setPath(QString path)
{
  _translator.reset(ScriptTranslatorFactory::getInstance().createTranslator(path));
}

}
