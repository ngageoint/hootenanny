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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TO_ENGLISH_TRANSLATE_STRING_DISTANCE_H
#define TO_ENGLISH_TRANSLATE_STRING_DISTANCE_H

// hoot
#include <hoot/core/algorithms/string/StringDistanceConsumer.h>
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * First, attempts to translate to English or transliterate the words and phrases in each input
 * string. Multiple variations on the translation may be derived in which case the best matching
 * translation will be used.
 */
class ToEnglishTranslateStringDistance : public StringDistance, public StringDistanceConsumer,
  public Configurable
{
public:

  static std::string className() { return "hoot::ToEnglishTranslateStringDistance"; }

  ToEnglishTranslateStringDistance();
  explicit ToEnglishTranslateStringDistance(const StringDistancePtr& d);
  ToEnglishTranslateStringDistance(const StringDistancePtr& d,
                                   const std::shared_ptr<ToEnglishTranslator>& translator);

  virtual ~ToEnglishTranslateStringDistance() = default;

  virtual void setStringDistance(const StringDistancePtr& sd) override { _d = sd; }

  /**
   * Returns a value from 1 (very similar) to 0 (very dissimilar) describing the distance between
   * two strings.
   */
  virtual double compare(const QString& s1, const QString& s2) const override;

  virtual void setConfiguration(const Settings& conf);

  virtual QString toString() const override { return "Translate " + _d->toString(); }

  void setTokenize(bool tokenize) { _tokenize = tokenize; }
  void setTranslateAll(bool translateAll) { _translateAll = translateAll; }

  virtual QString getDescription() const override
  { return "Returns a string comparison score based on the associated string comparator after first translating to English"; }

  virtual QString getName() const override { return QString::fromStdString(className()); }

private:

  StringDistancePtr _d;
  bool _tokenize;
  //if using the dict translator, try to get multiple translations for comparison scoring
  bool _translateAll;
  std::shared_ptr<ToEnglishTranslator> _translator;

  QStringList _getNamesToScore(const QString& name) const;
};

}

#endif // TO_ENGLISH_TRANSLATE_STRING_DISTANCE_H
