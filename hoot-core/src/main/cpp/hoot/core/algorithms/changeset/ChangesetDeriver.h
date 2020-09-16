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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESETDERIVER_H
#define CHANGESETDERIVER_H

#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/elements/ElementComparer.h>

namespace hoot
{

/**
 * Calculates the changeset difference between a source and target map. This logic is based on
 * similar logic to that used in Osmosis: https://wiki.openstreetmap.org/wiki/Osmosis.
 */
class ChangesetDeriver : public ChangesetProvider
{

public:

  ChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to);

  /**
   * @see ChangeSetProvider
   */
  virtual std::shared_ptr<OGRSpatialReference> getProjection() const override;

  virtual ~ChangesetDeriver();

  /**
   * @see ChangeSetProvider
   */
  virtual void close();

  /**
   * @see ChangeSetProvider
   */
  virtual bool hasMoreChanges();

  /**
   * @see ChangeSetProvider
   */
  virtual Change readNextChange() override;

  long getNumFromElementsParsed() const { return _numFromElementsParsed; }
  long getNumToElementsParsed() const { return _numToElementsParsed; }

  int getNumCreateChanges() const { return _changesByType[Change::ChangeType::Create]; }
  int getNumModifyChanges() const { return _changesByType[Change::ChangeType::Modify]; }
  int getNumDeleteChanges() const { return _changesByType[Change::ChangeType::Delete]; }
  int getNumChanges() const
  { return getNumCreateChanges() + getNumModifyChanges() + getNumDeleteChanges(); }

  void setAllowDeletingReferenceFeatures(bool allow) { _allowDeletingReferenceFeatures = allow; }

private:

  friend class ChangesetReplacementCreatorTest;

  Change _nextChange();

  ElementInputStreamPtr _from;
  ElementInputStreamPtr _to;
  Change _next;
  ElementPtr _fromE, _toE;
  ElementComparer _elementComparer;

  long _numFromElementsParsed;
  long _numToElementsParsed;

  // Prevents any reference features from being deleted. This is useful for Differential Conflation
  // and can be used as a safety feature for other conflation workflows.
  bool _allowDeletingReferenceFeatures;
  QMap<Change::ChangeType, int> _changesByType;

  // list of metadata tag keys allowed to trigger a change
  QStringList _metadataAllowKeys;
};

typedef std::shared_ptr<ChangesetDeriver> ChangesetDeriverPtr;

}

#endif // CHANGESETDERIVER_H
