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
#ifndef MOST_SPECIFIC_TAG_MERGER_H
#define MOST_SPECIFIC_TAG_MERGER_H

// Hoot
#include <hoot/core/schema/TypesTagMerger.h>

namespace hoot
{

/**
 * @brief The MostSpecificTagMerger class merges tags by keeping the most specifically typed tags.
 */
class MostSpecificTagMerger : public TypesTagMerger
{
public:

  static QString className() { return "MostSpecificTagMerger"; }

  /**
   * Constructor
   *
   * @param skipTagKeys optional; Any additional type tags found during merging with a key in this
   * list will be not be preserved.
   */
  MostSpecificTagMerger(const QSet<QString>& skipTagKeys = QSet<QString>());
  ~MostSpecificTagMerger() = default;

  /**
   * @see TypesTagMerger
   */
  void setPreserveTypes() override { _preserveTypes = false; }

  QString getDescription() const override
  { return "Keeps the most specific tag between two features"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // MOST_SPECIFIC_TAG_MERGER_H
