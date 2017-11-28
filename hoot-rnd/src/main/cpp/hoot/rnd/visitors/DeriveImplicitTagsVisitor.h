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
#ifndef DERIVEIMPLICITTAGSVISITOR_H
#define DERIVEIMPLICITTAGSVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{
class ImplicitRule;

/**
 * Derive tags based on the names.
 *
 * OLD VERSION - to be replaced by AddImplicitlyDerivedTagsPoiVisitor
 */
class DeriveImplicitTagsVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::DeriveImplicitTagsVisitor"; }

  DeriveImplicitTagsVisitor();

  virtual void visit(const ElementPtr& e);

public:

  QList< boost::shared_ptr<ImplicitRule> > _rules;

  /**
   * Ensure all rules are in lower case.
   */
  void _rulesToLower();

  /**
   * Extract the names from tags and then tokenize the names into a set of words.
   */
  QSet<QString> _extractNameWords(const Tags& t);
};

}

#endif // DERIVEIMPLICITTAGSVISITOR_H
