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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TranslationOp.h"

// hoot
#include <hoot/core/Exception.h>
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/io/ScriptTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, TranslationOp)

class TranslationVisitor : public ElementVisitor
{
public:

  TranslationVisitor(ScriptTranslator& t, bool toOgr, const OsmMapPtr& map) : _t(t), _toOgr(toOgr), _map(map)
  {
    _circularErrorKey = "error:circular";
    _accuracyKey = "accuracy";

    if (toOgr)
    {
      _togr = dynamic_cast<ScriptToOgrTranslator*>(&t);
      if (_togr == 0)
      {
        throw HootException("Translating to OGR requires a script that supports to OGR "
          "translations.");
      }
    }
  }

  virtual void visit(ElementType type, long id)
  {
    visit(_map->getElement(type, id));
  }

  virtual void visit(const shared_ptr<Element>& e)
  {
    if (e->getTags().getNonDebugCount() > 0)
    {
      if (_toOgr)
      {
        GeometryTypeId gtype = ElementConverter::getGeometryType(e);

        vector<Tags> allTags = _togr->translateToOgrTags(e->getTags(), e->getElementType(), gtype);

        if (allTags.size() > 0)
        {
          if (allTags.size() > 1)
          {
            LOG_WARN("More than one feature was returned, only keeping the first feature.");
          }

          e->setTags(allTags[0]);
        }
      }
      else
      {
        _t.translateToOsm(e->getTags(), "");

        if (e->getTags().contains(_circularErrorKey))
        {
          e->setCircularError(e->getTags().getDouble(_circularErrorKey));
          e->getTags().remove(_circularErrorKey);
          e->getTags().remove(_accuracyKey);
        }
        else if (e->getTags().contains(_accuracyKey))
        {
          e->setCircularError(e->getTags().getDouble(_accuracyKey));
          e->getTags().remove(_circularErrorKey);
          e->getTags().remove(_accuracyKey);
        }
      }
    }
  }

private:
  ScriptTranslator& _t;
  ScriptToOgrTranslator* _togr;
  QString _circularErrorKey;
  QString _accuracyKey;
  bool _toOgr;
  const OsmMapPtr& _map;
};

TranslationOp::TranslationOp()
{
  setConfiguration(conf());
}

void TranslationOp::apply(shared_ptr<OsmMap>& map)
{
  if (_translator.get() == 0)
  {
    throw Exception("The translation script must be set before the op can be applied. See "
                    "the configuration key: " + ConfigOptions().getTranslationScriptKey());
  }

  TranslationVisitor v(*_translator, _toOgr, map);
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
