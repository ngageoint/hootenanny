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
#ifndef ADDIMPLICITLYDERIVEDTAGSPOIVISITOR_H
#define ADDIMPLICITLYDERIVEDTAGSPOIVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>

namespace hoot
{

/**
 * Adds tags implicitly derived from POI names to POIs
 */
class AddImplicitlyDerivedTagsPoiVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::AddImplicitlyDerivedTagsPoiVisitor"; }

  AddImplicitlyDerivedTagsPoiVisitor();
  AddImplicitlyDerivedTagsPoiVisitor(const QString databasePath);
  ~AddImplicitlyDerivedTagsPoiVisitor();

  /**
   * Adds implicitly derived tags to an element
   *
   * @param e element to add derived tags to
   */
  virtual void visit(const ElementPtr& e);

private:

  boost::shared_ptr<ImplicitTagRulesSqliteReader> _ruleReader;
  bool _tokenizeNames;

  long _numNodesModified;
  long _numTagsAdded;
  long _numNodesInvolvedInMultipleRules;
  long _numNodesParsed;
  long _statusUpdateInterval;

  QSet<QString> _getNameTokens(const Tags& t);

};

}

#endif // ADDIMPLICITLYDERIVEDTAGSPOIVISITOR_H
