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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DeriveImplicitTagRulesVisitor.h"

#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>

// Qt
#include <QSet>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, DeriveImplicitTagRulesVisitor)

DeriveImplicitTagRulesVisitor::DeriveImplicitTagRulesVisitor()
{
  // an incomplete list derived over Yemen/Egypt for testing.

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("mosque"));
  _rules.back()->getWords().append(QString::fromUtf8("msjd"));
  _rules.back()->getWords().append(QString::fromUtf8("masjid"));
  _rules.back()->getWords().append(QString::fromUtf8("Jāmi"));
  _rules.back()->getTags().appendValue("amenity=place_of_worship");
  _rules.back()->getTags().appendValue("religion=muslim");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("alshyt"));
  _rules.back()->getWords().append(QString::fromUtf8("whdt"));
  _rules.back()->getWords().append(QString::fromUtf8("alshy"));
  _rules.back()->getWords().append(QString::fromUtf8("almrkz"));
  _rules.back()->getWords().append(QString::fromUtf8("shyt"));
  _rules.back()->getWords().append(QString::fromUtf8("bny"));
  _rules.back()->getWords().append(QString::fromUtf8("Sihhi"));
  _rules.back()->getWords().append(QString::fromUtf8("Şiḩḩī"));
  _rules.back()->getWords().append(QString::fromUtf8("alwhdt"));
  _rules.back()->getTags().appendValue("amenity=clinic");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("Şiḩḩīy"));
  _rules.back()->getWords().append(QString::fromUtf8("Mustashfa"));
  _rules.back()->getTags().appendValue("amenity=hospital");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("Shurţat"));
  _rules.back()->getWords().append(QString::fromUtf8("Police"));
  _rules.back()->getTags().appendValue("amenity=police");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("school"));
  _rules.back()->getTags().appendValue("amenity=school");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("embassy"));
  _rules.back()->getWords().append(QString::fromUtf8("ﺲﻓﺍﺭﺓ"));
  _rules.back()->getTags().appendValue("amenity=embassy");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("petroleumeum"));
  _rules.back()->getTags().appendValue("amenity=fuel");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("bank"));
  _rules.back()->getTags().appendValue("amenity=bank");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("hotel"));
  _rules.back()->getWords().append(QString::fromUtf8("funduq"));
  _rules.back()->getWords().append(QString::fromUtf8("ﻢﻧﺎﻣﺓ"));
  _rules.back()->getTags().appendValue("tourism=hotel");

  _rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
  _rules.back()->getWords().append(QString::fromUtf8("museum"));
  _rules.back()->getTags().appendValue("tourism=museum");

  _rulesToLower();
}

QSet<QString> DeriveImplicitTagRulesVisitor::_extractNameWords(const Tags& t)
{
  QSet<QString> result;
  QStringList names = t.getNames();
  StringTokenizer tokenizer;

  foreach (const QString& n, names)
  {
    QStringList words = tokenizer.tokenize(n);
    foreach (const QString& w, words)
    {
      result.insert(w.toLower());
    }
  }

  return result;
}

void DeriveImplicitTagRulesVisitor::_rulesToLower()
{
  foreach (ImplicitTagRulePtr r, _rules)
  {
    for (int i = 0; i < r->getWords().size(); i++)
    {
      r->getWords()[i] = r->getWords()[i].toLower();
    }
  }
}

void DeriveImplicitTagRulesVisitor::visit(const ElementPtr& e)
{
  // make sure the element is a node
  if (e->getElementType() == ElementType::Node &&
      // either the element isn't a poi, or it contains a generic poi type
      (OsmSchema::getInstance().hasCategory(e->getTags(), "poi") == false ||
      e->getTags().contains("poi") || e->getTags().get("place") == "locality"))
  {
    QSet<QString> nameWords = _extractNameWords(e->getTags());

    QStringList matchWords;
    ImplicitTagRulePtr match;
    // did we find two matches?
    bool duplicateMatch = false;
    foreach (ImplicitTagRulePtr r, _rules)
    {
      for (int i = 0; i < r->getWords().size(); ++i)
      {
        const QString word = r->getWords()[i];
        if (nameWords.contains(word))
        {
          matchWords.append(word);
          if (match.get() == 0 || match == r)
          {
            match = r;
          }
          else
          {
            duplicateMatch = true;
          }
        }
      }
    }

    if (duplicateMatch)
    {
      e->getTags().appendValue("note", "Found multiple possible matches for implicit tags: " +
                               matchWords.join(", "));
    }
    else if (match.get() != 0)
    {
      e->getTags().addTags(match->getTags());
      e->getTags().appendValue("note", "Implicitly derived tags based on: " +
                               matchWords.join(", "));
    }
  }
}

}
