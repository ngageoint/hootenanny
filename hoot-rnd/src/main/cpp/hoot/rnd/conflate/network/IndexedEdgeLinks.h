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
#ifndef INDEXEDEDGELINKS_H
#define INDEXEDEDGELINKS_H

#include "EdgeMatch.h"

namespace hoot
{

/**
 * links in the form {from, [to]}
 *
 * To make the terminology a little more precise, the term link will refer to a link from one
 * pair of edges to another pair of edges.
 */
class IndexedEdgeLinks : public QMultiHash<ConstEdgeMatchPtr, ConstEdgeMatchPtr>
{
public:
  IndexedEdgeLinks();
};

typedef shared_ptr<IndexedEdgeLinks> IndexedEdgeLinksPtr;
typedef shared_ptr<const IndexedEdgeLinks> ConstIndexedEdgeLinksPtr;

// not implemented
bool operator<(ConstIndexedEdgeLinksPtr, ConstIndexedEdgeLinksPtr);

}

#endif // INDEXEDEDGELINKS_H
