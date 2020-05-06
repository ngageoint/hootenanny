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

#ifndef TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
#define TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H

// hoot
#include <hoot/core/visitors/ToEnglishTranslationVisitor.h>
#include <hoot/core/algorithms/string/StringDistance.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * This is used to check hoot tag English translations against existing known (assumed)
 * correct translations; e.g. compare hoot's English translation of a tag to that of name:en
 */
class ToEnglishTranslationComparisonVisitor : public ToEnglishTranslationVisitor
{

public:

  static std::string className() { return "hoot::ToEnglishTranslationComparisonVisitor"; }

  ToEnglishTranslationComparisonVisitor();

  virtual void visit(const std::shared_ptr<Element>& e) override;

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const override
  { return "Translates selected tags to English and scores them against known translations"; }

  virtual QString getInitStatusMessage() const
  { return "Comparing to English tag translations..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Compared " + QString::number(_numProcessedTags) + " to English tag translations on " +
      QString::number(_numProcessedElements) + " different elements";
  }

  virtual std::string getClassName() const { return className(); }

private:

  //scores the similarity of the pre-translated and translated texts
  StringDistancePtr _translationScorer;

  QStringList _tagKeysAsList;
  QStringList _preTranslatedTagKeys;
  QString _preTranslatedVal;
  long _numTagsCompared;
};

}

#endif // TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
