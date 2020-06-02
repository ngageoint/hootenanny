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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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

  static std::string className() { return "hoot::OverwriteTagMerger"; }

  /**
   * If swap is set to true then t1 will be overwritten with t2 values.
   */
  OverwriteTagMerger(bool swap = false);

  virtual ~OverwriteTagMerger() {}

  virtual Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  // leave empty to avoid duplicate tag mergers displayed by the info command
  virtual QString getDescription() const { return ""; }
  virtual QString getClassName() const { return ""; }

  virtual void setConfiguration(const Settings& conf);

  void setOverwriteExcludeTagKeys(const QStringList& overwriteExcludeTagKeys)
  { _overwriteExcludeTagKeys = overwriteExcludeTagKeys; }

protected:

  bool _swap;
  // keys of tags not to be overwritten
  QStringList _overwriteExcludeTagKeys;
};

/**
 * The same as OverwriteTagMerger with a more explicit name. Tag 2 will be overwritten with Tag 1
 * tags.
 */
class OverwriteTag2Merger : public OverwriteTagMerger
{
public:

  static std::string className() { return "hoot::OverwriteTag2Merger"; }

  OverwriteTag2Merger() : OverwriteTagMerger(false) {}

  virtual QString getDescription() const
  {  return "Overwrites conflicting tags with those from the reference feature"; }

  virtual QString getClassName() const { return QString::fromStdString(className()); }
};

/**
 * Similar to OverwriteTagMerger, but tag 1 values will be overwritten with tag 2 values.
 */
class OverwriteTag1Merger : public OverwriteTagMerger
{
public:

  static std::string className() { return "hoot::OverwriteTag1Merger"; }

  OverwriteTag1Merger() : OverwriteTagMerger(true) {}

  virtual QString getDescription() const
  {  return "Overwrites conflicting tags with those from the secondary feature"; }

  virtual QString getClassName() const { return QString::fromStdString(className()); }
};

}

#endif // OVERWRITETAGMERGER_H
