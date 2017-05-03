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
import hoot.services.command.InternalCommand;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;


@Controller
@Path("/review/custom/HGIS")
@Transactional
public class HGISReviewResource extends HGISResource {
    private static final Logger logger = LoggerFactory.getLogger(HGISReviewResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private HGISPrepareForValidationCommandFactory hgisPrepareForValidationCommandFactory;

    @Autowired
    private UpdateMapTagsCommandFactory updateMapTagsCommandFactory;


    public HGISReviewResource() {}

    /**
     * This resource prepares existing map for 30% of random HGIS specific validation.
     * <p>
     *     POST hoot-services/job/review/custom/HGIS/preparevalidation
     * <p>
     * {
     *   "sourceMap":"AllDataTypesA", //Name of source layer
     *   "outputMap":"AllDataTypesAtest1" //Name of new output layer with reviewables
     * }
     *
     * @param request
     * @return Job ID
     */
    @POST
    @Path("/preparevalidation")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public PrepareForValidationResponse prepareItemsForValidationReview(PrepareForValidationRequest request) {
        checkHGISCommandParams(request.getSourceMap(), request.getOutputMap());

        try {
            String jobId = UUID.randomUUID().toString();
            String sourceMap = request.getSourceMap();
            String outputMap = request.getOutputMap();

            ExternalCommand validationCommand = hgisPrepareForValidationCommandFactory.build(jobId, sourceMap, outputMap, this.getClass());
            InternalCommand updateMapTagsCommand = updateMapTagsCommandFactory.build(jobId, outputMap, this.getClass());

            Command[] workflow = { validationCommand, updateMapTagsCommand };

            jobProcessor.submitAsync(new Job(jobId, workflow));

            PrepareForValidationResponse response = new PrepareForValidationResponse();
            response.setJobId(jobId);

            return response;
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception ex) {
            String msg = "Error preparing for validation";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }
    }
}
