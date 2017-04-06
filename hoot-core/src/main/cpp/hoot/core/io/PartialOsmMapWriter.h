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

  /**
   * Finalize the writing of partial data. All records should be flushed and any connections/files
   * should be closed when this is complete.
   */
  virtual void finalizePartial() = 0;

  /**
   * The default writes the map and then calls finalizePartial();
   */
  virtual void write(boost::shared_ptr<const OsmMap> map);

  /**
   * Write all the entries in the OsmMap to the output. This does not guarantee that all data will
   * be flushed to the output.
   *
   * The default write function writes nodes, ways, then relations.
   */
  virtual void writePartial(const boost::shared_ptr<const Element>& e);
  virtual void writePartial(const boost::shared_ptr<const OsmMap>& map);
  /**
   * These silly non-const overloads are here to placate the old compiler in RHEL 5.8.
   */
  void writePartial(const boost::shared_ptr<OsmMap>& map);

  virtual void writePartial(const boost::shared_ptr<const Node>& n) = 0;
  void writePartial(const boost::shared_ptr<Node>& n) { writePartial((const boost::shared_ptr<const Node>)n); }

  virtual void writePartial(const boost::shared_ptr<const Way>& w) = 0;
  void writePartial(const boost::shared_ptr<Way>& w) { writePartial((const boost::shared_ptr<const Way>)w); }

  virtual void writePartial(const boost::shared_ptr<const Relation>& r) = 0;
  void writePartial(const boost::shared_ptr<Relation>& r);

  virtual void writeElement(ElementInputStream& in);

  virtual void writeElement(ElementPtr& element);

};

}

#endif // PARTIALOSMMAPWRITER_H
