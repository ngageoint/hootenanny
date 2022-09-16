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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#ifndef JOSM_CHANGESET_DERIVER_H
#define JOSM_CHANGESET_DERIVER_H

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>

namespace hoot
{

/**
 * @brief The JosmChangesetDeriver class calculates the changeset difference between a source and target
 * map that
 */
class JosmChangesetDeriver : public ChangesetDeriver
{

public:

  JosmChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to);

  ~JosmChangesetDeriver() = default;

private:

  virtual Change _nextChange();

};

using JosmChangesetDeriverPtr = std::shared_ptr<JosmChangesetDeriver>;

}

#endif // JOSM_CHANGESET_DERIVER_H
