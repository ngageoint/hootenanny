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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENT_COUNTER_H
#define ELEMENT_COUNTER_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

class PartialOsmMapReader;

/**
 * Counts elements
 */
class ElementCounter
{
public:

  ElementCounter();
  ~ElementCounter() = default;

  /**
   * Counts the number of elements in inputs
   *
   * @param inputs paths to map inputs to count within
   * @return a count
   */
  long count(const QStringList& inputs);

  /**
   * @brief count counts the number of elements in a map
   * @param map the map containing elements to count
   * @return a count
   */
  long count(const OsmMapPtr& map) const;

  void setCountFeaturesOnly(const bool countFeaturesOnly)
  { _countFeaturesOnly = countFeaturesOnly; }
  void setCriteria(const QStringList& names);
  void setCriteria(const ElementCriterionPtr& criteria) { _crit = criteria; }

private:

  // if true, elements with no information tags are not counted
  bool _countFeaturesOnly;
  // optional filtering criteria
  ElementCriterionPtr _crit;
  // determines whether _crit supports streaming I/O
  bool _isStreamableCrit;
  // total number of elements counted
  long _total;

  int _taskStatusUpdateInterval;

  void _checkForMissingInputs(const QStringList& inputs) const;
  QString _getStreamingStatusMessage(const QString& input) const;
  QString _getMemoryBoundStatusMessage(const int inputsSize) const;
  QString _getMemoryBoundStatusMessage(const OsmMapPtr& map) const;

  std::shared_ptr<PartialOsmMapReader> _getStreamingReader(const QString& input) const;
  ElementInputStreamPtr _getFilteredInputStream(ElementInputStreamPtr inputStream, ConstElementVisitorPtr countVis) const;
  ConstElementVisitorPtr _getCountVis() const;

  long _countMemoryBound(const QStringList& inputs) const;
  long _countMemoryBound(const OsmMapPtr& map) const;
  long _countStreaming(const QString& input) const;
};

}

#endif // ELEMENT_COUNTER_H
