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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef PRESERVETYPESTAGMERGER_H
#define PRESERVETYPESTAGMERGER_H

// Hoot
#include <hoot/core/schema/TypesTagMerger.h>

namespace hoot
{

/**
 * This is used to preserve types when merging multiple elements into one. e.g. merging multiple
 * POIs into a multi-use building. It keeps the most specific type when multiple types with the same
 * key are encountered. In the case where duplicated types have the same level of specificity, the
 * duplicated types are arbitrarily placed in an "alt_types" tag.
 */
class PreserveTypesTagMerger : public TypesTagMerger
{
public:

  static QString className() { return "PreserveTypesTagMerger"; }

  /**
   * Constructor
   *
   * @param skipTagKeys optional; Any additional type tags found during merging with a key in this
   * list will be not be preserved.
   */
  PreserveTypesTagMerger(const QSet<QString>& skipTagKeys = QSet<QString>());
  ~PreserveTypesTagMerger() override = default;

  /**
   * @see TypesTagMerger
   */
  void setPreserveTypes() override { _preserveTypes = true; }

  QString getDescription() const override
  { return "Keeps tags from both features and preserves overlapping type tags"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // PRESERVETYPESTAGMERGER_H
