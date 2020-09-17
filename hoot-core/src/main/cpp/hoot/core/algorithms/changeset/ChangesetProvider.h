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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESETPROVIDER_H
#define CHANGESETPROVIDER_H

// GDAL
#include <ogr_spatialref.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/algorithms/changeset/Change.h>

namespace hoot
{

/**
 * Interface for classes implementing OSM changeset capabilities
 */
class ChangesetProvider
{

public:

  virtual std::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  /**
   * @brief ~ElementInputStream
   *
   * If the stream is open when the destructor is called, closeStream must be called in the
   * destructor
   */
  virtual ~ChangesetProvider() {}

  /**
   * @brief closeStream
   *
   * Releases all resources associated with the stream, if any
   */
  virtual void close() = 0;

  /**
   * Determines if the changeset has any unparsed changes
   *
   * @return true if the changeset has more changes; false otherwise
   */
  virtual bool hasMoreChanges() = 0;

  /**
   * Returns the next change in the changeset
   *
   * @return a changeset change
   */
  virtual Change readNextChange() = 0;

  // TODO: add doc

  virtual int getNumFromElementsParsed() const { return 0; }
  virtual int getNumToElementsParsed() const { return 0; }

  virtual int getNumCreateChanges() const { return 0; }
  virtual int getNumModifyChanges() const { return 0; }
  virtual int getNumDeleteChanges() const { return 0; }
  virtual int getNumChanges() const { return 0; }
};

typedef std::shared_ptr<ChangesetProvider> ChangesetProviderPtr;

}

#endif // CHANGESETPROVIDER_H
