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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SetTagVisitor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, SetTagVisitor)

SetTagVisitor::SetTagVisitor()
{
}

SetTagVisitor::SetTagVisitor(QString key, QString value) :
_k(key),
_v(value)
{
}

void SetTagVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _k = configOptions.getSetTagVisitorKey();
  _v = configOptions.getSetTagVisitorValue();
}

void SetTagVisitor::visit(const shared_ptr<Element>& e)
{
  if (_k.isEmpty())
  {
    throw IllegalArgumentException("You must set the key in the SetTagVisitor class.");
  }
  if (_k == MetadataTags::ErrorCircular())
  {
    bool ok;
    double v = _v.toDouble(&ok);
    if (!ok)
    {
      throw IllegalArgumentException(MetadataTags::ErrorCircular() + " expects a double value.");
    }
    e->setCircularError(v);
  }
  else
  {
    e->getTags()[_k] = _v;
  }
}

}
