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
package hoot.services.command;


import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.json.simple.JSONObject;

import hoot.services.HootProperties;


public class ExternalCommand extends JSONObject {

    protected void configureCommand(String command, Map<String, ?> substitutionMap, Class<?> caller) {
        this.put("caller", caller.getName());
        this.put("command", command);
        this.put("substitutionMap", substitutionMap);
        this.put("workDir", new File(HootProperties.TEMP_OUTPUT_PATH));
        this.put("trackable", Boolean.TRUE);
    }

    protected void configureCommand(String command, Map<String, ?> substitutionMap, Class<?> caller, File workDir) {
        this.put("caller", caller.getName());
        this.put("command", command);
        this.put("substitutionMap", substitutionMap);
        this.put("workDir", workDir);
        this.put("trackable", Boolean.TRUE);
    }

    protected void configureCommand(String command, Class<?> caller, Boolean trackable) {
        this.configureCommand(command, new HashMap<>(), caller);
        this.put("trackable", trackable);
    }

    protected void configureCommand(String command, Class<?> caller, File workDir, Boolean trackable) {
        this.configureCommand(command, new HashMap<>(), caller, workDir);
        this.put("trackable", trackable);
    }

    protected static String quote(String commandOption) {
        return "\"" + commandOption + "\"";
    }

    protected static String hootOptionsToString(List<String> hootOptions) {
        return hootOptions.stream().map(option -> "-D " + option).collect(Collectors.joining(" "));
    }
}
