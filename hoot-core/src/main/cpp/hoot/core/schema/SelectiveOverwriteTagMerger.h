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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef SELECTIVE_OVERWRITE_TAG_MERGER_H
#define SELECTIVE_OVERWRITE_TAG_MERGER_H

// Hoot
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * @brief The SelectiveOverwriteTagMerger class will transfer tags specified in a list from one
 * element to another. If no tags are specified, then all tags are transferred and the behavior is
 * the same as OverwriteTagMerger. Tags may also be excluded by key. Tag exclusions override
 * inclusions.
 */
class SelectiveOverwriteTagMerger : public TagMerger
{
public:

  static QString className() { return "SelectiveOverwriteTagMerger"; }

  /**
   * If swap is set to true then t1 will be overwritten with t2 values.
   */
  SelectiveOverwriteTagMerger(bool swap = false);
  ~SelectiveOverwriteTagMerger() override = default;

  Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  // leave empty to avoid duplicate tag mergers displayed by the info command
  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void setConfiguration(const Settings& conf) override;

  void setTagKeys(const QStringList& tagKeys) { _tagKeys = tagKeys; }
  void setTagExcludeKeys(const QStringList& tagKeys) { _tagKeysExclude = tagKeys; }

private:

  bool _swap;

  // list that determines which tags are transferred
  mutable QStringList _tagKeys;
  // exclude list that overrides _tagKeys
  QStringList _tagKeysExclude;
};

/**
 * @brief The SelectiveOverwriteTag2Merger class will transfer specified tags from element 1 to
 * element 2.
 *
 * See base class documentation for further details.
 */
class SelectiveOverwriteTag2Merger : public SelectiveOverwriteTagMerger
{
public:

  static QString className() { return "SelectiveOverwriteTag2Merger"; }

  SelectiveOverwriteTag2Merger() : SelectiveOverwriteTagMerger(false) { }
  ~SelectiveOverwriteTag2Merger() override = default;

  QString getDescription() const override
  {  return "Overwrites selected conflicting tags only with those from the reference feature"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

/**
 * @brief The SelectiveOverwriteTag1Merger class will transfer specified tags from element 2 to
 * element 1.
 *
 * See base class documentation for further details.
 */
class SelectiveOverwriteTag1Merger : public SelectiveOverwriteTagMerger
{
public:

  static QString className() { return "SelectiveOverwriteTag1Merger"; }

  SelectiveOverwriteTag1Merger() : SelectiveOverwriteTagMerger(true) { }
  ~SelectiveOverwriteTag1Merger() override = default;

  QString getDescription() const override
  {  return "Overwrites selected conflicting tags only with those from the secondary feature"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // SELECTIVE_OVERWRITE_TAG_MERGER_H
