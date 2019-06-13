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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef METADATAOP_H
#define METADATAOP_H

// Hoot
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>
#include <QPair>

namespace hoot
{

class MetadataOp : public OsmMapOperation, public OperationStatusInfo, public Configurable
{
public:

  static std::string className() { return "hoot::MetadataOp"; }
  MetadataOp() : _pConf(&conf()) {}

  // OsmMapOperation (partial)
  virtual void apply(std::shared_ptr<OsmMap>& pMap) override
  {
    _pMap = pMap;
    _configure();
    _apply();
  }

  // OperationStatusInfo
  virtual QString getInitStatusMessage() const { return "Processing metadata..."; }
  virtual QString getCompletedStatusMessage() const { return "Modified " + QString::number(_numAffected) + " elements"; }

  // Configurable
  virtual void setConfiguration(const Settings& conf) {
    LOG_INFO( "MetadataOp setConfiguration");_pConf = &conf; }

protected:

  std::shared_ptr<OsmMap> _pMap;
  QPair<QString,QString> _datasetIndicator;
  QHash<QString,QString> _tags;

private:

  const Settings* _pConf;

  virtual void _apply() = 0;

  void _configure()
  {
    ConfigOptions opts = ConfigOptions(*_pConf);
    QStringList indicator = opts.getMetadataDatasetIndicatorTag();
    QStringList tags = opts.getMetadataTags();

    if (indicator.length() > 1)
    {
      _datasetIndicator.first = indicator[0];
      _datasetIndicator.second = indicator[1];
    }

    for (int i = 0; i < tags.length(); i+=2)
    {
      QString key = tags[i];
      QString value = (i < tags.length()-1) ? tags[i+1] : "";
      _tags[key] = value;
    }
  }
};

}

#endif // METADATAOP_H
