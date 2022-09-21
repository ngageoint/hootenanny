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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ToEnglishTranslationVisitor.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>

// std
#include <typeinfo>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ToEnglishTranslationVisitor)

ToEnglishTranslationVisitor::ToEnglishTranslationVisitor()
  : _ignorePreTranslatedTags(false),
    _parseNames(false),
    _numTotalElements(0),
    _numProcessedTags(0),
    _numProcessedElements(0),
    _currentElementHasSuccessfulTagTranslation(false),
    _numTagTranslationsMade(0),
    _numElementsWithSuccessfulTagTranslation(0),
    _taskStatusUpdateInterval(10000)
{
}

ToEnglishTranslationVisitor::~ToEnglishTranslationVisitor()
{
  //check the last element parsed to see if it had any successful tag translations
  if (_currentElementHasSuccessfulTagTranslation)
    _numElementsWithSuccessfulTagTranslation++;

  LOG_INFO(
    _numTagTranslationsMade << " successful tag translations made on " <<
    _numElementsWithSuccessfulTagTranslation << " different elements.");
  LOG_INFO(
    "Translated " << _numTagTranslationsMade << " tags out of " << _numProcessedTags <<
    " encountered.");
  LOG_INFO(
    "Attempted to translate tags for " << _numProcessedElements << " elements out of " <<
    _numTotalElements << " elements encountered.");
}

void ToEnglishTranslationVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translatorClient =
    Factory::getInstance().constructObject<ToEnglishTranslator>(opts.getLanguageTranslationTranslator());
  _translatorClient->setConfiguration(conf);
  _translatorClient->setSourceLanguages(opts.getLanguageTranslationSourceLanguages());

  _tagKeys = opts.getLanguageTagKeys().toSet();
  _ignorePreTranslatedTags = opts.getLanguageIgnorePreTranslatedTags();
  _parseNames = opts.getLanguageParseNames();
  if (_parseNames)
    _tagKeys = _tagKeys.unite(Tags::getNameKeys().toSet());
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
}

void ToEnglishTranslationVisitor::visit(const std::shared_ptr<Element>& e)
{
  if (_tagKeys.isEmpty())
    throw HootException("No tag keys specified for language translation.");

  LOG_VART(e);

  //if this var was set while parsing the previous element, increment the counter now
  if (_currentElementHasSuccessfulTagTranslation)
    _numElementsWithSuccessfulTagTranslation++;
  _currentElementHasSuccessfulTagTranslation = false;

  const Tags& tags = e->getTags();
  bool elementProcessed = false;
  for (const auto& toTranslateTagKey : qAsConst(_tagKeys))
  {
    if (tags.contains(toTranslateTagKey))
    {     
      //making skipping tags that already have an english translated tag optional, b/c a many of the
      //OSM english translations I've seen are either just copies of the foreign language text or are
      //not very good translations
      const QString preTranslatedTagKey = toTranslateTagKey + ":en";
      if (!_ignorePreTranslatedTags && tags.contains(preTranslatedTagKey))
      {
        LOG_TRACE("Skipping element with pre-translated tag: " << preTranslatedTagKey << "=" << tags.get(toTranslateTagKey).trimmed());
      }
      else
      {
        _translate(e, toTranslateTagKey);
        elementProcessed = true;
      }
    }
  }

  if (elementProcessed)
  {
    _numProcessedElements++;
    if (_numProcessedElements % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO("Attempted tag translation for " << _numProcessedElements << " elements.");
    }
  }

  _numTotalElements++;
  if (_numTotalElements % _taskStatusUpdateInterval == 0)
  {
    PROGRESS_INFO("Visited " << _numTotalElements << " elements.");
  }
}

bool ToEnglishTranslationVisitor::_translate(const ElementPtr& e, const QString& toTranslateTagKey)
{
  bool translationMade = false;

  _toTranslateTagKey = toTranslateTagKey;
  _toTranslateVal = e->getTags().get(toTranslateTagKey).trimmed();
  LOG_VART(_toTranslateVal);
  _element = e;

  _translatedText = _translatorClient->translate(_toTranslateVal).trimmed();
  LOG_VART(_translatedText);
  const int strComparison = _translatedText.compare(_toTranslateVal, Qt::CaseInsensitive);
  LOG_VART(strComparison);
  //If the translator merely returned the same string we passed in as the translated text, then
  //no point in using it.
  if (!_translatedText.trimmed().isEmpty() && strComparison != 0)
  {
    LOG_TRACE("Translated: " << _toTranslateVal << " to: " << _translatedText);

    _element->getTags().appendValue("hoot:translated:" + _toTranslateTagKey + ":en", _translatedText);

    QString sourceLang;
    LOG_VART(_translatorClient->getSourceLanguages().size());
    if (_translatorClient->getDetectedLanguage().trimmed().isEmpty() &&
        _translatorClient->getSourceLanguages().at(0) != "detect")
    {
      assert(_translatorClient->getSourceLanguages().size() == 1);
      sourceLang = _translatorClient->getSourceLanguages().at(0);
    }
    else
      sourceLang = _translatorClient->getDetectedLanguage();

    _element->getTags().appendValue("hoot:translated:" + _toTranslateTagKey + ":en:source:language", sourceLang);

    _numTagTranslationsMade++;
    if (_numTagTranslationsMade % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_DEBUG("Translated " << _numTagTranslationsMade << " tags.");
    }
    _currentElementHasSuccessfulTagTranslation = true;

    translationMade = true;
  }
  else
  {
    if (!_translatedText.trimmed().isEmpty())
    {
      LOG_TRACE(
        "Translator returned translation with same value as text passed in.  Discarding " <<
        "translation; text: " << _translatedText);
    }
    else
    {
      LOG_TRACE("Unable to translate text: " << _toTranslateVal);
    }
  }

  _numProcessedTags++;
  if (_numProcessedTags % _taskStatusUpdateInterval == 0)
  {
    PROGRESS_DEBUG("Processed " << _numProcessedTags << " tags.");
  }

  return translationMade;
}

}
