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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;
import hoot.services.controllers.osm.map.UpdateParentCommandFactory;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.job.JobType;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;


@Controller
@Path("/conflation")
@Transactional
public class ConflateResource {
    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ConflateCommandFactory conflateCommandFactory;

    @Autowired
    private UpdateTagsCommandFactory updateTagsCommandFactory;

    @Autowired
    private UpdateParentCommandFactory updateParentCommandFactory;


    /**
     * Conflate service operates like a standard ETL service. The conflate
     * service specifies the input files, conflation type, options
     * and output file name.
     *
     * POST hoot-services/conflation/execute
     *
     * @param params
     *            parameters in json format :
     *
     *     INPUT1_TYPE: Conflation input type [OSM] | [OGR] | [DB]
     *     INPUT2_TYPE: Conflation input type [OSM] | [OGR] | [DB]
     *     INPUT1: Conflation input 1
     *     INPUT2: Conflation input 2
     *     OUTPUT_NAME: Conflation operation output name
     *     OUTPUT_FOLDER: Optional folder for DB outputs
     *     CONFLATION_TYPE: [Horizontal] | [Reference]
     *     REFERENCE_LAYER:
     *         The reference layer which will be dominant tags. Default is 1 and if 2 selected, layer 2
     *         tags will be dominant with layer 1 as geometry snap layer.
     *
     *     COLLECT_STATS: true to collect conflation statistics
     *     ADV_OPTIONS: Advanced options list for hoot-core command
     *
     *
     * @return Job ID
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflate(@Context HttpServletRequest request,
            ConflateParams params,
            @QueryParam("DEBUG_LEVEL") @DefaultValue("info") String debugLevel) {
        Users user = Users.fromRequest(request);

        String jobId = UUID.randomUUID().toString();

        try {
            ExternalCommand conflateCommand = conflateCommandFactory.build(jobId, params, debugLevel, this.getClass(), user);
            InternalCommand updateTagsCommand = updateTagsCommandFactory.build(jobId, user.getId(), params, this.getClass());
            InternalCommand setFolderCommand = updateParentCommandFactory.build(jobId, params.getOutputFolder(), params.getOutputName(), user, this.getClass());

            Command[] workflow = { conflateCommand, updateTagsCommand, setFolderCommand };

            Map<String, Object> jobStatusTags = new HashMap<>();
            jobStatusTags.put("bbox", params.getBounds());
            jobStatusTags.put("input1", params.getInput1());
            jobStatusTags.put("input2", params.getInput2());
            jobStatusTags.put("taskInfo", params.getTaskInfo());
            jobStatusTags.put("conflationType", params.getConflationType());

            // add both jobIds related to the inputs as the parent seperated by comma if both jobs exist
            String input1JobId = DbUtils.getJobIdByMapId(Long.parseLong(params.getInput1()));
            String input2JobId = DbUtils.getJobIdByMapId(Long.parseLong(params.getInput2()));
            String parentId = null;
            if (input1JobId != null && input2JobId != null) {
                parentId = input1JobId + "," + input2JobId;
            } else if (input1JobId != null) {
                parentId = input1JobId;
            } else if (input2JobId != null) {
                parentId = input2JobId;
            }
            jobStatusTags.put("parentId", parentId);

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow, JobType.CONFLATE, jobStatusTags));
        }
        catch (IllegalArgumentException iae) {
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
        catch (Exception e) {
            String msg = "Error during conflation!  Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("jobid", jobId);
        return Response.ok().entity(ret).build();
    }
}
