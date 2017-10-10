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

// STXXL
//#include <stxxl/map>

namespace hoot
{

//static const int MAX_KEY_LEN = 16;

//class FixedString
//{
//public:

//  char charStr[MAX_KEY_LEN];

//  bool operator< (const FixedString& fixedString) const
//  {
//    return
//      std::lexicographical_compare(
//        charStr, charStr + MAX_KEY_LEN, fixedString.charStr, fixedString.charStr + MAX_KEY_LEN);
//  }

//  bool operator==(const FixedString& fixedString) const
//  {
//    return std::equal(charStr, charStr + MAX_KEY_LEN, fixedString.charStr);
//  }

//  bool operator!=(const FixedString& fixedString) const
//  {
//    return !std::equal(charStr, charStr + MAX_KEY_LEN, fixedString.charStr);
//  }
//};

//struct comp_type : public std::less<FixedString>
//{
//  static FixedString max_value()
//  {
//    FixedString s;
//    std::fill(s.charStr, s.charStr+MAX_KEY_LEN, 0x7f);
//    return s;
//  }
//};

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
typedef QList<ImplicitTagRulePtr> ImplicitTagRules; //*?
//key=<word>, value=<key=kvp, value=kvp occurrance count>>
typedef QMap<QString, QMap<QString, long> > ImplicitTagRulesByWord; //*
//typedef stxxl::map<QString, long, CompareGreater, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> map_type;
//typedef stxxl::map<FixedString, long, comp_type, 4096, 4096> fixed_name_map;

}

#endif // IMPLICITTAGRULE_H
