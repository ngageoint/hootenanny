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
#ifndef IMPLICITTYPETAGGERBASE_H
#define IMPLICITTYPETAGGERBASE_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/ImplicitTagRulesSqliteReader.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/language/ToEnglishTranslator.h>

namespace hoot
{

/**
 * Base class for implicit element type taggers
 */
class ImplicitTypeTaggerBase : public ElementVisitor, public Configurable
{
public:

  ImplicitTypeTaggerBase();
  ImplicitTypeTaggerBase(const QString& databasePath);
  ~ImplicitTypeTaggerBase();

  /**
   * Adds implicitly derived tags to an element
   *
   * @param e element to add derived tags to
   */
  virtual void visit(const ElementPtr& e);

  virtual void setConfiguration(const Settings& conf);
  void setTranslateNamesToEnglish(bool translate) { _translateNamesToEnglish = translate; }
  void setMatchEndOfNameSingleTokenFirst(bool match) { _matchEndOfNameSingleTokenFirst = match; }
  void setAllowTaggingSpecificFeatures(bool allow) { _allowTaggingSpecificFeatures = allow; }
  void setAddTopTagOnly(bool add) { _ruleReader->setAddTopTagOnly(add); }
  void setAllowWordsInvolvedInMultipleRules(bool allow)
  { _ruleReader->setAllowWordsInvolvedInMultipleRules(allow); }

  virtual std::string getClassName() const { return className(); }

protected:

  virtual bool _visitElement(const ElementPtr& e) = 0;

  QStringList _getNames(const Tags& tags) const;

  //allows for tagging features with specificity greater than poi=yes, building=yes, or area=yes
  bool _allowTaggingSpecificFeatures;
  bool _elementIsASpecificFeature;

  //reads implicit tag rules from the configured rules database
  std::shared_ptr<ImplicitTagRulesSqliteReader> _ruleReader;

private:

  friend class ImplicitPoiTypeTaggerTest;

  //number of features which received added tags
  long _numFeaturesModified;
  //sum of all tags added to all features
  long _numTagsAdded;
  //number of features which were involved in multiple tag rules and not modified if the option was
  //specified to not tag features involved in multiple tag rules
  long _numFeaturesInvolvedInMultipleRules;
  //total number of features processed
  long _numFeaturesParsed;
  long _statusUpdateInterval;
  //smallest number of tags added to any one feature
  long _smallestNumberOfTagsAdded;
  //largest number of tags added to any one feature
  long _largestNumberOfTagsAdded;
  //if true; all feature names are first translated to english before querying the rules database;
  //the value of this parameter should match the corresponding parameter used when the tag rules
  //were generated (see ImplicitTagRawRulesDeriver)
  bool _translateNamesToEnglish;
  //If true, the tagger will attempt to search for a rule match with the last token in a name first.
  //Setting to true can reduce the number of multiple rule involvements encountered.  e.g. "Police
  //Hospital" would involved rules for both a police station and a hospital if the setting was
  //false.  Otherwise it would match "Hospital" alone and correctly identify the feature as a
  //hospital.
  bool _matchEndOfNameSingleTokenFirst;
  //will be appended to the default list of names
  QStringList _additionalNameKeys;
  //names won't be considered when they are longer than this; very long names tend to be gibberish
  int _maxNameLength;

  //translates names to English
  std::shared_ptr<ToEnglishTranslator> _translator;

  QStringList _getNameTokens(const QStringList& names) const;
  QStringList _getTranslatedNames(const QStringList& names, const Tags& tags);
  QStringList _cleanNames(Tags& tags);
  QString _getEndOfNameToken(const QString& name, const QStringList& nameTokensList) const;

  void _addImplicitTags(const ElementPtr& e, const Tags& tagsToAdd,
                        const QSet<QString>& matchingWords);
  void _updateElementForDuplicateMatch(const ElementPtr& e, const QSet<QString>& matchingWords);
  /*
   * Ensure a tag with less specificity than tags the element already contained are not added.
   */
  void _ensureCorrectTagSpecificity(const ElementPtr& e, Tags& tagsToAdd);

  void _getImplicitlyDerivedTagsFromMultipleNameTokens(
    const QStringList& names, const QStringList& nameTokensList, const Tags& elementTags,
    Tags& implicitlyDerivedTags, QSet<QString>& matchingWords, bool& wordsInvolvedInMultipleRules);

  void _getImplicitlyDerivedTagsFromSingleNameTokens(const QStringList& names,
    QStringList& nameTokensList, const Tags& elementTags, Tags& implicitlyDerivedTags,
    QSet<QString>& matchingWords, bool& wordsInvolvedInMultipleRules, bool& namesContainBuilding,
    bool& namesContainOffice);

  Tags _applyCustomRules(const ElementPtr& e, const QStringList& filteredNames);
};

}

#endif // IMPLICITTYPETAGGERBASE_H
