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

#ifndef MISSING_ELEMENT_RETRIEVAL_OP_H
#define MISSING_ELEMENT_RETRIEVAL_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Progress.h>

// Qt
#include <QTextStream>

namespace hoot
{

/**
 * A
 */
class MissingElementRetrievalOp : public OsmMapOperation
{
public:

  static QString className() { return "MissingElementRetrievalOp"; }

  MissingElementRetrievalOp();
  ~MissingElementRetrievalOp() override = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getDescription() const override { return "Retrieve missing elements from the current map from an API"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setApiEndpointUrl(const QString& url);

private:
/*
  QString createOverpassQuery(const std::set<long>& missingNodes, const std::set<long>& missingWays, const std::set<long>& missingRelations) const;
  void writeOverpassIdString(QTextStream& s, const std::set<long>& missingIds, const QString& elementType) const;
*/
  QString _apiEndpoint;

  //  Allow test class to access private members for white box testing
  friend class MissingElementRetrievalOpTest;
};

}

#endif // MAPCLEANER_H
