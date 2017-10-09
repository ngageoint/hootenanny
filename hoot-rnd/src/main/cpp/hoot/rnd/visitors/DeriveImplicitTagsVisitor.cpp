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
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QSet>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, DeriveImplicitTagsVisitor)

DeriveImplicitTagsVisitor::DeriveImplicitTagsVisitor()
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(ConfigOptions().getDeriveImplicitTagsDatabase());
}

DeriveImplicitTagsVisitor::DeriveImplicitTagsVisitor(const QString databasePath)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(databasePath);
}

DeriveImplicitTagsVisitor::~DeriveImplicitTagsVisitor()
{
  if (_ruleReader)
  {
    _ruleReader->close();
  }
}

void DeriveImplicitTagsVisitor::visit(const ElementPtr& e)
{
  bool foundDuplicateMatch = false;
  Tags tagsToAdd;
  if (e->getElementType() == ElementType::Node &&
      // either the element isn't a poi, or it contains a generic poi type
      (!OsmSchema::getInstance().hasCategory(e->getTags(), "poi") ||
       e->getTags().contains("poi") || e->getTags().get("place") == QLatin1String("locality")))
  {
    const QSet<QString> namesAndNameWords = _extractNamesAndNameWords(e->getTags());

    QSet<QString> matchingRuleWords;
    if (_ruleReader->wordsInvolveMultipleRules(namesAndNameWords, matchingRuleWords))
    {
      foundDuplicateMatch = true;
      assert(matchingRuleWords.size() != 0);
    }

    QSet<QString> matchingWords;
    tagsToAdd = _ruleReader->getImplicitTags(namesAndNameWords, matchingWords);

    if (foundDuplicateMatch)
    {
      QStringList matchingRuleWordsList = matchingRuleWords.toList();
      matchingRuleWordsList.sort();
      e->getTags().appendValue(
        "note",
        "Found multiple possible matches for implicit tags: " + matchingRuleWordsList.join(", "));
    }
    else if (!tagsToAdd.isEmpty())
    {
      e->getTags().addTags(tagsToAdd);
      assert(matchingWords.size() != 0);
      QStringList matchingWordsList = matchingWords.toList();
      matchingWordsList.sort();
      e->getTags().appendValue(
        "note", "Implicitly derived tags based on: " + matchingWordsList.join(", "));
    }
  }
}

QSet<QString> DeriveImplicitTagsVisitor::_extractNamesAndNameWords(const Tags& t)
{
  QSet<QString> result;

  QStringList names = t.getNames();
  StringTokenizer tokenizer;

  foreach (const QString& n, names)
  {
    result.insert(n.toLower());
    QStringList words = tokenizer.tokenize(n);
    foreach (const QString& w, words)
    {
      result.insert(w.toLower());
    }
  }

  return result;
}

}
