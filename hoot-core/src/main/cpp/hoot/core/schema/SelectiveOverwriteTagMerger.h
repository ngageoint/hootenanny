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
#ifndef SELECTIVE_OVERWRITE_TAG_MERGER_H
#define SELECTIVE_OVERWRITE_TAG_MERGER_H

// Hoot
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * @brief The SelectiveOverwriteTagMerger class will only transfer tags specified in a list from on
 * element to another.
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

private:

  bool _swap;

  // list that determines which tags are transferred
  QStringList _tagKeys;
};

/**
 * @brief The SelectiveOverwriteTag2Merger class will only transfer tags specified in a list from
 * element 1 to element 2.
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
 * @brief The SelectiveOverwriteTag1Merger class will only transfer tags specified in a list from
 * element 2 to element 1.
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
