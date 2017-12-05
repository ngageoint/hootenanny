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
#include "AddImplicitlyDerivedTagsBaseVisitor.h"

#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/algorithms/Translator.h>

// Qt
#include <QSet>
#include <QStringBuilder>

namespace hoot
{

AddImplicitlyDerivedTagsBaseVisitor::AddImplicitlyDerivedTagsBaseVisitor() :
_allowTaggingSpecificPois(ConfigOptions().getPoiImplicitTagRulesAllowTaggingSpecificPois()),
_allowTaggingGenericPois(ConfigOptions().getPoiImplicitTagRulesAllowTaggingGenericPois()),
_elementIsASpecificPoi(false),
_tokenizeNames(ConfigOptions().getPoiImplicitTagRulesTokenizeNames()),
_numNodesModified(0),
_numTagsAdded(0),
_numNodesInvolvedInMultipleRules(0),
_numNodesParsed(0),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_minWordLength(ConfigOptions().getPoiImplicitTagRulesMinimumWordLength()),
_smallestNumberOfTagsAdded(LONG_MAX),
_largestNumberOfTagsAdded(0),
_maxWordTokenizationGroupSize(1),
_translateAllNamesToEnglish(false)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(ConfigOptions().getPoiImplicitTagRulesDatabase());
}

AddImplicitlyDerivedTagsBaseVisitor::AddImplicitlyDerivedTagsBaseVisitor(const QString databasePath) :
_allowTaggingSpecificPois(ConfigOptions().getPoiImplicitTagRulesAllowTaggingSpecificPois()),
_allowTaggingGenericPois(ConfigOptions().getPoiImplicitTagRulesAllowTaggingGenericPois()),
_elementIsASpecificPoi(false),
_tokenizeNames(ConfigOptions().getPoiImplicitTagRulesTokenizeNames()),
_numNodesModified(0),
_numTagsAdded(0),
_numNodesInvolvedInMultipleRules(0),
_numNodesParsed(0),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_minWordLength(ConfigOptions().getPoiImplicitTagRulesMinimumWordLength()),
_smallestNumberOfTagsAdded(LONG_MAX),
_largestNumberOfTagsAdded(0),
_maxWordTokenizationGroupSize(1),
_translateAllNamesToEnglish(false)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(databasePath);
}

AddImplicitlyDerivedTagsBaseVisitor::~AddImplicitlyDerivedTagsBaseVisitor()
{
  if (_ruleReader)
  {
    LOG_VAR(_ruleReader->getTagsCacheSize());
    LOG_VAR(_ruleReader->getFirstRoundTagsCacheHits());
    LOG_VAR(_ruleReader->getSecondRoundTagsCacheHits());
    _ruleReader->close();
  }

  LOG_INFO(
    "Added " << StringUtils::formatLargeNumber(_numTagsAdded) << " tags to " <<
    StringUtils::formatLargeNumber(_numNodesModified) << " nodes / " <<
    StringUtils::formatLargeNumber(_numNodesParsed)  << " total nodes.");
  LOG_INFO(
    StringUtils::formatLargeNumber(_numNodesInvolvedInMultipleRules) <<
    " nodes were involved in multiple tag rules and were not modified.");
  if (_numTagsAdded > 0 && _numNodesModified > 0)
  {
    long avgTagsAdded = (long)(_numTagsAdded / _numNodesModified);
    LOG_INFO(
      "Average tags added per node: " << StringUtils::formatLargeNumber(avgTagsAdded));
    LOG_INFO(
      "Smallest number of tags added to a node: " <<
       StringUtils::formatLargeNumber(_smallestNumberOfTagsAdded));
    LOG_INFO(
      "Largest number of tags added to a node: " <<
       StringUtils::formatLargeNumber(_largestNumberOfTagsAdded));
  }
}

void AddImplicitlyDerivedTagsBaseVisitor::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);
  setTranslateAllNamesToEnglish(confOptions.getPoiImplicitTagRulesTranslateAllNamesToEnglish());
  _customRules.setCustomRuleFile(confOptions.getPoiImplicitTagRulesCustomRuleFile());
  _customRules.setRuleIgnoreFile(confOptions.getPoiImplicitTagRulesRuleIgnoreFile());
  _customRules.setTagIgnoreFile(confOptions.getPoiImplicitTagRulesTagIgnoreFile());
  _customRules.setTagFile(confOptions.getPoiImplicitTagRulesTagFile());
  _customRules.setWordIgnoreFile(confOptions.getPoiImplicitTagRulesWordIgnoreFile());

  _customRules.init();
  LOG_VARD(_customRules.getWordIgnoreList());
  LOG_VARD(_customRules.getCustomRulesList());
  _ruleReader->setCustomRules(_customRules);
}

