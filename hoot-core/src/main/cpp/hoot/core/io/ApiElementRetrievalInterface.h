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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#ifndef API_ELELENT_RETRIEVAL_INTERFACE_H
#define API_ELELENT_RETRIEVAL_INTERFACE_H

//  Hootenanny
#include <hoot/core/elements/ElementId.h>

//  Qt
#include <QString>

//  Standard
#include <set>

namespace hoot
{

/**
 * @brief The ApiElementRetrievalInterface contains all element retrieval related functions for readers.
 */
class ApiElementRetrievalInterface
{
public:

  ApiElementRetrievalInterface();
  virtual ~ApiElementRetrievalInterface() = default;

  bool hasMoreQueries() const;
  QString getNextQuery(const QString& baseUrl);

  void setMissingElements(const std::set<ElementId>& elements);

protected:

  std::set<ElementId> _missingElements;
  std::set<ElementId>::iterator _current;

  bool _isOverpassQuery;

  /**  Allow test class to access protected members for white box testing */
//  friend class OverpassReaderInterfaceTest;
};

}

#endif  //  API_ELELENT_RETRIEVAL_INTERFACE_H
