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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TO_ENGLISH_TRANSLATION_VISITOR_H
#define TO_ENGLISH_TRANSLATION_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/language/ToEnglishTranslator.h>

namespace hoot
{

/**
 * Translates selected tag values to English
 */
class ToEnglishTranslationVisitor : public ElementOsmMapVisitor, public Configurable
{

public:

  static std::string className() { return "hoot::ToEnglishTranslationVisitor"; }

  ToEnglishTranslationVisitor();
  virtual ~ToEnglishTranslationVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Translates selected tag values to English"; }

protected:

  boost::shared_ptr<ToEnglishTranslator> _translatorClient;

  QStringList _toTranslateTagKeys;
  QString _toTranslateTagKey;
  ElementPtr _element;

  bool _skipPreTranslatedTags;

  long _numTotalElements;

  QString _translatedText;

  /**
   * Translates a tag for an element
   *
   * @param e element containing the tag to translate
   * @param toTranslateTagKey the key of the tag whose value is to be translated
   * @return true if a successul translation was made; false otherwise
   */
  bool _translate(const ElementPtr& e, const QString toTranslateTagKey);

private:

  QString _toTranslateVal;

  bool _currentElementHasSuccessfulTagTranslation;
  long _numTagTranslationsMade;
  long _numElementsWithSuccessfulTagTranslation;
  long _numProcessedTags;
  long _numProcessedElements;
  int _taskStatusUpdateInterval;
};

}

#endif // TO_ENGLISH_TRANSLATION_VISITOR_H
