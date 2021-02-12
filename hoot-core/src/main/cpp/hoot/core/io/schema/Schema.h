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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef SCHEMA_H
#define SCHEMA_H

// Standard
#include <vector>
#include <map>

#include <hoot/core/io/schema/Layer.h>

namespace hoot
{

class Schema
{
public:
  Schema();

  void addLayer(const std::shared_ptr<Layer>& l);

  std::shared_ptr<const Layer> getLayer(size_t i) const;

  std::shared_ptr<const Layer> getLayer(const QString& name) const;

  bool hasLayer(const QString& name) const;

  size_t getLayerCount() const { return _layers.size(); }

private:
  std::vector<std::shared_ptr<Layer>> _layers;
  std::map<QString, size_t> _layerNameMap;
};

}

#endif // SCHEMA_H
