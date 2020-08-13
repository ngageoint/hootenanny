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
import java.time.LocalDateTime;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import hoot.services.command.CommandResult;
import hoot.services.utils.DbUtils;

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
        CommandResult commandResultStart = new CommandResult();
        commandResultStart.setStart(LocalDateTime.now());
        commandResultStart.setCommand("Overpass sync wait");
        commandResultStart.setJobId(jobId);
        commandResultStart.setStdout("Starting wait on overpass sync...\n");
        commandResultStart.setStderr("");
        commandResultStart.setPercentProgress(0);
        commandResultStart.setCaller(caller.getName());
        commandResultStart.setExitCode(CommandResult.SUCCESS);
        commandResultStart.setFinish(LocalDateTime.now());
        DbUtils.upsertCommandStatus(commandResultStart);
        DbUtils.completeCommandStatus(commandResultStart);

        if (GrailResource.isPrivateOverpassActive()) {
            long timeSpent = 0;
            String url = null;

            String lastId = DbUtils.getLastPushedId(jobId);
            String time = LocalDateTime.now().minusMinutes(5).toString(); // get 5 minutes before in case the update happened before this command was run
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
                    responseStream = PullApiCommand.getHttpResponseWithSSL(url);

                    br = new BufferedReader(new InputStreamReader(responseStream));
                    String line;
                    boolean foundChangesetId = false;
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

            }
            catch (Exception exc) {
                String msg = "Waiting for overpass update failed. [" + url + "]" + exc.getMessage();
                throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
            }

            logger.info("Database sync complete...");
        }

        CommandResult commandResultFinish = new CommandResult();
        commandResultFinish.setStart(LocalDateTime.now());
        commandResultFinish.setCommand("Overpass sync wait complete");
        commandResultFinish.setJobId(jobId);
        commandResultFinish.setStdout("Finished wait on overpass sync...\n");
        commandResultFinish.setStderr("");
        commandResultFinish.setPercentProgress(0);
        commandResultFinish.setCaller(caller.getName());
        commandResultFinish.setExitCode(CommandResult.SUCCESS);
        commandResultFinish.setFinish(LocalDateTime.now());
        DbUtils.upsertCommandStatus(commandResultFinish);
        DbUtils.completeCommandStatus(commandResultFinish);

        return commandResultFinish;
    }
}
