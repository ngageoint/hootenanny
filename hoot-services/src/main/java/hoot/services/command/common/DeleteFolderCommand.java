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
import java.io.IOException;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;


/**
 * Deletes a folder
 */
public class DeleteFolderCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(DeleteFolderCommand.class);

    private final String jobId;
    private final File folderToDelete;

    public DeleteFolderCommand(String jobId, File folderToDelete) {
        this.jobId = jobId;
        this.folderToDelete = folderToDelete;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(this.jobId);

        try {
            FileUtils.forceDelete(this.folderToDelete);
            commandResult.setExitCode(CommandResult.SUCCESS);
        }
        catch(IOException ioe) {
            commandResult.setExitCode(CommandResult.FAILURE);
            logger.warn("Error deleting {} directory!", folderToDelete, ioe);
        }

        return commandResult;
    }
}
