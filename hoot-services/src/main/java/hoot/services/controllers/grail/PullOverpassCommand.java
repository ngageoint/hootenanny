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

import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.net.URL;
import java.time.LocalDateTime;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.geo.BoundingBox;


/**
 * Used for pulling OSM data from Overpass
 */
class PullOverpassCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullOverpassCommand.class);

    private final GrailParams params;
    private final String jobId;
    private final Class<?> caller;

    PullOverpassCommand(GrailParams params, String jobId, Class<?> caller) {
        this.params = params;
        this.jobId = jobId;
        this.caller = caller;

        logger.info("Params: "+ params.toString());
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Pull OSM Data from Overpass] for " + params.getBounds());
        commandResult.setStart(LocalDateTime.now());
        commandResult.setCaller(caller.getName());

        getOverpass();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    private void getOverpass() {
        String url = "";
        try {
                BoundingBox boundingBox = new BoundingBox(params.getBounds());

    // Compact QL
    // https://overpass-api.de/api/interpreter?data=(node(-34.0044,150.9982,-33.9728,151.0656);<;>;);out meta qt;

    // XML
    // <osm-script>
    //   <union into="_">
    //     <bbox-query s="-34.0044" w="150.9982" n="-33.9728" e="151.0656"/>
    //     <recurse from="_" into="_" type="up"/>
    //     <recurse from="_" into="_" type="down"/>
    //   </union>
    //   <print e="" from="_" geometry="skeleton" ids="yes" limit="" mode="meta" n="" order="quadtile" s="" w=""/>
    // </osm-script>

                // This is Ugly! It is the encoded version of the compact QL script above
                url = replaceSensitiveData(params.getPullUrl()) +
                    "/api/interpreter?data=(node(" +
                    boundingBox.getMinLat() + "%2C" +
                    boundingBox.getMinLon() + "%2C" +
                    boundingBox.getMaxLat() + "%2C" +
                    boundingBox.getMaxLon() + ")%3B%3C%3B%3E%3B)%3Bout%20meta%20qt%3B";

                URL requestUrl = new URL(url);
                File outputFile = new File(params.getOutput());

                FileUtils.copyURLToFile(requestUrl,outputFile, Integer.parseInt(HootProperties.HTTP_TIMEOUT), Integer.parseInt(HootProperties.HTTP_TIMEOUT));
            }
            catch (Exception ex) {
                String msg = "Failure to pull data from Overpass [" + url + "]" + ex.getMessage();
                // throw new RuntimeException(msg, ex);
                throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
            }
    }
}
