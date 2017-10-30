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
#include "AddImplicitlyDerivedTagsPoiVisitor.h"

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

HOOT_FACTORY_REGISTER(ElementVisitor, AddImplicitlyDerivedTagsPoiVisitor)

AddImplicitlyDerivedTagsPoiVisitor::AddImplicitlyDerivedTagsPoiVisitor()
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(ConfigOptions().getPoiImplicitTagRulesDatabase());
}

AddImplicitlyDerivedTagsPoiVisitor::AddImplicitlyDerivedTagsPoiVisitor(const QString databasePath)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(databasePath);
}

AddImplicitlyDerivedTagsPoiVisitor::~AddImplicitlyDerivedTagsPoiVisitor()
{
  if (_ruleReader)
  {
    _ruleReader->close();
  }
}

bool caseInsensitiveLessThan(const QString s1, const QString s2)
{
  return s1.toLower() < s2.toLower();
}

void AddImplicitlyDerivedTagsPoiVisitor::visit(const ElementPtr& e)
{
  if (e->getElementType() == ElementType::Node &&
      // either the element isn't a poi, or it contains a generic poi type
      (!OsmSchema::getInstance().hasCategory(e->getTags(), "poi") ||
       e->getTags().contains("poi") || e->getTags().get("place") == QLatin1String("locality")))
  {
    //get names
    //query for implicit tags for the names; pass in a ref wordsInvolvedInMultipleRules param
    //if wordsInvolvedInMultipleRules is true, mark the feature
    //else if tags size > 0, add the tags
    //else get name tokens
    //name token logic follows above

    bool foundDuplicateMatch = false;
    Tags tagsToAdd;

    const QStringList names = e->getTags().getNames();
    LOG_VART(names);
    QSet<QString> matchingWords;
    bool wordsInvolvedInMultipleRules = false;
    //the name phrases take precendence over the tokenized names, so look for tags associated with
    //them first
    Tags implicitlyDerivedTags =
      _ruleReader->getImplicitTags(names.toSet(), matchingWords, wordsInvolvedInMultipleRules);
    LOG_VART(implicitlyDerivedTags);
    LOG_VART(matchingWords);
    LOG_VART(wordsInvolvedInMultipleRules)
    if (wordsInvolvedInMultipleRules)
    {
      LOG_TRACE(
        "Found duplicate match for names: " << names << " with matching words: " << matchingWords);
      foundDuplicateMatch = true;
    }
    else if (implicitlyDerivedTags.size() > 0)
    {
      LOG_TRACE(
        "Derived implicit tags for names: " << names << " with matching words: " << matchingWords);
      tagsToAdd = implicitlyDerivedTags;
    }
    else
    {
      //we didn't find any tags for the whole names, so let's look for them with the tokenized name
      //parts
      const QSet<QString> nameTokens = _getNameTokens(e->getTags());
      implicitlyDerivedTags =
        _ruleReader->getImplicitTags(nameTokens, matchingWords, wordsInvolvedInMultipleRules);
      if (wordsInvolvedInMultipleRules)
      {
        LOG_TRACE(
          "Found duplicate match for name tokens: " << nameTokens << " with matching words: " <<
          matchingWords);
        foundDuplicateMatch = true;
      }
      else if (implicitlyDerivedTags.size() > 0)
      {
        LOG_TRACE(
          "Derived implicit tags for name tokens: " << nameTokens << " with matching words: " <<
          matchingWords);
        tagsToAdd = implicitlyDerivedTags;
      }
    }

//    const QSet<QString> namesAndNameWords = _extractNamesAndNameWords(e->getTags());

//    QSet<QString> matchingRuleWords;
//    if (_ruleReader->wordsInvolveMultipleRules(namesAndNameWords, matchingRuleWords))
//    {
//      foundDuplicateMatch = true;
//      assert(matchingRuleWords.size() != 0);
//    }

//    QSet<QString> matchingWords;
//    tagsToAdd = _ruleReader->getImplicitTags(namesAndNameWords, matchingWords);

    if (foundDuplicateMatch)
    {
      QStringList matchingWordsList = matchingWords.toList();
      //matchingWordsList.sort();
      qSort(matchingWordsList.begin(), matchingWordsList.end(), caseInsensitiveLessThan);
      e->getTags().appendValue(
        "note",
        "Found multiple possible matches for implicit tags: " + matchingWordsList.join(", "));
    }
    else if (!tagsToAdd.isEmpty())
    {
      e->getTags().addTags(tagsToAdd);
      assert(matchingWords.size() != 0);
      QStringList matchingWordsList = matchingWords.toList();
      qSort(matchingWordsList.begin(), matchingWordsList.end(), caseInsensitiveLessThan);
      //matchingWordsList.sort();
      e->getTags().appendValue(
        "note", "Implicitly derived tags based on: " + matchingWordsList.join(", "));
    }
  }
}

QSet<QString> AddImplicitlyDerivedTagsPoiVisitor::_getNameTokens(const Tags& t)
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

//QSet<QString> AddImplicitlyDerivedTagsPoiVisitor::_extractNamesAndNameWords(const Tags& t)
//{
//  QSet<QString> result;

//  QStringList names = t.getNames();
//  StringTokenizer tokenizer;

//  foreach (const QString& n, names)
//  {
//    result.insert(n.toLower());
//    QStringList words = tokenizer.tokenize(n);
//    foreach (const QString& w, words)
//    {
//      result.insert(w.toLower());
//    }
//  }

//  return result;
//}

}
