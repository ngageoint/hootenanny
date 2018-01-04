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
#include "ImplicitTaggerBase.h"

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

ImplicitTaggerBase::ImplicitTaggerBase() :
_allowTaggingSpecificPois(true),
_elementIsASpecificPoi(false),
_numNodesModified(0),
_numTagsAdded(0),
_numNodesInvolvedInMultipleRules(0),
_numNodesParsed(0),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_minWordLength(3),
_smallestNumberOfTagsAdded(LONG_MAX),
_largestNumberOfTagsAdded(0),
_translateAllNamesToEnglish(true),
_matchEndOfNameSingleTokenFirst(true)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(ConfigOptions().getPoiImplicitTagRulesDatabase());
}

ImplicitTaggerBase::ImplicitTaggerBase(const QString databasePath) :
_allowTaggingSpecificPois(true),
_elementIsASpecificPoi(false),
_numNodesModified(0),
_numTagsAdded(0),
_numNodesInvolvedInMultipleRules(0),
_numNodesParsed(0),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_minWordLength(3),
_smallestNumberOfTagsAdded(LONG_MAX),
_largestNumberOfTagsAdded(0),
_translateAllNamesToEnglish(true),
_matchEndOfNameSingleTokenFirst(true)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(databasePath);
}

ImplicitTaggerBase::~ImplicitTaggerBase()
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

void ImplicitTaggerBase::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);

  setTranslateAllNamesToEnglish(confOptions.getPoiImplicitTagRulesTranslateAllNamesToEnglish());
  setMatchEndOfNameSingleTokenFirst(confOptions.getPoiImplicitTagRulesMatchEndOfNameSingleTokenFirst());
  setAllowTaggingSpecificPois(confOptions.getPoiImplicitTagRulesAllowTaggingSpecificPois());
  setMinWordLength(confOptions.getPoiImplicitTagRulesMinimumWordLength());

  _customRules.setCustomRuleFile(confOptions.getPoiImplicitTagRulesCustomRuleFile());
  _customRules.setTagIgnoreFile(confOptions.getPoiImplicitTagRulesTagIgnoreFile());
  _customRules.setWordIgnoreFile(confOptions.getPoiImplicitTagRulesWordIgnoreFile());

  _customRules.init();
  LOG_VARD(_customRules.getWordIgnoreList());
  LOG_VARD(_customRules.getCustomRulesList());
  _ruleReader->setConfiguration(conf);
  _ruleReader->setCustomRules(_customRules);
}

bool caseInsensitiveLessThan(const QString s1, const QString s2)
{
  return s1.toLower() < s2.toLower();
}

