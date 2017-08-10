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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PARTIALOSMMAPWRITER_H
#define PARTIALOSMMAPWRITER_H

#include "OsmMapWriter.h"
#include "ElementOutputStream.h"

namespace hoot
{

class ElementInputStream;

/**
 * Partial writing will be initialized by a call to OsmMapWriter::open. The partial writer should
 * be able to write out arbitrarily large data sets (AKA not memory bound).
 */
class PartialOsmMapWriter : public OsmMapWriter, public ElementOutputStream
{
public:

  PartialOsmMapWriter();

  virtual ~PartialOsmMapWriter() {}

  virtual void initializePartial() {}

  /**
   * Finalize the writing of partial data. All records should be flushed and any connections/files
   * should be closed when this is complete.
   */
  virtual void finalizePartial() = 0;

  /**
   * The default writes the map and then calls finalizePartial();
   */
  virtual void write(ConstOsmMapPtr map);

  /**
   * Write all the entries in the OsmMap to the output. This does not guarantee that all data will
   * be flushed to the output.
   *
   * The default write function writes nodes, ways, then relations.
   */
  virtual void writePartial(const boost::shared_ptr<const Element>& e);
  virtual void writePartial(const ConstOsmMapPtr& map);
  /**
   * These silly non-const overloads are here to placate the old compiler in RHEL 5.8.
   */
  void writePartial(const OsmMapPtr& map);

  virtual void writePartial(const ConstNodePtr& n) = 0;
  void writePartial(const NodePtr& n) { writePartial((const ConstNodePtr)n); }

  virtual void writePartial(const ConstWayPtr& w) = 0;
  void writePartial(const WayPtr& w) { writePartial((const ConstWayPtr)w); }

  virtual void writePartial(const ConstRelationPtr& r) = 0;
  void writePartial(const RelationPtr& r) { writePartial((const ConstRelationPtr)r); }

  virtual void writeElement(ElementPtr& element);

};

}

#endif // PARTIALOSMMAPWRITER_H