bool caseInsensitiveLessThan(const QString s1, const QString s2)
{
  return s1.toLower() < s2.toLower();
}

void AddImplicitlyDerivedTagsBaseVisitor::visit(const ElementPtr& e)
{
  if (_visitElement(e))
  {
    //get names
    //query for implicit tags for the names; pass in a ref wordsInvolvedInMultipleRules param
    //if wordsInvolvedInMultipleRules is true, mark the feature
    //else if tags size > 0, add the tags
    //else get name tokens
    //name token logic follows above

    bool foundDuplicateMatch = false;
    Tags tagsToAdd;

    //TODO: if name:en doesn't exist (or is not the same as name), translate here (?) - also see
    //tokenize method
    QStringList names = e->getTags().getNames();

    if (_translateAllNamesToEnglish)
    {
      QStringList filteredNames;
      if (e->getTags().contains("name:en"))
      {
        filteredNames.append(e->getTags().get("name:en"));
      }
      else
      {
        for (int i = 0; i < names.size(); i++)
        {
          const QString name = names.at(i);
          LOG_VART(name);
          if (name != e->getTags().get("alt_name"))
          {
            const QString englishName = Translator::getInstance().toEnglish(name);
            LOG_VART(englishName);
            filteredNames.append(englishName);
            break;
          }
        }
        if (filteredNames.isEmpty() && e->getTags().contains("alt_name"))
        {
          QString altName = e->getTags().get("alt_name");
          if (altName.contains(";"))
          {
            altName = altName.split(";")[0];
          }
          LOG_VART(altName);
          filteredNames.append(Translator::getInstance().toEnglish(e->getTags().get("alt_name")));
        }
      }
      LOG_VART(filteredNames);
      assert(!filteredNames.isEmpty());
      names = filteredNames;

//      if (!names.contains("name:en"))
//      {
//        for (int i = 0; i < names.size(); i++)
//        {
//          const QString name = names.at(i);
//          if (name.startsWith("name"))
//          {
//            const QString englishTranslatedName = _englishTranslator.toEnglish(name);
//            e->getTags().set("name:en", englishTranslatedName);
//            break;
//          }
//        }
//        if (!e->getTags().contains("name:en") && names.contains("alt_name"))
//        {
//          const QString englishTranslatedName =
//            _englishTranslator.toEnglish(e->getTags().get("alt_name"));
//          e->getTags().set("name:en", englishTranslatedName);
//        }
//      }
//      LOG_VART(e->getTags());

//      Tags tagsCopy;
//      for (Tags::const_iterator tagItr = e->getTags().begin();
//           tagItr != e->getTags().end(); ++tagItr)
//      {
//        const QString tagKey = tagItr.key();
//        if (tagKey.contains("name:") && tagKey != QLatin1String("name:en"))
//        {
//        }
//        else if (tagKey == QLatin1String("alt_name") || tagKey == QLatin1String("name"))
//        {
//        }
//        else
//        {
//          tagsCopy.appendValue(tagKey, tagItr.value());
//        }
//      }
//      LOG_VART(tagsCopy);
//      names = tagsCopy.getNames();
    }
    LOG_VART(names);

    QStringList filteredNames;
    for (int i = 0; i < names.size(); i++)
    {
      const QString name = names.at(i);
      if (name.length() >= _minWordLength &&
          !_customRules.getWordIgnoreList().contains(name.toLower()))
      {
        filteredNames.append(name);
      }
    }
    LOG_VART(filteredNames);

    if (filteredNames.size() > 0)
    {
      Tags implicitlyDerivedTags;
      QSet<QString> matchingWords;
      bool wordsInvolvedInMultipleRules = false;

      //check custom rules first
      for (int i = 0; i < filteredNames.size(); i++)
      {
        const QString word = filteredNames.at(i);
        const QString tag = _customRules.getCustomRulesList().value(word, "");
        if (!tag.trimmed().isEmpty())
        {
          implicitlyDerivedTags.appendValue(tag);
          matchingWords.insert(word);
          LOG_TRACE("Found custom rule for word: " << word << " and tag: " << tag);
          break;
        }
      }

      if (implicitlyDerivedTags.size() == 0)
      {
        //the name phrases take precendence over the tokenized names, so look for tags associated with
        //them first
        implicitlyDerivedTags =
          _ruleReader->getImplicitTags(
            /*names*/filteredNames.toSet(), matchingWords, wordsInvolvedInMultipleRules);
      }

      LOG_VART(implicitlyDerivedTags);
      LOG_VART(matchingWords);
      LOG_VART(wordsInvolvedInMultipleRules);

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
      else if (_tokenizeNames)
      {
        //we didn't find any tags for the whole names, so let's look for them with the tokenized name
        //parts
        const QSet<QString> nameTokens = _getNameTokens(filteredNames);
        QStringList nameTokensList = nameTokens.toList();

        //check custom rules first, then db for each group size in descending group size order

        if (implicitlyDerivedTags.size() == 0 && nameTokens.size() > 3 &&
            _maxWordTokenizationGroupSize >= 3)
        {
          QStringList nameTokensListGroupSizeThree;
          for (int i = 0; i < nameTokensList.size() - 2; i++)
          {
            QString nameToken =
              nameTokensList.at(i) + " " + nameTokensList.at(i + 1) + " " + nameTokensList.at(i + 2);
            if (_translateAllNamesToEnglish)
            {
              const QString englishNameToken = Translator::getInstance().toEnglish(nameToken);
//              if (englishNameToken.toLower() != nameToken.toLower())
//              {
//                LOG_TRACE(
//                  "Successfully translated " << nameToken << " to " << englishNameToken << ".");
//              }
              nameToken = englishNameToken;
              LOG_VART(englishNameToken);
            }
            nameTokensListGroupSizeThree.append(nameToken);
          }
          for (int i = 0; i < nameTokensListGroupSizeThree.size(); i++)
          {
            const QString word = nameTokensListGroupSizeThree.at(i);
            const QString tag = _customRules.getCustomRulesList().value(word, "");
            if (!tag.trimmed().isEmpty())
            {
              implicitlyDerivedTags.appendValue(tag);
              matchingWords.insert(word);
              LOG_TRACE("Found custom rule for word: " << word << " and tag: " << tag);
              break;
            }
          }
          if (implicitlyDerivedTags.size() == 0)
          {
            implicitlyDerivedTags =
              _ruleReader->getImplicitTags(
                nameTokensListGroupSizeThree.toSet(), matchingWords, wordsInvolvedInMultipleRules);
          }
        }

        if (implicitlyDerivedTags.size() == 0 && nameTokens.size() > 2 &&
            _maxWordTokenizationGroupSize >= 2)
        {
          QStringList nameTokensListGroupSizeTwo;
          for (int i = 0; i < nameTokensList.size() - 1; i++)
          {
            QString nameToken = nameTokensList.at(i) + " " + nameTokensList.at(i + 1);
            if (_translateAllNamesToEnglish)
            {
              const QString englishNameToken = Translator::getInstance().toEnglish(nameToken);
//              if (englishNameToken.toLower() != nameToken.toLower())
//              {
//                LOG_TRACE(
//                  "Successfully translated " << nameToken << " to " << englishNameToken << ".");
//              }
              nameToken = englishNameToken;
              LOG_VART(englishNameToken);
            }
            nameTokensListGroupSizeTwo.append(nameToken);
          }
          for (int i = 0; i < nameTokensListGroupSizeTwo.size(); i++)
          {
            const QString word = nameTokensListGroupSizeTwo.at(i);
            const QString tag = _customRules.getCustomRulesList().value(word, "");
            if (!tag.trimmed().isEmpty())
            {
              implicitlyDerivedTags.appendValue(tag);
              matchingWords.insert(word);
              LOG_TRACE("Found custom rule for word: " << word << " and tag: " << tag);
              break;
            }
          }
          if (implicitlyDerivedTags.size() == 0)
          {
            implicitlyDerivedTags =
              _ruleReader->getImplicitTags(
                nameTokensListGroupSizeTwo.toSet(), matchingWords, wordsInvolvedInMultipleRules);
          }
        }

        if (implicitlyDerivedTags.size() == 0)
        {
          if (_translateAllNamesToEnglish)
          {
            QStringList translatedNameTokens;
            for (int i = 0; i < nameTokensList.size(); i++)
            {
              const QString word = nameTokensList.at(i);
              const QString englishNameToken = Translator::getInstance().toEnglish(word);
//              if (englishNameToken.toLower() != word.toLower())
//              {
//                LOG_TRACE(
//                  "Successfully translated " << word << " to " << englishNameToken << ".");
//              }
              translatedNameTokens.append(englishNameToken);
            }
            nameTokensList = translatedNameTokens;
          }
          for (int i = 0; i < nameTokensList.size(); i++)
          {
            const QString word = nameTokensList.at(i);
            const QString tag = _customRules.getCustomRulesList().value(word, "");
            if (!tag.trimmed().isEmpty())
            {
              implicitlyDerivedTags.appendValue(tag);
              matchingWords.insert(word);
              LOG_TRACE("Found custom rule for word: " << word << " and tag: " << tag);
              break;
            }
          }

          if (implicitlyDerivedTags.size() == 0)
          {
            implicitlyDerivedTags =
              _ruleReader->getImplicitTags(nameTokens, matchingWords, wordsInvolvedInMultipleRules);
          }
        }

        LOG_VART(implicitlyDerivedTags);
        LOG_VART(matchingWords);
        LOG_VART(wordsInvolvedInMultipleRules);

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
      LOG_VART(tagsToAdd);

      LOG_VART(foundDuplicateMatch);
      if (foundDuplicateMatch)
      {
        QStringList matchingWordsList = matchingWords.toList();
        qSort(matchingWordsList.begin(), matchingWordsList.end(), caseInsensitiveLessThan);
        const QString tagValue =
          "No implicit tags added due to finding multiple possible matches for implicit tags: " +
          matchingWordsList.join(", ");
        LOG_VART(tagValue);
        e->getTags().appendValue("hoot:implicitTags:multipleRules", tagValue);
        _numNodesInvolvedInMultipleRules++;
        if (_numNodesInvolvedInMultipleRules % 10 == 0)
        {
          PROGRESS_INFO(
            StringUtils::formatLargeNumber(_numNodesInvolvedInMultipleRules) << " nodes have been " <<
            "involved in multiple rules.");
        }
      }
      else if (!tagsToAdd.isEmpty())
      {  
        assert(!matchingWords.isEmpty());
        LOG_VART(matchingWords);

        Tags ruleFilteredTags;
        QMap<QString, QString> rulesToIgnore = _customRules.getRulesIgnoreList();
        for (QSet<QString>::const_iterator wordItr = matchingWords.begin();
             wordItr != matchingWords.end(); ++wordItr)
        {
          const QString word = *wordItr;
          LOG_VART(word);
          for (Tags::const_iterator tagItr = tagsToAdd.begin(); tagItr != tagsToAdd.end(); ++tagItr)
          {
            const QString tagKey = tagItr.key();
            LOG_VART(tagKey);
            const QString tagValue = tagItr.value();
            LOG_VART(tagValue);
            if (!ruleFilteredTags.contains(tagKey) && rulesToIgnore[word] != tagKey % "=" % tagValue)
            {
              LOG_VART(tagKey % "=" % tagValue);
              ruleFilteredTags.appendValue(tagKey, tagValue);
            }
          }
        }
        LOG_VART(ruleFilteredTags);

        Tags updatedTags;
        bool tagsAdded = false;
        //TODO: check tag include list
        LOG_VART(_elementIsASpecificPoi);
        for (Tags::const_iterator tagItr = ruleFilteredTags.begin();
             tagItr != ruleFilteredTags.end(); ++tagItr)
        {
          const QString implicitTagKey = tagItr.key();
          LOG_VART(implicitTagKey);
          const QString implicitTagValue = tagItr.value();
          LOG_VART(implicitTagValue);
          const QString tagStr = implicitTagKey % "=" % implicitTagValue;
          if (_customRules.getTagIgnoreList().contains(tagStr))
          {
            //skip
          }
          else if (e->getTags().contains(implicitTagKey))
          {
            //don't add a less specific tag if the element already has one with the same key; e.g. if
            //the element has amenity=public_hall, don't add amenity=hall
            //for ties keep the one we already have; e.g. if the element has amenity=bank, don't add
            //amenity=school

            const QString elementTagKey = implicitTagKey;
            const QString elementTagValue = e->getTags()[implicitTagKey];
            //only use the implicit tag if it is more specific than the one the element already has;
            //if neither is more specific than the other, we'll arbitrarily keep the one we already had
            LOG_VART(OsmSchema::getInstance().isAncestor(implicitTagKey % "=" % implicitTagValue,
                                                         elementTagKey % "=" % elementTagValue));
            if (OsmSchema::getInstance().isAncestor(implicitTagKey % "=" % implicitTagValue,
                                                    elementTagKey % "=" % elementTagValue))
            {
              LOG_TRACE(
                implicitTagKey % "=" % implicitTagValue << " is more specific than " <<
                elementTagKey % "=" % elementTagValue << " on the input feature.  Replacing with " <<
                "the more specific tag.")
              updatedTags.appendValue(implicitTagKey, implicitTagValue);
              tagsAdded = true;
            }
            else
            {
              updatedTags.appendValue(elementTagKey, elementTagValue);
            }
          }
          //TODO: Silver Avenue clinic - but increases wrong ways in C
          else if (!_elementIsASpecificPoi)
          {
            LOG_TRACE("Input feature does not contain tag: " <<
                      implicitTagKey % "=" % implicitTagValue << ", so adding it...");
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

        if (!tagsToAdd.isEmpty())
        {
          e->getTags().addTags(tagsToAdd);
          assert(matchingWords.size() != 0);
          QStringList matchingWordsList = matchingWords.toList();
          qSort(matchingWordsList.begin(), matchingWordsList.end(), caseInsensitiveLessThan);
          QString tagValue =
            "Added " + QString::number(tagsToAdd.size()) + " implicitly derived tag(s) based on: " +
            matchingWordsList.join(", ");
          tagValue += "; tags added: " + tagsToAdd.toString().trimmed().replace("\n", ", ");
          LOG_VART(tagValue);
          e->getTags().appendValue("hoot:implicitTags:tagsAdded", tagValue);
          _numNodesModified++;
          _numTagsAdded += tagsToAdd.size();
          if (_numTagsAdded < _smallestNumberOfTagsAdded)
          {
            _smallestNumberOfTagsAdded = _numTagsAdded;
          }
          if (tagsToAdd.size() > _largestNumberOfTagsAdded)
          {
            _largestNumberOfTagsAdded = tagsToAdd.size();
          }
          if (_numNodesModified % 100 == 0)
          {
            PROGRESS_INFO(
              "Added " << StringUtils::formatLargeNumber(_numTagsAdded) << " tags total to " <<
              StringUtils::formatLargeNumber(_numNodesModified) << " nodes.");
          }
        }
      }
    }

    _numNodesParsed++;
    if (_numNodesParsed % 1000 == 0)
    {
      PROGRESS_INFO(
        "Parsed " << StringUtils::formatLargeNumber(_numNodesParsed) << " nodes from input.");
    }
  }
}

QSet<QString> AddImplicitlyDerivedTagsBaseVisitor::_getNameTokens(const QStringList names)
{
  QStringList result;

  //const QStringList names = t.getNames();
  //LOG_VART(names);

  if (_translateAllNamesToEnglish)
  {
    assert(names.size() == 1);
    //assert(t.contains("name:en"));
  }

  StringTokenizer tokenizer;
  foreach (const QString& n, names)
  {
    QStringList words = tokenizer.tokenize(n);
    foreach (const QString& w, words)
    {
      if (!_customRules.getWordIgnoreList().contains(w.toLower()))
      {
        LOG_TRACE("Inserting token: " << w);
        result.append(w.toLower());
      }
    }
  }

  QStringList filteredNames;
  for (int i = 0; i < result.size(); i++)
  {
    const QString name = result.at(i);
    if (name.length() >= _minWordLength &&
        !_customRules.getWordIgnoreList().contains(name.toLower()))
    {
      filteredNames.append(name);
    }
  }
  LOG_VART(filteredNames);

  return filteredNames.toSet();
}

}
