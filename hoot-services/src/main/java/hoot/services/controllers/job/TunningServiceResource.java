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
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.job.JobExecutioner;


@Path("/tunning")
public class TunningServiceResource {
    private static final Logger logger = LoggerFactory.getLogger(TunningServiceResource.class);

    public TunningServiceResource() {}

    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public JobId process(String params) {
        String uuid = UUID.randomUUID().toString();

        try {
            JSONParser jsonParser = new JSONParser();
            JSONObject parameters = (JSONObject) jsonParser.parse(params);
            String input = parameters.get("INPUT").toString();
            String inputType = parameters.get("INPUT_TYPE").toString();

            JSONObject command = new JSONObject();
            command.put("input", input);
            command.put("inputtype", inputType);
            command.put("execImpl", "tunningService");

            (new JobExecutioner(uuid, command)).start();
        }
        catch (Exception e) {
            String message = "Tuning Service error: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        return new JobId(uuid);
    }
}