void ImplicitTaggerBase::visit(const ElementPtr& e)
{
  if (_visitElement(e))
  {
    bool foundDuplicateMatch = false;
    Tags tagsToAdd;

    QStringList names = e->getTags().getNames();

    if (names.removeAll("old_name") > 0)
    {
      LOG_VARD("Removed old name tag.");
    }

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
    }
    LOG_VARD(names);

    QStringList filteredNames;
    for (int i = 0; i < names.size(); i++)
    {
      QString name = names.at(i);

      //TODO: move to common class
      name =
        name.replace("(", "").replace(")", "").replace(".", "").replace("/", " ").replace("<", "")
            .replace(">", "").replace("[", "").replace("]", "").replace("@", "").replace("&", "and");
      if (name.startsWith("-"))
      {
        name = name.replace(0, 1, "");
      }
      if (name.startsWith("_"))
      {
        name = name.replace(0, 1, "");
      }
      //TOOD: expand this
      if (name.at(0).isDigit() &&
          (name.endsWith("th") || name.endsWith("nd") || name.endsWith("rd") || name.endsWith("st") ||
           name.endsWith("ave") || name.endsWith("avenue") || name.endsWith("st") ||
           name.endsWith("street") || name.endsWith("pl") || name.endsWith("plaza")))
      {
        name = "";
      }

      if (name.length() >= _minWordLength &&
          !_customRules.getWordIgnoreList().contains(name.toLower()))
      {
        filteredNames.append(name.toLower());
      }
    }
    LOG_VARD(filteredNames);

    if (filteredNames.size() > 0)
    {
      Tags implicitlyDerivedTags;
      QSet<QString> matchingWords;
      bool wordsInvolvedInMultipleRules = false;

      if (implicitlyDerivedTags.size() == 0)
      {
        //the name phrases take precendence over the tokenized names, so look for tags associated with
        //them first
        implicitlyDerivedTags =
          _ruleReader->getImplicitTags(
            filteredNames.toSet(), matchingWords, wordsInvolvedInMultipleRules);
      }

      LOG_VARD(implicitlyDerivedTags);
      LOG_VARD(matchingWords);
      LOG_VARD(wordsInvolvedInMultipleRules);

      bool namesContainBuilding = false;
      bool namesContainOffice = false;
      if (wordsInvolvedInMultipleRules)
      {
        LOG_DEBUG(
          "Found duplicate match for names: " << filteredNames << " with matching words: " <<
          matchingWords);
        foundDuplicateMatch = true;
      }
      else if (implicitlyDerivedTags.size() > 0)
      {
        LOG_DEBUG(
          "Derived implicit tags for names: " << filteredNames << " with matching words: " <<
            matchingWords);
        tagsToAdd = implicitlyDerivedTags;
      }
      else
      {
        //we didn't find any tags for the whole names, so let's look for them with the tokenized name
        //parts
        //const QSet<QString> nameTokens = _getNameTokens(filteredNames);
        QStringList nameTokensList = _getNameTokens(filteredNames);

        LOG_VARD(nameTokensList);
        LOG_VARD(implicitlyDerivedTags.size());
        LOG_VARD(nameTokensList.size());

        //check custom rules first, then db for each group size in descending group size order

        if (implicitlyDerivedTags.size() == 0 && nameTokensList.size() > 2)
        {
          LOG_DEBUG("Attempting match with token group size of 2...");

          QStringList nameTokensListGroupSizeTwo;
          for (int i = 0; i < nameTokensList.size() - 1; i++)
          {
            QString nameToken = nameTokensList.at(i) + " " + nameTokensList.at(i + 1);
            if (_translateAllNamesToEnglish)
            {
              const QString englishNameToken = Translator::getInstance().toEnglish(nameToken);
              nameToken = englishNameToken;
              LOG_VART(englishNameToken);
            }
            nameTokensListGroupSizeTwo.append(nameToken);
          }
          LOG_VARD(nameTokensListGroupSizeTwo);

          if (_matchEndOfNameSingleTokenFirst)
          {
            QString endOfNameToken;

            QString name = e->getTags().get("name:en");
            for (int i = 0; i < nameTokensListGroupSizeTwo.size(); i++)
            {
              const QString nameToken = nameTokensListGroupSizeTwo.at(i);
              if (name.endsWith(nameToken))
              {
                endOfNameToken = nameToken;
                break;
              }
            }

            if (endOfNameToken.isEmpty())
            {
              QString name = e->getTags().get("name");
              for (int i = 0; i < nameTokensListGroupSizeTwo.size(); i++)
              {
                const QString nameToken = nameTokensListGroupSizeTwo.at(i);
                if (name.endsWith(nameToken))
                {
                  endOfNameToken = nameToken;
                  break;
                }
              }
            }

            if (endOfNameToken.isEmpty())
            {
              for (int i = 0; i < filteredNames.size(); i++)
              {
                const QString filteredName = filteredNames.at(i);
                for (int j = 0; j < nameTokensListGroupSizeTwo.size(); j++)
                {
                  const QString nameToken = nameTokensListGroupSizeTwo.at(j);
                  if (filteredName.endsWith(nameToken))
                  {
                    endOfNameToken = nameToken;
                    break;
                  }
                }

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
                implicitlyDerivedTags =
                  _ruleReader->getImplicitTags(
                    nameTokensListGroupSizeTwo.toSet(), matchingWords, wordsInvolvedInMultipleRules);
              }
            }
            else
            {
              implicitlyDerivedTags =
                _ruleReader->getImplicitTags(
                  nameTokensListGroupSizeTwo.toSet(), matchingWords, wordsInvolvedInMultipleRules);
            }
          }
          else
          {
            implicitlyDerivedTags =
              _ruleReader->getImplicitTags(
                nameTokensListGroupSizeTwo.toSet(), matchingWords, wordsInvolvedInMultipleRules);
          }

          LOG_VARD(implicitlyDerivedTags);
          LOG_VARD(matchingWords);
          LOG_VARD(wordsInvolvedInMultipleRules);
        }

        if (implicitlyDerivedTags.size() == 0)
        {
          LOG_DEBUG("Attempting match with token group size of 1...");

          if (_translateAllNamesToEnglish)
          {
            QStringList translatedNameTokens;
            for (int i = 0; i < nameTokensList.size(); i++)
            {
              const QString word = nameTokensList.at(i);
              const QString englishNameToken = Translator::getInstance().toEnglish(word);
              translatedNameTokens.append(englishNameToken);
            }
            nameTokensList = translatedNameTokens;
          }
          LOG_VARD(nameTokensList);

          if (nameTokensList.contains("building") || nameTokensList.contains("buildings"))
          {
            namesContainBuilding = true;
            nameTokensList.removeAll("building");
            nameTokensList.removeAll("buildings");
          }
          if (nameTokensList.contains("office") || nameTokensList.contains("offices"))
          {
            namesContainOffice = true;
            nameTokensList.removeAll("office");
            nameTokensList.removeAll("offices");
          }

          if (implicitlyDerivedTags.size() == 0 && nameTokensList.size() > 0)
          {
            if (_matchEndOfNameSingleTokenFirst)
            {
              QString endOfNameToken;

              QString name = e->getTags().get("name:en");
              for (int i = 0; i < nameTokensList.size(); i++)
              {
                const QString nameToken = nameTokensList.at(i);
                if (name.endsWith(nameToken))
                {
                  endOfNameToken = nameToken;
                  break;
                }
              }

              if (endOfNameToken.isEmpty())
              {
                QString name = e->getTags().get("name");
                for (int i = 0; i < nameTokensList.size(); i++)
                {
                  const QString nameToken = nameTokensList.at(i);
                  if (name.endsWith(nameToken))
                  {
                    endOfNameToken = nameToken;
                    break;
                  }
                }
              }

              if (endOfNameToken.isEmpty())
              {
                for (int i = 0; i < filteredNames.size(); i++)
                {
                  const QString filteredName = filteredNames.at(i);
                  for (int j = 0; j < nameTokensList.size(); j++)
                  {
                    const QString nameToken = nameTokensList.at(j);
                    if (filteredName.endsWith(nameToken))
                    {
                      endOfNameToken = nameToken;
                      break;
                    }
                  }

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
                  implicitlyDerivedTags =
                    _ruleReader->getImplicitTags(
                      nameTokensList.toSet(), matchingWords, wordsInvolvedInMultipleRules);
                }
              }
              else
              {
                implicitlyDerivedTags =
                  _ruleReader->getImplicitTags(
                    nameTokensList.toSet(), matchingWords, wordsInvolvedInMultipleRules);
              }
            }
            else
            {
              implicitlyDerivedTags =
                _ruleReader->getImplicitTags(
                  nameTokensList.toSet(), matchingWords, wordsInvolvedInMultipleRules);
            }
          }
        }

        LOG_VARD(implicitlyDerivedTags);
        LOG_VARD(matchingWords);
        LOG_VARD(wordsInvolvedInMultipleRules);

        if (wordsInvolvedInMultipleRules)
        {
          LOG_DEBUG(
            "Found duplicate match for name tokens: " << nameTokensList << " with matching words: " <<
            matchingWords);
          foundDuplicateMatch = true;
        }
        else if (implicitlyDerivedTags.size() > 0)
        {
          LOG_DEBUG(
            "Derived implicit tags for name tokens: " << nameTokensList << " with matching words: " <<
            matchingWords);
          tagsToAdd = implicitlyDerivedTags;
        }
      }
      LOG_VARD(tagsToAdd);

      LOG_VARD(foundDuplicateMatch);
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

        //TODO: remove this
        Tags ruleFilteredTags = tagsToAdd;
        LOG_VARD(ruleFilteredTags);

        Tags updatedTags;
        bool tagsAdded = false;
        //TODO: check tag include list
        LOG_VARD(_elementIsASpecificPoi);
        for (Tags::const_iterator tagItr = ruleFilteredTags.begin();
             tagItr != ruleFilteredTags.end(); ++tagItr)
        {
          const QString implicitTagKey = tagItr.key();
          LOG_VARD(implicitTagKey);
          const QString implicitTagValue = tagItr.value();
          LOG_VARD(implicitTagValue);
          const QString tagStr = implicitTagKey % "=" % implicitTagValue;
          if (_customRules.getTagIgnoreList().contains(tagStr))
          {
            LOG_DEBUG("Skipping tag on ignore list: " << tagStr);
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
            LOG_VARD(OsmSchema::getInstance().isAncestor(implicitTagKey % "=" % implicitTagValue,
                                                         elementTagKey % "=" % elementTagValue));
            if (OsmSchema::getInstance().isAncestor(implicitTagKey % "=" % implicitTagValue,
                                                    elementTagKey % "=" % elementTagValue))
            {
              LOG_DEBUG(
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
          else if (!_elementIsASpecificPoi)
          {
            LOG_DEBUG("Input feature does not contain tag: " <<
                      implicitTagKey % "=" % implicitTagValue << ", so adding it...");
            updatedTags.appendValue(implicitTagKey, implicitTagValue);
            tagsAdded = true;
          }
        }
        LOG_VARD(updatedTags);
        LOG_VARD(tagsAdded);
        if (tagsAdded)
        {
          tagsToAdd = updatedTags;
        }
        else
        {
          tagsToAdd.clear();
        }
        LOG_VARD(tagsToAdd);

        if (tagsToAdd.isEmpty() && !_elementIsASpecificPoi && namesContainOffice)
        {
          tagsToAdd.appendValue("building", "office");
          matchingWords.insert("office");
        }
        else if (tagsToAdd.isEmpty() && !_elementIsASpecificPoi && namesContainBuilding)
        {
          tagsToAdd.appendValue("building", "yes");
          matchingWords.insert("building");
        }

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
          LOG_VARD(tagValue);
          e->getTags().appendValue("hoot:implicitTags:tagsAdded", tagValue);
          if (e->getTags().get("poi") == "yes")
          {
            e->getTags().remove("poi");
          }
          if (tagsToAdd.get("building") != "yes" && e->getTags().get("building") == "yes")
          {
            e->getTags().remove("building");
          }
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

QStringList ImplicitTaggerBase::_getNameTokens(const QStringList names)
{
  QStringList result;

  if (_translateAllNamesToEnglish)
  {
    assert(names.size() == 1);
  }

  StringTokenizer tokenizer;
  foreach (const QString& n, names)
  {
    QStringList words = tokenizer.tokenize(n);
    foreach (const QString& w, words)
    {
      LOG_TRACE("Inserting token: " << w);
      result.append(w.toLower());
    }
  }

  QStringList filteredNames;
  for (int i = 0; i < result.size(); i++)
  {
    const QString name = result.at(i);
    filteredNames.append(name);
  }
  LOG_VART(filteredNames);

  return filteredNames;
}

}
