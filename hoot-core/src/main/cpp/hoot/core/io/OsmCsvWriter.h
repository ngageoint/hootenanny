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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMCSVWRITER_H
#define OSMCSVWRITER_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/PartialOsmMapWriter.h>

// Boost
#include <boost/shared_ptr.hpp>

// Qt
#include <QFile>
#include <QTextStream>

// std
#include <vector>

namespace hoot
{

class OsmCsvWriter : public PartialOsmMapWriter, Configurable
{
public:
  static std::string className() { return "hoot::OsmCsvWriter"; }

  OsmCsvWriter();
  virtual ~OsmCsvWriter();

  virtual bool isSupported(QString url) { return url.toLower().endsWith(".csv"); }
  virtual QString supportedFormats() { return ".csv"; }

  virtual void open(QString url);

  void close();

  static QString toString(const ConstOsmMapPtr& map);

  virtual void write(ConstOsmMapPtr map);

  virtual void writePartial(const ConstNodePtr& n);
  virtual void writePartial(const ConstWayPtr& w);
  virtual void writePartial(const ConstRelationPtr& r);
  virtual void finalizePartial();

  virtual void setConfiguration(const Settings& conf);

private:
  void _initFiles();
  QString _getTags(const ConstWayPtr& w);

  int _precision;
  QString _separator;
  QString _endl;

  QTextStream _nodes;
  QTextStream _ways;
  QTextStream _waynodes;

  boost::shared_ptr<QFile> _n;
  boost::shared_ptr<QFile> _w;
  boost::shared_ptr<QFile> _wn;


};

}

#endif  //  OSMCSVWRITER_H
