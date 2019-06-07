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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ReplaceTagVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ReplaceTagVisitor)

ReplaceTagVisitor::ReplaceTagVisitor():
  _matchKey(""),
  _matchValue(""),
  _replaceKey(""),
  _replaceValue("")
{
  setConfiguration(conf());
}

ReplaceTagVisitor::ReplaceTagVisitor(QString matchKey, QString matchValue,
                                     QString replaceKey, QString replaceValue):
  _matchKey(matchKey),
  _matchValue(matchValue),
  _replaceKey(replaceKey),
  _replaceValue(replaceValue)
{
}

void ReplaceTagVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);

  const QString matchTag = configOptions.getReplaceTagVisitorMatchTag();
  const QString replaceTag = configOptions.getReplaceTagVisitorReplaceTag();
  if (!matchTag.trimmed().isEmpty() && !replaceTag.trimmed().isEmpty())
  {
    if (!matchTag.contains("="))
    {
      throw IllegalArgumentException("Invalid match tag: " + matchTag);
    }
    const QStringList matchTagParts = matchTag.split("=");
    if (matchTagParts.size() != 2)
    {
      throw IllegalArgumentException("Invalid match tag: " + matchTag);
    }
    _matchKey = matchTagParts[0];
    _matchValue = matchTagParts[1];

    if (!replaceTag.contains("="))
    {
      throw IllegalArgumentException("Invalid replace tag: " + replaceTag);
    }
    const QStringList replaceTagParts = replaceTag.split("=");
    if (replaceTagParts.size() != 2)
    {
      throw IllegalArgumentException("Invalid replace tag: " + replaceTag);
    }
    _replaceKey = replaceTagParts[0];
    _replaceValue = replaceTagParts[1];
  }
}

void ReplaceTagVisitor::setMatchTag(QString k, QString v)
{
  _matchKey = k;
  _matchValue = v;
}

void ReplaceTagVisitor::setReplaceTag(QString k, QString v)
{
  _replaceKey = k;
  _replaceValue = v;
}

void ReplaceTagVisitor::visit(const std::shared_ptr<Element>& e)
{
  // Key and Value must match exactly. Then we replace.
  Tags::iterator it = e->getTags().find(_matchKey);
  if (it != e->getTags().end() && (0 == it.value().compare(_matchValue)))
  {
    e->getTags().remove(_matchKey);
    e->getTags().appendValue(_replaceKey, _replaceValue);
    _numAffected++;
  }
}

}
