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

#include "WriteOsmSqlStatementsMapper.h"

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>
#include <pp/conf/Configuration.h>

// Hoot
#include <hoot/hadoop/Debug.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

PP_FACTORY_REGISTER(pp::Mapper, WriteOsmSqlStatementsMapper)

WriteOsmSqlStatementsMapper::WriteOsmSqlStatementsMapper()
{

}

void WriteOsmSqlStatementsMapper::_map(shared_ptr<OsmMap>& /*map*/, HadoopPipes::MapContext& /*context*/)
{

}

}
