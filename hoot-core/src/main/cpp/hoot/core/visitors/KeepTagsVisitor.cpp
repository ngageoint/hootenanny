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
#include "KeepTagsVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, KeepTagsVisitor)

KeepTagsVisitor::KeepTagsVisitor()
{
  setConfiguration(conf());
}

KeepTagsVisitor::KeepTagsVisitor(QStringList keys) :
_keys(keys)
{
}

void KeepTagsVisitor::setConfiguration(const Settings& conf)
{
  _keys = ConfigOptions(conf).getKeepTagsVisitorKeys();
}

void KeepTagsVisitor::visit(const boost::shared_ptr<Element>& e)
{
  //get a copy of the tags for modifying
  Tags tags;
  tags.addTags(e->getTags());
  for (Tags::const_iterator it = e->getTags().begin(); it != e->getTags().end(); ++it)
  {
    if (!_keys.contains(it.key()))
    {
      tags.remove(it.key());
    }
  }
  e->setTags(tags);
}

}
