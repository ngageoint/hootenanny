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
#include "TransliterateNameVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/Translator.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, TransliterateNameVisitor)

TransliterateNameVisitor::TransliterateNameVisitor()
{
}

bool TransliterateNameVisitor::isLatin(const QString& s)
{
  for (int i = 0; i < s.size(); i++)
  {
    // if the character isn't representable in latin1 Qt returns 0
    if (s[i].toLatin1() == 0)
    {
      return false;
    }
  }

  return true;
}

void TransliterateNameVisitor::visit(const boost::shared_ptr<Element>& e)
{
  QStringList names = e->getTags().getNames();

  for (int i = 0; i < names.size(); i++)
  {
    // we found a latin name, no need to do any more work.
    if (isLatin(names[i]))
    {
      return;
    }
  }

  if (names.size() > 0)
  {
    e->getTags().addNote("Transliterated Name: " + Translator::getInstance().toEnglish(names[0]));
  }
}


}
