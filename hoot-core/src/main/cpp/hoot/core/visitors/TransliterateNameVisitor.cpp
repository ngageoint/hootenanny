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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "TransliterateNameVisitor.h"

// hoot
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, TransliterateNameVisitor)

bool TransliterateNameVisitor::isLatin(const QString& s)
{
  for (const auto& character : s)
  {
    // if the character isn't representable in latin1 Qt returns 0
    if (character.toLatin1() == 0)
      return false;
  }
  return true;
}

void TransliterateNameVisitor::visit(const std::shared_ptr<Element>& e)
{
  QStringList names = e->getTags().getNames();

  for (const auto& name : qAsConst(names))
  {
    // we found a latin name, no need to do any more work.
    if (isLatin(name))
      return;
  }

  if (!names.empty())
  {
    // Should we be translating all of the names here?
    e->getTags().addNote("Transliterated Name: " + ToEnglishDictionaryTranslator().toEnglish(names[0]));
    _numAffected++;
  }
}

}
