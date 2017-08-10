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
#include "NamedOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/ops/VisitorOp.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, NamedOp)

NamedOp::NamedOp()
{
  _conf = 0;
}

void NamedOp::apply(boost::shared_ptr<OsmMap> &map)
{
  Factory& f = Factory::getInstance();

  foreach (QString s, _namedOps)
  {
    if (s.isEmpty())
    {
      // pass
    }
    else if (f.hasBase<OsmMapOperation>(s.toStdString()))
    {
      LOG_INFO("Applying operation: " << s);
      boost::shared_ptr<OsmMapOperation> t(Factory::getInstance().constructObject<OsmMapOperation>(s));

      Configurable* c = dynamic_cast<Configurable*>(t.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      if (!t->toString().trimmed().isEmpty())
      {
        LOG_DEBUG("Details: " << t->toString());
      }

      t->apply(map);

      LOG_VARD(map->getElementCount());
    }
    else if (f.hasBase<ConstElementVisitor>(s.toStdString()))
    {
      LOG_INFO("Applying visitor: " << s);
      boost::shared_ptr<ConstElementVisitor> t(Factory::getInstance().
        constructObject<ConstElementVisitor>(s));

      Configurable* c = dynamic_cast<Configurable*>(t.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      if (!t->toString().trimmed().isEmpty())
      {
        LOG_DEBUG("Details: " << t->toString());
      }

      boost::shared_ptr<OsmMapOperation> op(new VisitorOp(t));
      op->apply(map);

      LOG_VARD(map->getElementCount());
    }
    else if (f.hasBase<ElementVisitor>(s.toStdString()))
    {
      LOG_INFO("Applying visitor: " << s);
      boost::shared_ptr<ElementVisitor> t(Factory::getInstance().
        constructObject<ElementVisitor>(s));

      Configurable* c = dynamic_cast<Configurable*>(t.get());
      if (_conf != 0 && c != 0)
      {
        c->setConfiguration(*_conf);
      }

      if (!t->toString().trimmed().isEmpty())
      {
        LOG_DEBUG("Details: " << t->toString());
      }

      map->visitRw(*t);
    }
    else
    {
      throw HootException("Unexpected named operation: " + s);
    }
  }
}

void NamedOp::setConfiguration(const Settings& conf)
{
  _conf = &conf;
}

}
