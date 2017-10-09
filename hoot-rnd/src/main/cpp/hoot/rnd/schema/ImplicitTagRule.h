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
#ifndef IMPLICITTAGRULE_H
#define IMPLICITTAGRULE_H

// Hoot
#include <hoot/core/elements/Tags.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 *
 */
class ImplicitTagRule
{
public:

  ImplicitTagRule();

  QStringList& getWords() { return _words; }
  Tags& getTags() { return _tags; }

private:

  QStringList _words;
  Tags _tags;
};

typedef boost::shared_ptr<ImplicitTagRule> ImplicitTagRulePtr;
typedef QList<ImplicitTagRulePtr> ImplicitTagRules;
//key=<word>, value=<key=kvp, value=kvp occurrance count>>
typedef QMap<QString, QMap<QString, long> > ImplicitTagRulesByWord;

}

#endif // IMPLICITTAGRULE_H
