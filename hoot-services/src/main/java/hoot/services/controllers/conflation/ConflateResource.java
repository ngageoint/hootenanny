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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;
import hoot.services.controllers.common.ExportRenderDBCommandFactory;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;


@Controller
@Path("/conflation")
@Transactional
public class ConflateResource {
    private static final Logger logger = LoggerFactory.getLogger(ConflateResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ConflateCommandFactory conflateCommandFactory;

    @Autowired
    private ExportRenderDBCommandFactory exportRenderDBCommandFactory;

    @Autowired
    private UpdateTagsCommandFactory updateTagsCommandFactory;


    /**
     * Conflate service operates like a standard ETL service. The conflate
     * service specifies the input files, conflation type, match threshold, miss
     * threshold, and output file name. The conflation type can be specified as
     * the average of the two input datasets or based on a single input file
     * that is intended to be the reference dataset. It has two fronts, WPS and
     * standard rest end point.
     *
     * that is intended to be the reference dataset.
     *
     * POST hoot-services/conflation/execute
     *
     * @param params
     *            parameters in json format :
     *
     *     INPUT1_TYPE: Conflation input type [OSM] | [OGR] | [DB] | [OSM_API_DB]
     *     INPUT2_TYPE: Conflation input type [OSM] | [OGR] | [DB]
     *     INPUT1: Conlfation input 1
     *     INPUT2: Conlfation input 2
     *     OUTPUT_NAME: Conflation operation output name
     *     CONFLATION_TYPE: [Average] | [Reference]
     *     REFERENCE_LAYER:
     *         The reference layer which will be dominant tags. Default is 1 and if 2 selected, layer 2
     *         tags will be dominant with layer 1 as geometry snap layer.
     *
     *     COLLECT_STATS: true to collect conflation statistics
     *     ADV_OPTIONS: Advanced options list for hoot-core command
     *
     *     GENERATE_REPORT: Not used.  true to generate conflation report
     *     TIME_STAMP: Not used   Time stamp used in generated report if GENERATE_REPORT is true
     *     USER_EMAIL: Not used.  Email address of the user requesting the conflation job
     *     AUTO_TUNNING: Not used. Always false
     *
     * @return Job ID
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflate(ConflateParams params,
                             @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {

        String jobId = UUID.randomUUID().toString();

        try {
            ExternalCommand conflateCommand = conflateCommandFactory.build(jobId, params, debugLevel, this.getClass());
            InternalCommand updateTagsCommand = updateTagsCommandFactory.build(jobId, params, this.getClass());
            ExternalCommand exportRenderDBCommand = exportRenderDBCommandFactory.build(jobId, params.getOutputName(), this.getClass());

            Command[] workflow = { conflateCommand, updateTagsCommand, exportRenderDBCommand };

            jobProcessor.submitAsync(new Job(jobId, workflow));
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during conflation!  Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }
}