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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ImplicitTypeTaggerBase.h"

#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/schema/ImplicitTagUtils.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchema.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QSet>
#include <QStringBuilder>

namespace hoot
{

ImplicitTypeTaggerBase::ImplicitTypeTaggerBase() :
_allowTaggingSpecificFeatures(true),
_elementIsASpecificFeature(false),
_numFeaturesModified(0),
_numTagsAdded(0),
_numFeaturesInvolvedInMultipleRules(0),
_numFeaturesParsed(0),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_smallestNumberOfTagsAdded(LONG_MAX),
_largestNumberOfTagsAdded(0),
_translateNamesToEnglish(true),
_matchEndOfNameSingleTokenFirst(true),
_additionalNameKeys(ConfigOptions().getImplicitTaggerAdditionalNameKeys()),
_maxNameLength(ConfigOptions().getImplicitTaggerMaxNameLength())
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(ConfigOptions().getImplicitTaggerRulesDatabase());
}

ImplicitTypeTaggerBase::ImplicitTypeTaggerBase(const QString& databasePath) :
_allowTaggingSpecificFeatures(true),
_elementIsASpecificFeature(false),
_numFeaturesModified(0),
_numTagsAdded(0),
_numFeaturesInvolvedInMultipleRules(0),
_numFeaturesParsed(0),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_smallestNumberOfTagsAdded(LONG_MAX),
_largestNumberOfTagsAdded(0),
_translateNamesToEnglish(true),
_matchEndOfNameSingleTokenFirst(true),
_additionalNameKeys(ConfigOptions().getImplicitTaggerAdditionalNameKeys()),
_maxNameLength(ConfigOptions().getImplicitTaggerMaxNameLength())
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(databasePath);
}

ImplicitTypeTaggerBase::~ImplicitTypeTaggerBase()
{
  if (_ruleReader)
  {
    LOG_VARD(_ruleReader->getTagsCacheSize());
    LOG_VARD(_ruleReader->getFirstRoundTagsCacheHits());
    LOG_VARD(_ruleReader->getSecondRoundTagsCacheHits());
    _ruleReader->close();
  }

  LOG_INFO(
    "Added " << StringUtils::formatLargeNumber(_numTagsAdded) << " tags to " <<
    StringUtils::formatLargeNumber(_numFeaturesModified) << " / " <<
    StringUtils::formatLargeNumber(_numFeaturesParsed)  << " total features.");
  LOG_INFO(
    StringUtils::formatLargeNumber(_numFeaturesInvolvedInMultipleRules) <<
    " features were involved in multiple tag rules and were not modified.");
  if (_numTagsAdded > 0 && _numFeaturesModified > 0)
  {
    long avgTagsAdded = (long)(_numTagsAdded / _numFeaturesModified);
    LOG_INFO(
      "Average tags added per feature: " << StringUtils::formatLargeNumber(avgTagsAdded));
    LOG_INFO(
      "Smallest number of tags added to a feature: " <<
       StringUtils::formatLargeNumber(_smallestNumberOfTagsAdded));
    LOG_INFO(
      "Largest number of tags added to a feature: " <<
       StringUtils::formatLargeNumber(_largestNumberOfTagsAdded));
  }
}

void ImplicitTypeTaggerBase::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);

  setTranslateNamesToEnglish(confOptions.getImplicitTaggingTranslateNamesToEnglish());
  setMatchEndOfNameSingleTokenFirst(confOptions.getImplicitTaggerMatchEndOfNameSingleTokenFirst());
  setAllowTaggingSpecificFeatures(confOptions.getImplicitTaggerAllowTaggingSpecificEntities());

  _ruleReader->setAddTopTagOnly(confOptions.getImplicitTaggerAddTopTagOnly());
  _ruleReader->setAllowWordsInvolvedInMultipleRules(
    confOptions.getImplicitTaggerAllowWordsInvolvedInMultipleRules());

  if (_translateNamesToEnglish)
  {
    _translator.reset(
      Factory::getInstance().constructObject<ToEnglishTranslator>(
        confOptions.getLanguageTranslationTranslator()));
    _translator->setConfiguration(conf);
    _translator->setSourceLanguages(confOptions.getLanguageTranslationSourceLanguages());
    _translator->setId("ImplicitTypeTaggerBase");
  }
}

