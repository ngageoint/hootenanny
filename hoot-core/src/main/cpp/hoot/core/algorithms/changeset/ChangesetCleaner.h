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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_CLEANER_H
#define CHANGESET_CLEANER_H

#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>

namespace hoot
{

/**
 * TODO
 */
class ChangesetCleaner : public ChangesetProvider
{

public:

  ChangesetCleaner(ChangesetProviderPtr from, ElementInputStreamPtr to) {}

  /**
   * @see ChangeSetProvider
   */
  virtual std::shared_ptr<OGRSpatialReference> getProjection() const override;

  virtual ~ChangesetDeriver();

  /**
   * @see ChangeSetProvider
   */
  virtual void close(){}

  /**
   * @see ChangeSetProvider
   */
  virtual bool hasMoreChanges(){ return false; }

  /**
   * @see ChangeSetProvider
   */
  virtual Change readNextChange() override{ return Change(); }

//  long getNumFromElementsParsed() const { return _numFromElementsParsed; }
//  long getNumToElementsParsed() const { return _numToElementsParsed; }

//  int getNumCreateChanges() const { return _changesByType[Change::ChangeType::Create]; }
//  int getNumModifyChanges() const { return _changesByType[Change::ChangeType::Modify]; }
//  int getNumDeleteChanges() const { return _changesByType[Change::ChangeType::Delete]; }
//  int getNumChanges() const
//  { return getNumCreateChanges() + getNumModifyChanges() + getNumDeleteChanges(); }

private:

  //Change _nextChange();

  ChangesetProviderPtr _from;
  ElementInputStreamPtr _to;
  Change _next;
  ElementPtr _fromE, _toE;

//  long _numFromElementsParsed;
//  long _numToElementsParsed;

  QMap<Change::ChangeType, int> _changesByType;
};

}

#endif // CHANGESET_CLEANER_H
