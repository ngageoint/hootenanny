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

import hoot.services.command.ExternalCommand;


public class ZIPDirectoryContentsCommand extends ExternalCommand {

    /**
     * Recursively compresses directory contents into a ZIP file using system's 'zip' utility
     *
     * @param targetZIP ZIP file to produce
     * @param workDir present working directory (PWD) during execution
     * @param caller class that identifies caller of the command
     */
    public ZIPDirectoryContentsCommand(File targetZIP, File workDir, Class<?> caller) {
        //zip -r targetZip .
        String command = "zip -r " + quote(targetZIP.getAbsolutePath()) + " " + ".";
        super.configureCommand(command, caller, workDir);
    }
}
