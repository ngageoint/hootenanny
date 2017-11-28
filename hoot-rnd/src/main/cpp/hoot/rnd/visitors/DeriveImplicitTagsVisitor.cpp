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
  _rules.back()->word += QString::fromUtf8("mosque"); //no religion; 7.5k
  _rules.back()->word += QString::fromUtf8("msjd");  //no religion; 1.4k
  _rules.back()->word += QString::fromUtf8("masjid"); //no religion; 11k
  _rules.back()->word += QString::fromUtf8("Jāmi"); //no religion; 13
  _rules.back()->tags["amenity"] = "place_of_worship";
  _rules.back()->tags["religion"] = "muslim";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("alshyt"); //900
  _rules.back()->word += QString::fromUtf8("whdt"); //400
  _rules.back()->word += QString::fromUtf8("alshy"); //300
  _rules.back()->word += QString::fromUtf8("almrkz"); //200
  _rules.back()->word += QString::fromUtf8("shyt"); //200
  _rules.back()->word += QString::fromUtf8("bny"); //100
  _rules.back()->word += QString::fromUtf8("Sihhi"); //500
  _rules.back()->word += QString::fromUtf8("Şiḩḩī"); // X
  _rules.back()->word += QString::fromUtf8("alwhdt"); //900
  _rules.back()->tags["amenity"] = "clinic";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("Şiḩḩīy"); // X
  _rules.back()->word += QString::fromUtf8("Mustashfa"); //500
  _rules.back()->tags["amenity"] = "hospital";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("Shurţat"); //500
  _rules.back()->word += QString::fromUtf8("Police"); //16k
  _rules.back()->tags["amenity"] = "police";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("school"); //473k
  _rules.back()->tags["amenity"] = "school";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("embassy"); //3k
  _rules.back()->word += QString::fromUtf8("ﺲﻓﺍﺭﺓ"); // X
  _rules.back()->tags["amenity"] = "embassy";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("petroleumeum"); //28
  _rules.back()->tags["amenity"] = "fuel";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("bank"); //51k
  _rules.back()->tags["amenity"] = "bank";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("hotel"); //122k
  _rules.back()->word += QString::fromUtf8("funduq"); //1k
  _rules.back()->word += QString::fromUtf8("ﻢﻧﺎﻣﺓ"); //X
  _rules.back()->tags["tourism"] = "hotel";

  _rules.append(ImplicitRulePtr(new ImplicitRule()));
  _rules.back()->word += QString::fromUtf8("museum"); //17k
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
