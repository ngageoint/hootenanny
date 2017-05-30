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
#include "TagRenameKeyVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, TagRenameKeyVisitor)

TagRenameKeyVisitor::TagRenameKeyVisitor()
{
}

TagRenameKeyVisitor::TagRenameKeyVisitor(const QString oldKey, const QString newKey) :
_oldKey(oldKey),
_newKey(newKey)
{
}

void TagRenameKeyVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _oldKey = configOptions.getTagRenameVisitorOldKey();
  _newKey = configOptions.getTagRenameVisitorNewKey();
  if (_oldKey.trimmed() == "" || _newKey.trimmed() == "")
  {
    throw HootException("Invalid configuration value(s) for the TagRenameKeyVisitor.");
  }
}

void TagRenameKeyVisitor::visit(const boost::shared_ptr<Element>& e)
{
  LOG_VART(_oldKey);
  LOG_VART(_newKey);

  if (e->getTags().contains(_oldKey))
  {
    Tags& tags = e->getTags();
    const QString value = tags.get(_oldKey);
    tags.remove(_oldKey);
    tags.insert(_newKey, value);
    e->setTags(tags);
  }
}

}
