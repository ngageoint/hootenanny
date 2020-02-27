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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TO_ENGLISH_TRANSLATION_VISITOR_H
#define TO_ENGLISH_TRANSLATION_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/info/OperationStatusInfo.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * Translates selected tag values to English
 */
class ToEnglishTranslationVisitor : public ElementVisitor, public Configurable
{

public:

  static std::string className() { return "hoot::ToEnglishTranslationVisitor"; }

  ToEnglishTranslationVisitor();
  virtual ~ToEnglishTranslationVisitor();

  virtual void visit(const std::shared_ptr<Element>& e) override;

  virtual void setConfiguration(const Settings& conf) override;

  virtual QString getDescription() const override
  { return "Translates selected tag values to English"; }

  virtual QString getInitStatusMessage() const override
  { return "Translating tags to English..."; }

  virtual QString getCompletedStatusMessage() const override
  {
    return
      "Translated " + QString::number(_numTagTranslationsMade) + " tags to English on " +
      QString::number(_numProcessedElements) + " different elements";
  }

  virtual std::string getClassName() const { return className(); }

protected:

  std::shared_ptr<ToEnglishTranslator> _translatorClient;

  QSet<QString> _tagKeys;
  QString _toTranslateTagKey;
  ElementPtr _element;

  bool _ignorePreTranslatedTags;
  bool _parseNames;

  long _numTotalElements;
  long _numProcessedTags;
  long _numProcessedElements;

  QString _translatedText;

  /**
   * Translates a tag for an element
   *
   * @param e element containing the tag to translate
   * @param toTranslateTagKey the key of the tag whose value is to be translated
   * @return true if a successul translation was made; false otherwise
   */
  bool _translate(const ElementPtr& e, const QString& toTranslateTagKey);

private:

  QString _toTranslateVal;

  bool _currentElementHasSuccessfulTagTranslation;
  long _numTagTranslationsMade;
  long _numElementsWithSuccessfulTagTranslation;
  int _taskStatusUpdateInterval;
};

}

#endif // TO_ENGLISH_TRANSLATION_VISITOR_H
