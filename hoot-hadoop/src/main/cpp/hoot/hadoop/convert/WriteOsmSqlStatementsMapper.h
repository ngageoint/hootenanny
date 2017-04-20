/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef WRITEOSMSQLSTATEMENTSMAPPER_H
#define WRITEOSMSQLSTATEMENTSMAPPER_H

// Hoot
#include <hoot/hadoop/OsmMapMapper.h>

// Qt
#include <QVector>
#include <QPair>
#include <QStringList>

namespace hoot
{

class OsmMap;
class OsmApiDbSqlStatementFormatter;

/**
 * Mapreduce map task for converting OSM map data into executable OSM API database SQL statements.
 * Every entity is given a unique ID for the highest level of parallelism in the reducers.
 */
class WriteOsmSqlStatementsMapper : public OsmMapMapper
{
public:

  static std::string className() { return "hoot::WriteOsmSqlStatementsMapper"; }

  WriteOsmSqlStatementsMapper();
  virtual ~WriteOsmSqlStatementsMapper() {}

protected:

  virtual void _map(boost::shared_ptr<OsmMap>& map, HadoopPipes::MapContext& context);

private:

  HadoopPipes::MapContext* _context;
  boost::shared_ptr<OsmApiDbSqlStatementFormatter> _sqlFormatter;
  QString _outputDelimiter;
  boost::shared_ptr<QVector<QPair<QString, QString> > > _statementsBuffer;
  bool _localJobTracker;

  void _writeElementAndTagsSqlStatements(const ConstElementPtr& element,
                                         const unsigned long elementId,
                                         const QStringList& elementSqlHeaders,
                                         const QStringList& elementTagSqlHeaders);
  void _flush();

};

}

#endif // WRITEOSMSQLSTATEMENTSMAPPER_H
