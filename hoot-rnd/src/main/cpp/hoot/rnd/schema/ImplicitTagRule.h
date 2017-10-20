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
#include <QSet>

namespace hoot
{

/**
 * A rule that can be applied to add tags to a feature derived implicitly from the feature's name
 */
class ImplicitTagRule
{
public:

  ImplicitTagRule();

  /**
   * Returns all words associated with this rule
   *
   * @return a collection of words
   */
  QSet<QString>& getWords() { return _words; }
  void setWords(const QSet<QString>& words) { _words = words; }

  /**
   * Returns all tags associated with this rule
   *
   * @return a collection of tags
   */
  Tags& getTags() { return _tags; }
  void setTags(const Tags& tags) { _tags = tags; }

private:

  QSet<QString> _words;
  Tags _tags;
};

typedef boost::shared_ptr<ImplicitTagRule> ImplicitTagRulePtr;
typedef QList<ImplicitTagRulePtr> ImplicitTagRules;

}

#endif // IMPLICITTAGRULE_H
