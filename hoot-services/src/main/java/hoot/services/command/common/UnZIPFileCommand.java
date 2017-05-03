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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.command.common;


import java.io.File;
import java.util.HashMap;
import java.util.Map;

import hoot.services.command.ExternalCommand;


public class UnZIPFileCommand extends ExternalCommand {

    /**
     * Decompresses a ZIP file using system 'unzip' utility
     *
     * @param sourceZIP ZIP file to decompress
     * @param targetFolder directory where to store the contents of the ZIP
     * @param caller class that identifies caller of the command
     */
    public UnZIPFileCommand(File sourceZIP, File targetFolder, Class<?> caller) {
        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("SOURCE_ZIP_FILE", sourceZIP.getAbsolutePath());
        substitutionMap.put("TARGET_FOLDER", targetFolder.getAbsolutePath());

        String command = "unzip ${SOURCE_ZIP_FILE} -d ${TARGET_FOLDER}";

        super.configureCommand(command, substitutionMap, caller, false);
    }
}
