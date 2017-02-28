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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.hgis;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;


@Controller
@Path("/filter/custom/HGIS")
@Transactional
public class HGISFilterResource extends HGISResource {
    private static final Logger logger = LoggerFactory.getLogger(HGISFilterResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExternalCommandManager externalCommandManager;

    @Autowired
    private FilterNonHGISPOIsCommandFactory filterNonHGISPOIsCommandFactory;


    public HGISFilterResource() {}

    /**
     * This resource produces layer that filters Non HGIS POIs.
     * <p>
     *     POST hoot-services/job/filter/custom/HGIS/filternonhgispois
     * <p>
     * { "source":"AllDataTypesA", "output":"AllDataTypesAtest1" }
     *
     * @param request
     * @return Job ID
     */
    @POST
    @Path("/filternonhgispois")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public FilterNonHgisPoisResponse filterNonHgisPois(FilterNonHgisPoisRequest request) {
        FilterNonHgisPoisResponse response = new FilterNonHgisPoisResponse();

        checkHGISCommandParams(request.getSource(), request.getOutput());

        try {
            String jobId = UUID.randomUUID().toString();

            Command[] commands = {
                () -> {
                    ExternalCommand filterNonHgisPoisCommand = filterNonHGISPOIsCommandFactory.build(
                        request.getSource(), request.getOutput(), this.getClass());
                    return externalCommandManager.exec(jobId, filterNonHgisPoisCommand);
                }
            };

            jobProcessor.process(new Job(jobId, commands));

            response.setJobId(jobId);
        }
        catch (Exception e) {
            String msg = "Error while trying to filter non-HGIS POI's.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return response;
    }
}