QStringList ImplicitTypeTaggerBase::_getNames(const Tags& tags) const
{
  QStringList namesToReturn;
  QStringList names = tags.getNames();
  for (int i = 0; i < _additionalNameKeys.size(); i++)
  {
    const QString key = _additionalNameKeys.at(i);
    LOG_VART(key);
    names.append(tags.get(key));
  }
  LOG_VART(names);
  for (int i = 0; i < names.size(); i++)
  {
    const QString name = names.at(i);
    if (name.size() <= _maxNameLength)
    {
      namesToReturn.append(name);
    }
  }
  LOG_VART(namesToReturn);
  return namesToReturn;
}

bool caseInsensitiveLessThan(const QString& s1, const QString& s2)
{
  return s1.toLower() < s2.toLower();
}

void ImplicitTypeTaggerBase::visit(const ElementPtr& e)
{
  if (_translateNamesToEnglish && !_translator.get())
  {
    throw HootException("To English translation enabled but no translator was specified.");
  }

  if (_visitElement(e))
  {
    bool foundDuplicateMatch = false;
    Tags tagsToAdd;

    // TODO: this is temp for testing until it gets fixed with a schema translation
    if (e->getTags().get("gnis:feature_type").toLower() == "mine")
    {
      LOG_TRACE("Using custom tagging rule...");
      e->getTags().appendValue("man_made", "mine");
      QString tagValue =
        "Added 1 implicitly derived tag(s) based on: gnis:feature_type=mine; tags added: man_made=mine";
      LOG_VART(tagValue);
      e->getTags().appendValue("hoot:implicitTags:tagsAdded", tagValue);
    }

    QStringList filteredNames = _cleanNames(e->getTags());

    if (filteredNames.size() > 0)
    {
      Tags implicitlyDerivedTags;
      QSet<QString> matchingWords;
      bool wordsInvolvedInMultipleRules = false;

      if (implicitlyDerivedTags.size() == 0)
      {
        //the complete name phrases take precendence over the tokenized names, so look for tags
        //associated with them first
        implicitlyDerivedTags =
          _ruleReader->getImplicitTags(
            filteredNames.toSet(), matchingWords, wordsInvolvedInMultipleRules);
      }
      LOG_VART(implicitlyDerivedTags);
      LOG_VART(matchingWords);
      LOG_VART(wordsInvolvedInMultipleRules);

      bool namesContainBuilding = false;
      bool namesContainOffice = false;

      if (wordsInvolvedInMultipleRules)
      {
        LOG_TRACE(
          "Found duplicate match for names: " << filteredNames << " with matching words: " <<
          matchingWords);
        foundDuplicateMatch = true;
      }
      else if (implicitlyDerivedTags.size() > 0)
      {
        LOG_TRACE(
          "Derived implicit tags for names: " << filteredNames << " with matching words: " <<
          matchingWords);
        tagsToAdd = implicitlyDerivedTags;
      }
      else
      {
        //we didn't find any tags for the whole names, so let's look for them with the tokenized
        //name parts
        QStringList nameTokensList = _getNameTokens(filteredNames);

        LOG_VART(nameTokensList);
        LOG_VART(implicitlyDerivedTags.size());
        LOG_VART(nameTokensList.size());

        //only going up to token group size = 2, as larger sizes weren't found experimentally to
        //yield any better results
        if (implicitlyDerivedTags.size() == 0 && nameTokensList.size() > 2)
        {
          _getImplicitlyDerivedTagsFromMultipleNameTokens(
            filteredNames, nameTokensList, e->getTags(), implicitlyDerivedTags, matchingWords,
            wordsInvolvedInMultipleRules);
        }

        if (implicitlyDerivedTags.size() == 0)
        {
          //didn't find any matches with the token groups, so let's try with single tokens
          _getImplicitlyDerivedTagsFromSingleNameTokens(
            filteredNames, nameTokensList, e->getTags(), implicitlyDerivedTags, matchingWords,
            wordsInvolvedInMultipleRules, namesContainBuilding, namesContainOffice);
        }
        LOG_VART(implicitlyDerivedTags);
        LOG_VART(matchingWords);
        LOG_VART(wordsInvolvedInMultipleRules);

        if (wordsInvolvedInMultipleRules)
        {
          LOG_TRACE(
            "Found duplicate match for name tokens: " << nameTokensList << " with matching words: " <<
            matchingWords);
          foundDuplicateMatch = true;
        }
        else if (implicitlyDerivedTags.size() > 0)
        {
          LOG_TRACE(
            "Derived implicit tags for name tokens: " << nameTokensList << " with matching words: " <<
            matchingWords);
          tagsToAdd = implicitlyDerivedTags;
        }
      }
      LOG_VART(tagsToAdd);

      LOG_VART(foundDuplicateMatch);
      if (foundDuplicateMatch)
      {
        _updateElementForDuplicateMatch(e, matchingWords);
      }
      else if (!tagsToAdd.isEmpty())
      {
        assert(!matchingWords.isEmpty());
        LOG_VART(matchingWords);

        _ensureCorrectTagSpecificity(e, tagsToAdd);

        //This is a little kludgy, but we'll leave it for now since it helps.
        if (tagsToAdd.isEmpty() && !_elementIsASpecificFeature && namesContainOffice)
        {
          tagsToAdd.appendValue("building", "office");
          matchingWords.insert("office");
        }
        else if (tagsToAdd.isEmpty() && !_elementIsASpecificFeature && namesContainBuilding)
        {
          tagsToAdd.appendValue("building", "yes");
          matchingWords.insert("building");
        }
        LOG_VART(tagsToAdd);

        //bit of hack...to handle a situation where features have been incorrectly tagged;
        //the argument could be made to just correct the input data instead
        if (tagsToAdd.isEmpty())
        {
          tagsToAdd = _applyCustomRules(e, filteredNames);
        }

        if (!tagsToAdd.isEmpty())
        {
          _addImplicitTags(e, tagsToAdd, matchingWords);
        }
      } 
    }

    _numFeaturesParsed++;
    if (_numFeaturesParsed % (_statusUpdateInterval / 10) == 0)
    {
      PROGRESS_INFO(
        "Parsed " << StringUtils::formatLargeNumber(_numFeaturesParsed) <<
        " features from input for type tagging.");
    }
  }
}

