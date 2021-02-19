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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.command.common;


import java.io.File;
import java.util.HashMap;
import java.util.Map;

import hoot.services.command.ExternalCommand;


public class ZIPDirectoryContentsCommand extends ExternalCommand {

    /**
     * Recursively compresses directory contents into a ZIP file using system 'zip' utility
     *
     * @param targetZIP ZIP file to produce
     * @param workDir work directory during execution
     * @param caller class that identifies caller of the command
     */
    public ZIPDirectoryContentsCommand(File targetZIP, File workDir, Class<?> caller) {
        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("TARGET_ZIP_FILE", targetZIP.getAbsolutePath());

        String command = "zip -r ${TARGET_ZIP_FILE} .";

        super.configureCommand(command, substitutionMap, caller, workDir, false);
    }
}
