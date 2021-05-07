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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef CONFLATECMD_H
#define CONFLATECMD_H

// Hoot
#include <hoot/core/cmd/BoundedCommand.h>

// Qt
#include <QString>

namespace hoot
{

class SingleStat;

/**
 * Executes conflation
 */
class ConflateCmd : public BoundedCommand
{

public:

  static QString className() { return "hoot::ConflateCmd"; }

  ConflateCmd() = default;

  QString getName() const override { return "conflate"; }
  QString getDescription() const override { return "Conflates two maps into a single map"; }

  int runSimple(QStringList& args) override;

private:

  friend class TestUtils;
};

}

#endif // CONFLATECMD_H