Tags ImplicitTypeTaggerBase::_applyCustomRules(const ElementPtr& e, const QStringList& filteredNames)
{
  Tags tagsToAdd;

  if (PoiPolygonSchema::isPark(e))
  {
    for (int i = 0; i < filteredNames.size(); i++)
    {
      const QString name = filteredNames.at(i).toLower();
      LOG_VART(name);
      if (name.endsWith("play area") || name.endsWith("play areas") || name.endsWith("playground"))
      {
        LOG_TRACE("Using custom tagging rule...");
        tagsToAdd.appendValue("leisure", "playground");
        break;
      }
      else if (name.endsWith("golf course"))
      {
        LOG_TRACE("Using custom tagging rule...");
        tagsToAdd.appendValue("leisure", "golf_course");
        break;
      }
    }
  }

  return tagsToAdd;
}

QStringList ImplicitTypeTaggerBase::_cleanNames(Tags& tags)
{
  //the normal hoot convention is to split the name tag on ';' into multiple names; bypassing that
  //here, as it seems to cause more harm to implicit tagging than good
  QString name = tags.get("name");
  if (name.contains(";"))
  {
    tags.set("name", name.replace(";", "").trimmed());
  }

  QStringList names = _getNames(tags);
  LOG_VART(names);
  if (names.removeAll("old_name") > 0)
  {
    LOG_VART("Removed old name tag.");
  }
  if (names.removeAll("former_name") > 0)
  {
    LOG_VART("Removed former name tag.");
  }
  if (_translateNamesToEnglish)
  {
    names = ImplicitTagUtils::translateNamesToEnglish(names, tags, _translator);
  }
  LOG_VART(names);
  QStringList filteredNames;
  for (int i = 0; i < names.size(); i++)
  {
    QString name = names.at(i);
    ImplicitTagUtils::cleanName(name);
    if (!name.isEmpty())
    {
      filteredNames.append(name.toLower());
    }
  }
  LOG_VART(filteredNames);
  return filteredNames;
}

QString ImplicitTypeTaggerBase::_getEndOfNameToken(const QString& name,
                                                   const QStringList& nameTokensList) const
{
  for (int i = 0; i < nameTokensList.size(); i++)
  {
    const QString nameToken = nameTokensList.at(i);
    if (name.endsWith(nameToken))
    {
      return nameToken;
    }
  }
  return "";
}

