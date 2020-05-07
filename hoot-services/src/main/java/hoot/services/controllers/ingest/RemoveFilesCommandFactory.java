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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.ingest;


import org.springframework.stereotype.Component;

import java.io.File;
import java.util.List;


/**
 * Used to create an internal command that can clean all the specific folders.
 * Useful because it can be added to a jobs workflow to cleanup after the files are used and unnecessary
 */
@Component
public class RemoveFilesCommandFactory {

    public RemoveFilesCommand build(String jobId, List<File> deleteFiles) {
        return new RemoveFilesCommand(jobId, deleteFiles);
    }
}
