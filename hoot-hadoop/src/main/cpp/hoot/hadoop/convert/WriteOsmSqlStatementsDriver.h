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

#ifndef WRITEOSMSQLSTATEMENTSDRIVER_H
#define WRITEOSMSQLSTATEMENTSDRIVER_H

// hoot
#include <hoot/hadoop/Driver.h>

// Standard
#include <string>

namespace hoot
{

using namespace std;

class WriteOsmSqlStatementsDriver : public Driver
{
public:

  WriteOsmSqlStatementsDriver();

  void write(const QString input, const QString output);

private:

  void _runElementSqlStatementsWriteJob(const string& input, const string& output);
  void _runChangesetSqlStatementsWriteJob(const string& input, const string& output);
};

}

#endif // WRITEOSMSQLSTATEMENTSDRIVER_H
