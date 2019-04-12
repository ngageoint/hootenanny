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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.net.URL;
import java.time.LocalDateTime;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.geo.BoundingBox;

import javax.ws.rs.core.Response;
import javax.ws.rs.WebApplicationException;


/**
 * Used for pulling MapEdit data
 */
class PullApiCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullApiCommand.class);

    private final GrailParams params;
    private final String jobId;
    private final Class<?> caller;

    PullApiCommand(GrailParams params, String jobId, Class<?> caller) {
        this.params = params;
        this.jobId = jobId;
        this.caller = caller;

        logger.info("Params: "+ params.toString());

    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Pull OSM Data from Api DB] for " + params.getBounds());
        commandResult.setStart(LocalDateTime.now());
        commandResult.setCaller(caller.getName());

        getApiData();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    private void getApiData() {
        try {
            BoundingBox boundingBox = new BoundingBox(params.getBounds());
            double bboxArea = boundingBox.getArea();

            double maxBboxArea = params.getMaxBBoxSize();

            if (bboxArea > maxBboxArea) {
                throw new IllegalArgumentException("The bounding box area (" + bboxArea +
                        ") is too large. It must be less than " + maxBboxArea + " degrees");
            }

            URL requestUrl = new URL(replaceSensitiveData(params.getPullUrl()) +
                "/mapfull?bbox=" + boundingBox.toServicesString());

            File outputFile = new File(params.getOutput());

            FileUtils.copyURLToFile(requestUrl,outputFile, 10000, 10000);
            }
            catch (Exception ex) {
                String msg = "Failure to pull data from the OSM Api DB" + ex.getMessage();
                // throw new RuntimeException(msg, ex);
                throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
            }
    }
}
