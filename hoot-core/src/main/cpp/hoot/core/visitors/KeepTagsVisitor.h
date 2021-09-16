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
#ifndef KEEPTAGSVISITOR_H
#define KEEPTAGSVISITOR_H

// hoot
#include <hoot/core/visitors/RemoveTagsVisitor.h>

namespace hoot
{

/**
 * Keeps any tags with keys matching those passed to this visitor; removes all others
 */
class KeepTagsVisitor : public RemoveTagsVisitor
{
public:

  static QString className() { return "KeepTagsVisitor"; }

  KeepTagsVisitor() = default;
  explicit KeepTagsVisitor(const QStringList& keys);
  ~KeepTagsVisitor() override = default;

  void visit(const std::shared_ptr<Element>& e) override;

  QString getDescription() const override { return "Keeps tags by key"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}


#endif // KEEPTAGSVISITOR_H
