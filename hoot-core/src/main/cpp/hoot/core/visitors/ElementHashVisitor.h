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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENT_HASH_VISITOR_H
#define ELEMENT_HASH_VISITOR_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

/**
 * Calculates a hash for each element and optionally stores the hash in the MetadataTags::HootHash()
 * tag and or collects it for later use. Any existing hash tag value will be overwritten.
 *
 * See this for details: https://github.com/ngageoint/hootenanny/issues/1663
 *
 * We want to keep ID's out of this, so not using GeoJsonWriter. Although, possibly we could add
 * a switch to GeoJsonWriter to not write ID's and use it at some point instead of this.
 *
 * @todo implement OperationStatus
 */
class ElementHashVisitor : public ElementOsmMapVisitor
{
public:

  static QString className() { return "hoot::ElementHashVisitor"; }

  ElementHashVisitor();
  virtual ~ElementHashVisitor() = default;

  virtual void visit(const ElementPtr& e);

  /**
   * Inserts a hash tag into an element and/or records the hash mapping with the element for later
   * use.
   *
   * @param element element whose hash to record
   * @param hash a unique hash value for the element
   */
  void insertHash(const ElementPtr& element, const QString& hash);

  QString toJson(const ConstElementPtr& e) const;
  virtual QString toJson(const Tags& tags, const double ce = -1.0) const;

  QByteArray toHash(const ConstElementPtr& e) const;
  QByteArray toHash(const Tags& tags, const double ce = -1.0) const;
  QString toHashString(const ConstElementPtr& e) const;
  QString toHashString(const Tags& tags, const double ce = -1.0) const;

  void clearHashes();

  virtual QString getDescription() const { return "Calculates unique hash values for elements"; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

  QMap<QString, ElementId> getHashesToElementIds() const { return _hashesToElementIds; }
  QMap<ElementId, QString> getElementIdsToHashes() const { return _elementIdsToHashes; }
  QSet<std::pair<ElementId, ElementId>> getDuplicates() const { return _duplicates; }

  void setCoordinateComparisonSensitivity(int sensitivity)
  { _coordinateComparisonSensitivity = sensitivity; }
  void setIncludeCircularError(bool include) { _includeCe = include; }
  void setUseNodeTags(bool use) { _useNodeTags = use; }
  void setWriteHashes(bool write) { _writeHashes = write; }
  void setCollectHashes(bool collect) { _collectHashes = collect; }

protected:

  virtual QString _toJson(const ConstNodePtr& node) const;
  virtual QString _toJson(const ConstWayPtr& way) const;
  virtual QString _toJson(const ConstRelationPtr& relation) const;

private:

  // node comparison decimal place sensitivity
  int _coordinateComparisonSensitivity;
  // determines if element circular error will be used in computation of the hash
  bool _includeCe;
  // an optional list of metadata tag keys to factor into the comparison; by default no metadata
  // keys are examined during comparison
  QStringList _nonMetadataIgnoreKeys;
  // if enabled, node tags will factor into node comparisons
  bool _useNodeTags;

  // determines whether hashes are written to an element's tags
  bool _writeHashes;
  // determines if hash values should be collected for post-processing purposes
  bool _collectHashes;

  // collected hash values mapped to element IDs
  QMap<QString, ElementId> _hashesToElementIds; // TODO: make this QHash?
  // collected element IDs mapped to hash values
  QMap<ElementId, QString> _elementIdsToHashes;
  // pairings of all duplicate elements found
  QSet<std::pair<ElementId, ElementId>> _duplicates;
};

}

#endif // ELEMENT_HASH_VISITOR_H
