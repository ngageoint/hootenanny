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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#ifndef STRINGS_H
#define STRINGS_H

namespace pp
{
// Keys that are accessed in the configuration file.
#define PP_MAPPER "pretty.pipes.mapper"
#define PP_REDUCER "pretty.pipes.reducer"
#define PP_COMBINER "pretty.pipes.combiner"
#define PP_PARTITIONER "pretty.pipes.partitioner"
#define PP_RECORD_READER "pretty.pipes.record.reader"
#define PP_RECORD_WRITER "pretty.pipes.record.writer"
#define PP_INPUT_FORMAT "pretty.pipes.input.format"
#define PP_OUTPUT_FORMAT "pretty.pipes.output.format"
#define PP_PLUGINS "pretty.pipes.plugins"
}

#endif // STRINGS_H
