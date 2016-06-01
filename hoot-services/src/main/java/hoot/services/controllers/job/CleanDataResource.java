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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;


@Path("/cleandata")
public class CleanDataResource extends JobControllerBase {
    private static final Logger log = LoggerFactory.getLogger(CleanDataResource.class);
    @SuppressWarnings("unused")
    private String homeFolder = null;

    public CleanDataResource() {
        try {
            if (processScriptName == null) {
                processScriptName = HootProperties.getProperty("cleanDataMakePath");
            }

            homeFolder = HootProperties.getProperty("homeFolder");
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    /**
     * Clean Data service represents REST and WPS end points for hoot --cleanup
     * which is defined as --cleanup (input) (output). Removes common erroneous
     * data scenarios from input and writes to output. * input - Input (e.g.
     * .osm file). output - Output file (e.g. .osm file).
     * 
     * When DB Type then name of the map record. If OSM then the relative (to
     * HOOT_HOME) path of osm file.
     * 
     * POST hoot-services/job/cleandata/execute
     * 
     * { "INPUT_TYPE":"DB", "INPUT":"DcGisRoads", "OUTPUT_TYPE":"DB",
     * "OUTPUT":"DcGisRoadsOUt5" }
     * 
     * @param params
     *            [OMS|DB]
     * @return Job Id
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response process(String params) {
        String jobId = UUID.randomUUID().toString();
        try {

            JSONArray commandArgs = parseParams(params);
            String argStr = createPostBody(commandArgs);
            postJobRquest(jobId, argStr);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error process data clean request: " + ex.toString(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }
        JSONObject res = new JSONObject();
        res.put("jobid", jobId);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

}
