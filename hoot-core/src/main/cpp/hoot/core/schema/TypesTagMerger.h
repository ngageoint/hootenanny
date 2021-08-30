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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef TYPES_TAG_MERGER_H
#define TYPES_TAG_MERGER_H

// Hoot
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * @brief The TypesTagMerger class TODO
 */
class TypesTagMerger : public TagMerger
{
public:

  static QString ALT_TYPES_TAG_KEY;

  static QString className() { return "hoot::TypesTagMerger"; }

  /**
   * Constructor
   *
   * @param skipTagKeys optional; Any additional type tags found during merging with a key in this
   * list will be not be preserved.
   */
  TypesTagMerger(const QSet<QString>& skipTagKeys = QSet<QString>());
  ~TypesTagMerger() = default;

  /**
   * @see TagMerger
   */
  Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @brief setPreserveTypes TODO
   */
  virtual void setPreserveTypes() = 0;

  void setOverwrite1(bool overwrite) { _overwrite1 = overwrite; }
  void setSkipTagKeys(const QSet<QString>& keys) { _skipTagKeys = keys; }
  void setOverwriteExcludeTagKeys(const QStringList& overwriteExcludeTagKeys)
  { _overwriteExcludeTagKeys = overwriteExcludeTagKeys; }

protected:

  // TODO
  bool _preserveTypes;

private:

  // if true the first set of tags passed into mergeTags are overwritten; otherwise the second set
  // is overwritten
  bool _overwrite1;
  // any type tag that would otherwise be preserved will be skipped if in this list
  QSet<QString> _skipTagKeys;
  // keys of general tags not to be overwritten (see OverwriteTagMerger)
  QStringList _overwriteExcludeTagKeys;

  Tags _preserveAltTypes(const Tags& source, const Tags& target) const;
  // can probably eventually get rid of this by correcting logic that's duplicating tags in
  // mergeTags
  void _removeRedundantAltTypeTags(Tags& tags) const;

  bool _isAncestor(
    const QString& childKey, const QString& childVal, const QString& parentKey,
    const QString& parentVal) const;
};

}

#endif // TYPES_TAG_MERGER_H
