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
#include "DeriveImplicitTagsVisitor.h"

#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>

// Qt
#include <QSet>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, DeriveImplicitTagsVisitor)

class ImplicitRule
{
public:
  QStringList word;
  QStringList wholeName;
  Tags tags;
};

typedef boost::shared_ptr<ImplicitRule> ImplicitRulePtr;

DeriveImplicitTagsVisitor::DeriveImplicitTagsVisitor()
{
  // an incomplete list derived over Yemen/Egypt for testing.

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("mosque");
  _rules.back()->word += QString::fromUtf8("msjd");
  _rules.back()->word += QString::fromUtf8("masjid");
  _rules.back()->word += QString::fromUtf8("Jāmi");
  _rules.back()->tags["amenity"] = "place_of_worship";
  _rules.back()->tags["religion"] = "muslim";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("alshyt");
  _rules.back()->word += QString::fromUtf8("whdt");
  _rules.back()->word += QString::fromUtf8("alshy");
  _rules.back()->word += QString::fromUtf8("almrkz");
  _rules.back()->word += QString::fromUtf8("shyt");
  _rules.back()->word += QString::fromUtf8("bny");
  _rules.back()->word += QString::fromUtf8("Sihhi");
  _rules.back()->word += QString::fromUtf8("Şiḩḩī");
  _rules.back()->word += QString::fromUtf8("alwhdt");
  _rules.back()->tags["amenity"] = "clinic";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("Şiḩḩīy");
  _rules.back()->word += QString::fromUtf8("Mustashfa");
  _rules.back()->tags["amenity"] = "hospital";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("Shurţat");
  _rules.back()->word += QString::fromUtf8("Police");
  _rules.back()->tags["amenity"] = "police";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("school");
  _rules.back()->tags["amenity"] = "school";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("embassy");
  _rules.back()->word += QString::fromUtf8("ﺲﻓﺍﺭﺓ");
  _rules.back()->tags["amenity"] = "embassy";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("petroleumeum");
  _rules.back()->tags["amenity"] = "fuel";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("bank");
  _rules.back()->tags["amenity"] = "bank";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("hotel");
  _rules.back()->word += QString::fromUtf8("funduq");
  _rules.back()->word += QString::fromUtf8("ﻢﻧﺎﻣﺓ");
  _rules.back()->tags["tourism"] = "hotel";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("museum");
  _rules.back()->tags["tourism"] = "museum";

  _rulesToLower();
}

QSet<QString> DeriveImplicitTagsVisitor::_extractNameWords(const Tags& t)
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

void DeriveImplicitTagsVisitor::_rulesToLower()
{
  foreach (ImplicitRulePtr r, _rules)
  {
    for (int i = 0; i < r->word.size(); i++)
    {
      r->word[i] = r->word[i].toLower();
    }
  }
}

void DeriveImplicitTagsVisitor::visit(const ElementPtr& e)
{
  // make sure the element is a node
  if (e->getElementType() == ElementType::Node &&
      // either the element isn't a poi, or it contains a generic poi type
      (OsmSchema::getInstance().hasCategory(e->getTags(), "poi") == false ||
      e->getTags().contains("poi") || e->getTags().get("place") == "locality"))
  {
    QSet<QString> nameWords = _extractNameWords(e->getTags());

    QStringList matchWords;
    ImplicitRulePtr match;
    // did we find two matches?
    bool duplicateMatch = false;
    foreach (ImplicitRulePtr r, _rules)
    {
      for (int i = 0; i < r->word.size(); ++i)
      {
        if (nameWords.contains(r->word[i]))
        {
          matchWords.append(r->word[i]);
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
      e->getTags().addTags(match->tags);
      e->getTags().appendValue("note", "Implicitly derived tags based on: " +
                               matchWords.join(", "));
    }
  }

}

}