void ImplicitTypeTaggerBase::_getImplicitlyDerivedTagsFromMultipleNameTokens(const QStringList& names, const QStringList& nameTokensList, const Tags& elementTags,
  Tags& implicitlyDerivedTags, QSet<QString>& matchingWords, bool& wordsInvolvedInMultipleRules)
{
  // This method needs cleanup

  LOG_TRACE("Attempting match with token group size of 2...");

  QStringList nameTokensListGroupSizeTwo;
  for (int i = 0; i < nameTokensList.size() - 1; i++)
  {
    QString nameToken = nameTokensList.at(i) + " " + nameTokensList.at(i + 1);
    if (_translateNamesToEnglish)
    {
      // Can this be combined with the ImplicitTagUtils translate method?
      const QString englishNameToken = _translator->translate(nameToken);
      LOG_VART(englishNameToken);
      if (!englishNameToken.isEmpty())
      {
        nameToken = englishNameToken;
      }
    }
    nameTokensListGroupSizeTwo.append(nameToken);
  }
  LOG_VART(nameTokensListGroupSizeTwo);

  if (_matchEndOfNameSingleTokenFirst)
  {
    //match the end of the name with an implicit tag rule before matching anything else in the name

    QString endOfNameToken =
      _getEndOfNameToken(elementTags.get("name:en"), nameTokensListGroupSizeTwo);
    if (endOfNameToken.isEmpty())
    {
      endOfNameToken = _getEndOfNameToken(elementTags.get("name"), nameTokensListGroupSizeTwo);
    }
    if (endOfNameToken.isEmpty())
    {
      for (int i = 0; i < names.size(); i++)
      {
        endOfNameToken = _getEndOfNameToken(names.at(i), nameTokensListGroupSizeTwo);
        if (!endOfNameToken.isEmpty())
        {
          break;
        }
      }
    }
    if (!endOfNameToken.isEmpty())
    {
      QStringList tempTokenList;
      tempTokenList.append(endOfNameToken);
      implicitlyDerivedTags =
        _ruleReader->getImplicitTags(
          tempTokenList.toSet(), matchingWords, wordsInvolvedInMultipleRules);
      if (implicitlyDerivedTags.size() == 0)
      {
        //end of name token didn't match; do token matching
        implicitlyDerivedTags =
          _ruleReader->getImplicitTags(
            nameTokensListGroupSizeTwo.toSet(), matchingWords, wordsInvolvedInMultipleRules);
      }
    }
    else
    {
      //end of name token invalid
      implicitlyDerivedTags =
        _ruleReader->getImplicitTags(
          nameTokensListGroupSizeTwo.toSet(), matchingWords, wordsInvolvedInMultipleRules);
    }
  }
  else
  {
    //skip end of name token matching
    implicitlyDerivedTags =
      _ruleReader->getImplicitTags(
        nameTokensListGroupSizeTwo.toSet(), matchingWords, wordsInvolvedInMultipleRules);
  }

  LOG_VART(implicitlyDerivedTags);
  LOG_VART(matchingWords);
  LOG_VART(wordsInvolvedInMultipleRules);
}

