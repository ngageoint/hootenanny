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

#ifndef DUPLICATENAMEREMOVER_H
#define DUPLICATENAMEREMOVER_H

// Qt
#include <QString>

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

namespace hoot
{
class OsmMap;

/**
 * Searches for ways that contain the same name multiple times in the name and/or alt_name fields.
 * Any duplicates in the alt_name field will be removed.
 */
class DuplicateNameRemover : public OsmMapOperation, public Configurable,
  public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "hoot::DuplicateNameRemover"; }

  DuplicateNameRemover();
  ~DuplicateNameRemover() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Remove alternate names that are duplicates.
   */
  static void removeDuplicates(std::shared_ptr<OsmMap> map);

  void setConfiguration(const Settings& conf) override;

  void setCaseSensitive(bool caseSensitive) { _caseSensitive = caseSensitive; }

  QString getInitStatusMessage() const override { return "Removing duplicate name tags..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " duplicate name tags"; }

  QString getDescription() const override { return "Removes duplicate name tags from a feature"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

private:

  bool _caseSensitive;
  // If the feature has a name tag, enabling this will preserve the name tag value in the "name"
  // tag.  Otherwise, names and alternate names are treated equally.  Currently, this setting is
  // used by Attribute Conflation only, but possibly could be made the default behavior at some
  // point.
  bool _preserveOriginalName;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  std::shared_ptr<OsmMap> _map;

  QString _getBestName(const QString& n1, const QString& n2) const;
};

}

#endif // DUPLICATENAMEREMOVER_H
