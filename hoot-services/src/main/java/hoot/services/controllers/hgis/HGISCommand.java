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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.hgis;

import static hoot.services.HootProperties.CORE_SCRIPT_PATH;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.ExternalCommand;


class HGISCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(HGISCommand.class);

    HGISCommand(String sourceMap, String outputMap, String scriptName, Class<?> caller) {
        String source = HOOTAPI_DB_URL + "/" + sourceMap;
        String output = HOOTAPI_DB_URL + "/" + outputMap;
        String script = new File(CORE_SCRIPT_PATH, scriptName).getAbsolutePath();

        Map<String, String> substitutionMap = new HashMap<>();
        substitutionMap.put("SCRIPT", script);
        substitutionMap.put("SOURCE", source);
        substitutionMap.put("OUTPUT", output);

        String command = "${SCRIPT} ${SOURCE} ${OUTPUT}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
