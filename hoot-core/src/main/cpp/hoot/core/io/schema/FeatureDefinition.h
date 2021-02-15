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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef FEATUREDEFINITION_H
#define FEATUREDEFINITION_H

// Qt
#include <QString>

// Standard
#include <memory>
#include <vector>

namespace hoot
{

class FieldDefinition;

class FeatureDefinition
{
public:
  FeatureDefinition();

  void addField(const std::shared_ptr<FieldDefinition>& fd) { _fields.push_back(fd); }

  bool hasField(const QString& name) const;

  size_t getFieldCount() const { return _fields.size(); }

  std::shared_ptr<const FieldDefinition> getFieldDefinition(size_t i) const { return _fields[i]; }

private:

  std::vector<std::shared_ptr<FieldDefinition>> _fields;
};

}

#endif // FEATUREDEFINITION_H
