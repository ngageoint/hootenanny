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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef TRANSLATEDTAGDIFFERENCER_H
#define TRANSLATEDTAGDIFFERENCER_H

// hoot
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/schema/TagDifferencer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * The translated tag differencer calculate the difference between two sets of tags after they have
 * been translated.
 *
 * To calculate the difference the following steps are used:
 * - translate the tags using the provided script
 * - go through all the translated tags
 * -- if it is in the ignore list, move on
 * -- if same, increment same counter
 * -- if different, increment different counter
 *
 * Return difference as:
 * 1.0 - (same / (same + different));
 *
 * @sa translated.tag.differencer.script
 * @sa translated.tag.differencer.ignore.list
 */
class TranslatedTagDifferencer : public TagDifferencer, public Configurable
{
public:

  static QString className() { return "hoot::TranslatedTagDifferencer"; }

  TranslatedTagDifferencer();
  ~TranslatedTagDifferencer() = default;

  double diff(const ConstOsmMapPtr& map, const ConstElementPtr& e1,
    const ConstElementPtr& e2) const override;

  void setConfiguration(const Settings& conf) override;

private:
  QSet<QString> _ignoreList;
  QString _script;
  mutable std::shared_ptr<ScriptToOgrSchemaTranslator> _translator;

  class Comparison
  {
  public:
    Comparison() : same(0), different(0) {}
    int same;
    int different;
  };

  Comparison _compare(const Tags& t1, const Tags& t2) const;

  std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature> _translate(const ConstOsmMapPtr& map,
    const ConstElementPtr& in) const;

  /**
   * Does a lazy load of the translator to avoid initializing configuration options that aren't
   * being used.
   */
  std::shared_ptr<ScriptToOgrSchemaTranslator> _getTranslator() const;

  /**
   * Converts to tags if not-null otherwise returns an empty set of tags.
   */
  static Tags _toTags(const hoot::ScriptToOgrSchemaTranslator::TranslatedFeature* tf);
};

}

#endif // TRANSLATEDTAGDIFFERENCER_H
