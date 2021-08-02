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
#ifndef OVERWRITETAGMERGER_H
#define OVERWRITETAGMERGER_H

// Hoot
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * Combine all names in a fashion where no unique names will be lost and then favor t1 tags over
 * t2 tags. See TagComparator::overwriteMerge.
 */
class OverwriteTagMerger : public TagMerger
{
public:

  static QString className() { return "hoot::OverwriteTagMerger"; }

  /**
   * If swap is set to true then t1 will be overwritten with t2 values.
   */
  OverwriteTagMerger(bool swap = false);
  virtual ~OverwriteTagMerger() = default;

  Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  // leave empty to avoid duplicate tag mergers displayed by the info command
  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void setConfiguration(const Settings& conf) override;

  void setOverwriteExcludeTagKeys(const QStringList& tagKeys)
  { _overwriteExcludeTagKeys = tagKeys; }
  void setAccumulateValuesTagKeys(const QStringList& tagKeys)
  { _accumulateValuesTagKeys = tagKeys; }

private:

  bool _swap;

  // keys of tags not to be overwritten
  QStringList _overwriteExcludeTagKeys;

  // Any tags being merged with matching keys will have values appended rather than overwritten in
  // the target.
  QStringList _accumulateValuesTagKeys;
};

/**
 * The same as OverwriteTagMerger with a more explicit name. Tag 2 will be overwritten with Tag 1
 * tags.
 */
class OverwriteTag2Merger : public OverwriteTagMerger
{
public:

  static QString className() { return "hoot::OverwriteTag2Merger"; }

  OverwriteTag2Merger() : OverwriteTagMerger(false) { }
  ~OverwriteTag2Merger() = default;

  QString getDescription() const override
  {  return "Overwrites conflicting tags with those from the reference feature"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

/**
 * Similar to OverwriteTagMerger, but tag 1 values will be overwritten with tag 2 values.
 */
class OverwriteTag1Merger : public OverwriteTagMerger
{
public:

  static QString className() { return "hoot::OverwriteTag1Merger"; }

  OverwriteTag1Merger() : OverwriteTagMerger(true) { }
  ~OverwriteTag1Merger() = default;

  QString getDescription() const override
  {  return "Overwrites conflicting tags with those from the secondary feature"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // OVERWRITETAGMERGER_H
