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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ChangesetStats.h"

namespace hoot
{

ChangesetStats::ChangesetStats()
{
  _stats.clear();
  _stats["node-create"] = 0;
  _stats["node-modify"] = 0;
  _stats["node-delete"] = 0;
  _stats["way-create"] = 0;
  _stats["way-modify"] = 0;
  _stats["way-delete"] = 0;
  _stats["relation-create"] = 0;
  _stats["relation-modify"] = 0;
  _stats["relation-delete"] = 0;
}

QString ChangesetStats::toString() const
{
  return
    "Node(s) Created: " + QString::number(_stats["node-create"]) + "\n" +
    "Node(s) Modified: " + QString::number(_stats["node-modify"]) + "\n" +
    "Node(s) Deleted: " + QString::number(_stats["node-delete"]) + "\n" +
    "Way(s) Created: " + QString::number(_stats["way-create"]) + "\n" +
    "Way(s) Modified: " + QString::number(_stats["way-modify"]) + "\n" +
    "Way(s) Deleted: " + QString::number(_stats["way-delete"]) + "\n" +
    "Relation(s) Created: " + QString::number(_stats["relation-create"]) + "\n" +
    "Relation(s) Modified: " + QString::number(_stats["relation-modify"]) + "\n" +
    "Relation(s) Deleted: " + QString::number(_stats["relation-delete"]) + "\n";
}

}
