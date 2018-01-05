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
#ifndef IMPLICITTAGGERBASE_H
#define IMPLICITTAGGERBASE_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/ImplicitTagRulesSqliteReader.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Adds tags implicitly derived from POI names to POIs
 *
 * Relevant configuration options are those beginning with implicit.tagger.* in
 * conf/core/ConfigOptions.asciidoc.
 */
class ImplicitTaggerBase : public ElementVisitor, public Configurable
{
public:

  ImplicitTaggerBase();
  ImplicitTaggerBase(const QString databasePath);
  ~ImplicitTaggerBase();

  /**
   * Adds implicitly derived tags to an element
   *
   * @param e element to add derived tags to
   */
  virtual void visit(const ElementPtr& e);

  virtual void setConfiguration(const Settings& conf);

  void setTranslateAllNamesToEnglish(bool translate) { _translateAllNamesToEnglish = translate; }
  void setMatchEndOfNameSingleTokenFirst(bool match) { _matchEndOfNameSingleTokenFirst = match; }
  void setAllowTaggingSpecificPois(bool allow) { _allowTaggingSpecificPois = allow; }
  void setAddTopTagOnly(bool add) { _ruleReader->setAddTopTagOnly(add); }
  void setAllowWordsInvolvedInMultipleRules(bool allow)
  { _ruleReader->setAllowWordsInvolvedInMultipleRules(allow); }

protected:

  virtual bool _visitElement(const ElementPtr& e) = 0;

  bool _allowTaggingSpecificPois;
  bool _elementIsASpecificPoi;

  boost::shared_ptr<ImplicitTagRulesSqliteReader> _ruleReader;

private:

  long _numNodesModified;
  long _numTagsAdded;
  long _numNodesInvolvedInMultipleRules;
  long _numNodesParsed;
  long _statusUpdateInterval;
  long _smallestNumberOfTagsAdded;
  long _largestNumberOfTagsAdded;
  bool _translateAllNamesToEnglish;
  bool _matchEndOfNameSingleTokenFirst;

  QStringList _getNameTokens(const QStringList names);

};

}

#endif // IMPLICITTAGGERBASE_H
