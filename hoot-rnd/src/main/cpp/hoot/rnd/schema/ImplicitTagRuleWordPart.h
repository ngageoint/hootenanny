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
#ifndef IMPLICITTAGRULEWORDPART_H
#define IMPLICITTAGRULEWORDPART_H

// Qt
#include <QMap>

namespace hoot
{

/**
 * A rule that can be applied to add tags to a feature derived implicitly from the feature's name
 */
class ImplicitTagRuleWordPart
{
public:

  ImplicitTagRuleWordPart(const QString word, const QMap<QString, long>& tagsToCounts);

  /**
   * Returns the word associated with this rule part
   *
   * @return a word
   */
  QString getWord() const { return _word; }
  void setWord(const QString word) { _word = word; }

  /**
   * Returns all tags associated with this rule part
   *
   * @return a collection of tags
   */
  QMap<QString, long> getTagsToCounts() const { return _tagsToCounts; }
  void setTagsToCounts(const QMap<QString, long>& tagsToCounts) { _tagsToCounts = tagsToCounts; }

  bool operator<(const ImplicitTagRuleWordPart& part) const;

private:

  QString _word;
  QMap<QString, long> _tagsToCounts;
};

typedef boost::shared_ptr<ImplicitTagRuleWordPart> ImplicitTagRuleWordPartPtr;
typedef QList<ImplicitTagRuleWordPartPtr> ImplicitTagRuleWordParts;

}

#endif // IMPLICITTAGRULEWORDPART_H