void ImplicitTypeTaggerBase::_getImplicitlyDerivedTagsFromSingleNameTokens(const QStringList& names, QStringList& nameTokensList, const Tags& elementTags,
  Tags& implicitlyDerivedTags, QSet<QString>& matchingWords, bool& wordsInvolvedInMultipleRules,
  bool& namesContainBuilding, bool& namesContainOffice)
{
  // Should be possible to combine this logic with
  //_getImplicitlyDerivedTagsFromMultipleNameTokens into a single method

  LOG_TRACE("Attempting match with token group size of 1...");

  if (_translateNamesToEnglish)
  {
    QStringList translatedNameTokens;
    for (int i = 0; i < nameTokensList.size(); i++)
    {
      const QString word = nameTokensList.at(i);
      LOG_VART(word);
      // Can this be combined with the ImplicitTagUtils translate method?
      const QString englishNameToken = _translator->translate(word);
      LOG_VART(englishNameToken);
      if (!englishNameToken.isEmpty())
      {
        translatedNameTokens.append(englishNameToken);
      }
      else
      {
        translatedNameTokens.append(word);
      }
    }
    nameTokensList = translatedNameTokens;
  }
  LOG_VART(nameTokensList);

  //This logic is kind of one-off but did help a little bit with reducing false positives with
  //offices and buildings...probably need something cleaner and more extensible.
  namesContainBuilding = false;
  if (nameTokensList.contains("building") || nameTokensList.contains("buildings"))
  {
    namesContainBuilding = true;
    nameTokensList.removeAll("building");
    nameTokensList.removeAll("buildings");
  }
  namesContainOffice = false;
  if (nameTokensList.contains("office") || nameTokensList.contains("offices"))
  {
    namesContainOffice = true;
    nameTokensList.removeAll("office");
    nameTokensList.removeAll("offices");
  }
  LOG_VART(nameTokensList);

  if (implicitlyDerivedTags.size() == 0 && nameTokensList.size() > 0)
  {
    if (_matchEndOfNameSingleTokenFirst)
    {
      //match the end of the name with an implicit tag rule before matching anything else in the
      //name

      QString endOfNameToken =
        _getEndOfNameToken(elementTags.get("name:en"), nameTokensList);
      if (endOfNameToken.isEmpty())
      {
        endOfNameToken = _getEndOfNameToken(elementTags.get("name"), nameTokensList);
      }
      if (endOfNameToken.isEmpty())
      {
        for (int i = 0; i < names.size(); i++)
        {
          endOfNameToken = _getEndOfNameToken(names.at(i), nameTokensList);
          if (!endOfNameToken.isEmpty())
          {
            break;
          }
        }
      }
      LOG_VART(endOfNameToken)
      if (!endOfNameToken.isEmpty())
      {
        QStringList tempTokenList;
        tempTokenList.append(endOfNameToken);
        implicitlyDerivedTags =
          _ruleReader->getImplicitTags(
            tempTokenList.toSet(), matchingWords, wordsInvolvedInMultipleRules);       
        if (implicitlyDerivedTags.size() == 0)
        {
          //end of name token didn't match; do token matching
          implicitlyDerivedTags =
            _ruleReader->getImplicitTags(
              nameTokensList.toSet(), matchingWords, wordsInvolvedInMultipleRules);
        }
      }
      else
      {
        //end of name token invalid
        implicitlyDerivedTags =
          _ruleReader->getImplicitTags(
            nameTokensList.toSet(), matchingWords, wordsInvolvedInMultipleRules);
      }
    }
    else
    {
      //skip end of name token matching
      implicitlyDerivedTags =
        _ruleReader->getImplicitTags(
          nameTokensList.toSet(), matchingWords, wordsInvolvedInMultipleRules);
    } 
  }
  LOG_VART(implicitlyDerivedTags);
}

void ImplicitTypeTaggerBase::_ensureCorrectTagSpecificity(const ElementPtr& e, Tags& tagsToAdd)
{
  Tags updatedTags;
  bool tagsAdded = false;
  LOG_VART(_elementIsASpecificFeature);
  for (Tags::const_iterator tagItr = tagsToAdd.begin(); tagItr != tagsToAdd.end(); ++tagItr)
  {
    const QString implicitTagKey = tagItr.key();
    LOG_VART(implicitTagKey);
    const QString implicitTagValue = tagItr.value();
    LOG_VART(implicitTagValue);
    LOG_VART(e->getTags().contains(implicitTagKey));
    if (e->getTags().contains(implicitTagKey))
    {
      //don't add a less specific tag if the element already has one with the same key; e.g. if
      //the element has amenity=public_hall, don't add amenity=hall; for ties, keep the one we
      //already have

      const QString elementTagKey = implicitTagKey;
      const QString elementTagValue = e->getTags()[implicitTagKey];
      LOG_VART(OsmSchema::getInstance().isAncestor(implicitTagKey % "=" % implicitTagValue,
                                                   elementTagKey % "=" % elementTagValue));
      if (OsmSchema::getInstance().isAncestor(implicitTagKey % "=" % implicitTagValue,
                                              elementTagKey % "=" % elementTagValue))
      {
        LOG_TRACE(
          implicitTagKey % "=" % implicitTagValue << " is more specific than " <<
          elementTagKey % "=" % elementTagValue << " on the input feature.  Replacing with " <<
          "the more specific tag.");
        updatedTags.appendValue(implicitTagKey, implicitTagValue);
        tagsAdded = true;
      }
      else
      {
        updatedTags.appendValue(elementTagKey, elementTagValue);
      }
    }
    else if (!_elementIsASpecificFeature || _allowTaggingSpecificFeatures)
    {
      LOG_TRACE(
        "Input feature does not contain key: " << implicitTagKey << " so adding tag: " <<
        implicitTagKey % "=" % implicitTagValue << "...");
      updatedTags.appendValue(implicitTagKey, implicitTagValue);
      tagsAdded = true;
    }
  }
  LOG_VART(updatedTags);
  LOG_VART(tagsAdded);
  if (tagsAdded)
  {
    tagsToAdd = updatedTags;
  }
  else
  {
    tagsToAdd.clear();
  }
  LOG_VART(tagsToAdd);
}

