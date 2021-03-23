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
#ifndef TRANSLITERATENAMEVISITOR_H
#define TRANSLITERATENAMEVISITOR_H

// Hoot
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

class TransliterateNameVisitor : public ElementVisitor
{
public:

  static QString className() { return "hoot::TransliterateNameVisitor"; }

  TransliterateNameVisitor() = default;
  ~TransliterateNameVisitor() = default;

  static bool isLatin(const QString& s);

  void visit(const std::shared_ptr<Element>& e) override;

  QString getDescription() const override { return "Transliterates names"; }

  QString getInitStatusMessage() const override { return "Transliterating names..."; }

  QString getCompletedStatusMessage() const override
  { return "Transliterated " + QString::number(_numAffected) + " names"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }
};

}

#endif // TRANSLITERATENAMEVISITOR_H
