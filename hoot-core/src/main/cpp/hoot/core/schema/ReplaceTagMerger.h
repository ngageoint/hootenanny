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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REPLACE_TAG_MERGER_H
#define REPLACE_TAG_MERGER_H

// Hoot
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * Completely replaces tags in a feature
 */
class ReplaceTagMerger : public TagMerger
{
public:

  static QString className() { return "hoot::ReplaceTagMerger"; }

  /**
   * If swap is set to true then t1 will be overwritten with t2 values.
   */
  ReplaceTagMerger(bool swap = false);
  virtual ~ReplaceTagMerger() = default;

  Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  // leave empty to avoid duplicate tag mergers displayed by the info command
  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void setConfiguration(const Settings& conf) override;

  void setOverwriteExcludeTagKeys(const QStringList& overwriteExcludeTagKeys)
  { _overwriteExcludeTagKeys = overwriteExcludeTagKeys; }

private:

  bool _swap;
  // keys of tags not to be overwritten
  QStringList _overwriteExcludeTagKeys;
};

/**
 * The same as ReplaceTagMerger with a more explicit name. Tags 2 will be completely replaced with
 * tags 1.
 */
class ReplaceTag2Merger : public ReplaceTagMerger
{
public:

  static QString className() { return "hoot::ReplaceTag2Merger"; }

  ReplaceTag2Merger() : ReplaceTagMerger(false) { }
  ~ReplaceTag2Merger() = default;

  QString getDescription() const override
  {
    return
      "Completely replaces tags in the secondary feature with those from the reference feature";
  }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

/**
 * The same as ReplaceTagMerger with a more explicit name. Tags 1 will be completely replaced with
 * tags 2.
 */
class ReplaceTag1Merger : public ReplaceTagMerger
{
public:

  static QString className() { return "hoot::ReplaceTag1Merger"; }

  ReplaceTag1Merger() : ReplaceTagMerger(true) { }
  ~ReplaceTag1Merger() = default;

  QString getDescription() const override
  {
    return
      "Completely replaces tags in the reference feature with those from the secondary feature";
  }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // REPLACE_TAG_MERGER_H