void ImplicitTypeTaggerBase::_updateElementForDuplicateMatch(const ElementPtr& e,
                                                             const QSet<QString>& matchingWords)
{
  QStringList matchingWordsList = matchingWords.toList();
  qSort(matchingWordsList.begin(), matchingWordsList.end(), caseInsensitiveLessThan);
  const QString tagValue =
    "No implicit tags added due to finding multiple possible matches for implicit tags: " +
    matchingWordsList.join(", ");
  LOG_VART(tagValue);
  e->getTags().appendValue("hoot:implicitTags:multipleRules", tagValue);
  _numFeaturesInvolvedInMultipleRules++;
  if (_numFeaturesInvolvedInMultipleRules % 10 == 0)
  {
    PROGRESS_INFO(
      StringUtils::formatLargeNumber(_numFeaturesInvolvedInMultipleRules) <<
      " features have been involved in multiple rules.");
  }
}

void ImplicitTypeTaggerBase::_addImplicitTags(const ElementPtr& e, const Tags& tagsToAdd,
                                              const QSet<QString>& matchingWords)
{
  //add implicit tags and associated metadata tags
  e->getTags().add(tagsToAdd);
  assert(matchingWords.size() != 0);
  QStringList matchingWordsList = matchingWords.toList();
  qSort(matchingWordsList.begin(), matchingWordsList.end(), caseInsensitiveLessThan);
  QString tagValue =
    "Added " + QString::number(tagsToAdd.size()) + " implicitly derived tag(s) based on: " +
    matchingWordsList.join(", ");
  tagValue += "; tags added: " + tagsToAdd.toString().trimmed().replace("\n", ", ");
  LOG_VART(tagValue);
  e->getTags().appendValue("hoot:implicitTags:tagsAdded", tagValue);

  //remove generic tags
  //Removing the generic tag after adding the specific tag was causing certain feature to be ignored
  //by the poi/poly criterion.
//  if (e->getTags().get("poi") == "yes")
//  {
//    e->getTags().remove("poi");
//  }
//  if (tagsToAdd.get("building") != "yes" && e->getTags().get("building") == "yes")
//  {
//    e->getTags().remove("building");
//  }
//  if (tagsToAdd.get("area") != "yes" && e->getTags().get("area") == "yes")
//  {
//    e->getTags().remove("area");
//  }

  _numFeaturesModified++;
  _numTagsAdded += tagsToAdd.size();
  if (_numTagsAdded < _smallestNumberOfTagsAdded)
  {
    _smallestNumberOfTagsAdded = _numTagsAdded;
  }
  if (tagsToAdd.size() > _largestNumberOfTagsAdded)
  {
    _largestNumberOfTagsAdded = tagsToAdd.size();
  }
  if (_numFeaturesModified % 100 == 0)
  {
    PROGRESS_INFO(
      "Added " << StringUtils::formatLargeNumber(_numTagsAdded) << " tags total to " <<
      StringUtils::formatLargeNumber(_numFeaturesModified) << " features.");
  }
}

QStringList ImplicitTypeTaggerBase::_getNameTokens(const QStringList& names) const
{
  if (_translateNamesToEnglish)
  {
    assert(names.size() == 1);
  }

  StringTokenizer tokenizer;
  QStringList nameTokens;
  foreach (const QString& n, names)
  {
    QStringList words = tokenizer.tokenize(n);
    foreach (const QString& w, words)
    {
      LOG_TRACE("Inserting token: " << w);
      nameTokens.append(w.toLower());
    }
  }

  return nameTokens;
}

}
