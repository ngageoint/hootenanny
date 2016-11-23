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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.info;

import org.json.simple.JSONObject;

import hoot.services.nativeinterfaces.NativeInterface;
import hoot.services.nativeinterfaces.NativeInterfaceException;


public class NativeInterfaceStubImpl implements NativeInterface {
    @Override
    public JSONObject exec(JSONObject command) throws NativeInterfaceException {
        JSONObject json = new JSONObject();

        // hoot version --debug
        String coreVersionWithDebugInfo = "Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant\n"
                + "17:14:43.796 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  81) GEOS Version: 3.4.2\n"
                + "17:14:43.796 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  82) GDAL Version: 1.10.1\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  83) GLPK Version: 4.52\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  84) Qt Version: 4.8.6\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  88) Boost Version: 1.54.0\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  90) STXXL Version: STXXL v1.3.1 (release)\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  93) CppUnit Version: 1.13.1\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  96) Memory usage, vm: 402.3MB rss: 35.93MB\n";

        json.put("stdout", coreVersionWithDebugInfo);
        return json;
    }

    @Override
    public String getJobProgress(String jobId) {
        return null;
    }

    @Override
    public void terminate(String jobId) throws NativeInterfaceException {

    }
}
