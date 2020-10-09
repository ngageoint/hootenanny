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
package hoot.services.controllers.grail;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.time.Duration;
import java.time.LocalDateTime;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.ws.rs.NotFoundException;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import hoot.services.command.CommandResult;
import hoot.services.utils.DbUtils;

import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PATH;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_URL;
import static hoot.services.HootProperties.replaceSensitiveData;


/**
 * Used for pushing overpass data to the database
 */
class WaitOverpassUpdate extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(WaitOverpassUpdate.class);
    private final String jobId;
    private final GrailParams params;
    private final Class<?> caller;
    private final int TIMEOUT_TIME = 300000;
    private final int SLEEP_TIME = 30000;

    WaitOverpassUpdate(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);
        logger.info("Params: " + params);

        this.jobId = jobId;
        this.params = params;
        this.caller = caller;
    }

    @Override
    public CommandResult execute() {
        LocalDateTime startTime = LocalDateTime.now();
        CommandResult commandResultStart = new CommandResult();
        commandResultStart.setStart(startTime);
        commandResultStart.setCommand("Overpass sync wait");
        commandResultStart.setJobId(jobId);
        commandResultStart.setStdout("Starting wait on overpass sync...\n");
        commandResultStart.setStderr("");
        commandResultStart.setPercentProgress(100);
        commandResultStart.setCaller(caller.getName());
        commandResultStart.setExitCode(CommandResult.SUCCESS);
        commandResultStart.setFinish(startTime);
        DbUtils.upsertCommandStatus(commandResultStart);
        DbUtils.completeCommandStatus(commandResultStart);

        if (GrailResource.isPrivateOverpassActive()) {
            long timeSpent = 0;
            String url = null;
            boolean foundChangesetId = false;

            String lastId = DbUtils.getLastPushedId(jobId);
            if (lastId == null) {
                String msg = "No changeset id found. No changeset was pushed or timed out job may have been deleted.";
                throw new WebApplicationException(new NotFoundException(), Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
            }

            LocalDateTime jobStartTime = DbUtils.getJobStartDate(jobId); // check if changeset id exists in overpass since the time the job was initially run
            String time = jobStartTime.minusMinutes(2).toString(); // get 2 minutes before just for a little leeway
            String query = "[out:csv(::changeset)][bbox:{{bbox}}];"
                    + "("
                    + "node(newer:\"" + time + "\");"
                    + "way(newer:\"" + time + "\");"
                    + "rel(newer:\"" + time + "\");"
                    + ");"
                    + "out meta;";

            try {
                logger.info("Database sync starting...");

                BufferedReader br;
                InputStream responseStream;
                while (TIMEOUT_TIME > timeSpent) {
                    url = PullOverpassCommand.getOverpassUrl(replaceSensitiveData(PRIVATE_OVERPASS_URL), this.params.getBounds(), "csv", query);

                    // if cert path and phrase are specified then we assume to use them for the request
                    if (!replaceSensitiveData(PRIVATE_OVERPASS_CERT_PATH).equals(PRIVATE_OVERPASS_CERT_PATH)) {
                        responseStream = PullApiCommand.getHttpResponseWithSSL(url);
                    } else {
                        responseStream = GrailResource.getUrlInputStreamWithNullHostnameVerifier(url);
                    }

                    br = new BufferedReader(new InputStreamReader(responseStream));
                    String line;
                    while ((line = br.readLine()) != null) {
                        if (line.contains(lastId)) {
                            foundChangesetId = true;
                            break;
                        }
                    }
                    br.close();

                    if (foundChangesetId) {
                        break;
                    } else {
                        Thread.sleep(SLEEP_TIME);
                        timeSpent += SLEEP_TIME;
                    }
                }
            } catch (InterruptedException exc) {
                String msg = "Waiting for overpass update failed due to interrupted exception. [" + url + "]" + exc.getMessage();
                throw new WebApplicationException(exc, Response.serverError().entity(msg).build());

            } catch (Exception exc) {
                String msg = "Failure to poll data from overpass during WaitOverpassUpdate. [" + url + "]" + exc.getMessage();
                throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
            }

            // tag timeout if not found
            if (!foundChangesetId) {
                DbUtils.tagTimeoutTask(jobId);
                String msg = "Overpass sync wait time exceeded.";
                throw new WebApplicationException(new NotFoundException(), Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
            } else if (this.params.getTaskInfo() != null) {
                DbUtils.removeTimeoutTag(this.params.getTaskInfo());
            }

            logger.info("Database sync complete...");
        }

        LocalDateTime finishTime = LocalDateTime.now();
        String timeWaited = secondsToTimeString(Duration.between(startTime, finishTime).getSeconds());
        CommandResult commandResultFinish = new CommandResult();
        commandResultFinish.setStart(finishTime);
        commandResultFinish.setCommand("Overpass sync wait complete");
        commandResultFinish.setJobId(jobId);
        commandResultFinish.setStdout("Finished wait on overpass sync after " + timeWaited + "\n");
        commandResultFinish.setStderr("");
        commandResultFinish.setPercentProgress(100);
        commandResultFinish.setCaller(caller.getName());
        commandResultFinish.setExitCode(CommandResult.SUCCESS);
        commandResultFinish.setFinish(finishTime);
        DbUtils.upsertCommandStatus(commandResultFinish);
        DbUtils.completeCommandStatus(commandResultFinish);

        return commandResultFinish;
    }

    static String secondsToTimeString(long totalSeconds) {
        long seconds = (totalSeconds % 60);
        long minutes = (totalSeconds % 3600) / 60;
        long hours = (totalSeconds % 86400) / 3600;
        long days = (totalSeconds % (86400 * 30)) / 86400;

        String timeString = "";
        if (days > 0) {
            timeString += days + " days ";
        }
        if (hours > 0) {
            timeString += hours + " hours ";
        }
        if (minutes > 0) {
            timeString += minutes + " minutes ";
        }
        if (seconds > 0) {
            timeString += seconds + " seconds";
        }

        return timeString.trim();
    }

}
