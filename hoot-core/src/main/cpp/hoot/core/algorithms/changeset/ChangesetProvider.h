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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef CHANGESETPROVIDER_H
#define CHANGESETPROVIDER_H

// GDAL
#include <ogr_spatialref.h>

// hoot
#include <hoot/core/algorithms/changeset/Change.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * @brief The ChangesetProvider class is the interface for classes providing OSM changeset
 * capabilities.
 */
class ChangesetProvider
{

public:

  virtual std::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  /**
   * @brief ~ChangesetProvider Destructor
   * If the stream is open when the destructor is called, closeStream must be called in the
   * destructor
   */
  virtual ~ChangesetProvider() = default;

  /**
   * @brief close releases all resources associated with the stream, if any.
   */
  virtual void close() = 0;

  /**
   * @brief hasMoreChanges determines if the changeset has any unparsed changes.
   * @return true if the changeset has more changes; false otherwise
   */
  virtual bool hasMoreChanges() = 0;

  /**
   * @brief readNextChange returns the next change in the changeset.
   * @return a changeset change
   */
  virtual Change readNextChange() = 0;

  // Keeping these optional for now, as their implementation may be a little difficult and not
  // also really needed yet on some of the inheriting members.

  /**
   * @brief getNumFromElementsParsed returns the number of element parsed from the starting state
   * used to generate changes.
   * @return a number of elements
   */
  virtual int getNumFromElementsParsed() const { return 0; }

  /**
   * @brief getNumToElementsParsed returns the number of element parsed from the ending state used
   * to generate changes.
   * @return a number of elements
   */
  virtual int getNumToElementsParsed() const { return 0; }

  /**
   * @brief getNumCreateChanges returns the number of create changes in the changeset.
   * @return a number of changes
   */
  virtual int getNumCreateChanges() const { return 0; }

  /**
   * @brief getNumModifyChanges returns the number of modify changes in the changeset.
   * @return a number of changes
   */
  virtual int getNumModifyChanges() const { return 0; }

  /**
   * @brief getNumDeleteChanges returns the number of delete changes in the changeset.
   * @return a number of changes
   */
  virtual int getNumDeleteChanges() const { return 0; }

  /**
   * @brief getNumChanges returns the total number of changes in the changeset.
   * @return a number of changes
   */
  virtual int getNumChanges() const { return 0; }
};

using ChangesetProviderPtr = std::shared_ptr<ChangesetProvider>;

}

#endif // CHANGESETPROVIDER_H
