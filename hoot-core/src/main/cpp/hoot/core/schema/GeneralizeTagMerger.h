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
#ifndef GENERALIZETAGMERGER_H
#define GENERALIZETAGMERGER_H

#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

class GeneralizeTagMerger : public TagMerger
{
public:

  static QString className() { return "GeneralizeTagMerger"; }

  GeneralizeTagMerger() = default;
  ~GeneralizeTagMerger() = default;

  Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  QString getDescription() const override
  { return "Keeps tags from both features and overlapping tags are generalized to a common parent"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }
};

}

#endif // GENERALIZETAGMERGER_H
