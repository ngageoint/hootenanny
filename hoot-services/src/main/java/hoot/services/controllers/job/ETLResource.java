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

import static hoot.services.HootProperties.ETL_MAKEFILE;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONArray;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


@Path("/etl")
public class ETLResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(ETLResource.class);

    public ETLResource() {
        super(ETL_MAKEFILE);
    }

    /**
     * For ETL service, there are 2 types of services are available: Standard
     * REST endpoint. Accessed by POST. ETL
     * service ends up at hoot command shell and use makeetl make file. makeetl
     * make file handles 2 types of ETL formats: OGR and OSM. For OGR, it
     * translates the INPUT shapefile into hoot db using provided translation
     * file. Also, multiple inputs can be listed using semicolon as a separator.
     * For OSM, it directly converts the INPUT osm file into hoot db. makeetl
     * file assumes that the specified translation file and INPUT files resides
     * in an common parent directory where current default is at HOOT_HOME.
     * 
     * POST hoot-services/job/etl/load
     * 
     * { "TRANSLATION":"translations/MGCP.js", //relative path of translation
     * script. (relative to HOOT_HOME) "INPUT_TYPE":"OSM", [ OSM | OGR ]
     * "INPUT":"test-files/ToyTestA.osm", relative path of input file. (relative
     * to HOOT_HOME) "INPUT_NAME":"ToyTestA" Name stored in hoot db. }
     * 
     * @param params
     *            input parameters; see description
     * @return Job ID
     * @deprecated
     */
    @Deprecated
    @POST
    @Path("/load")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public JobId process(String params) {
        String uuid = UUID.randomUUID().toString();

        try {
            JSONArray commandArgs = parseParams(params);
            String argStr = createPostBody(commandArgs);

            postJobRquest(uuid, argStr);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error processing ETL request!  Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return new JobId(uuid);
    }
}
