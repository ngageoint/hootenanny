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


// CPP Unit
#include <cppunit/plugin/TestPlugIn.h>

// The macro below has some unused parameters. This suppresses teh annoying warning
#pragma GCC diagnostic ignored "-Wunused-parameter"

// in addition to adding the plugin you must also make sure that the linking executable uses
// something in the shared library. (e.g. call a function or construct a class)
CPPUNIT_PLUGIN_IMPLEMENT();

