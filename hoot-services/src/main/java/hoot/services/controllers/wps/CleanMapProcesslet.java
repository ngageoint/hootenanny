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
package hoot.services.controllers.wps;

import java.util.List;
import java.util.UUID;

import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.input.LiteralInput;
import org.deegree.services.wps.input.ProcessletInput;
import org.deegree.services.wps.output.LiteralOutput;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.job.JobExecutioner;


public class CleanMapProcesslet extends BaseProcesslet {
    private static final Logger log = LoggerFactory.getLogger(CleanMapProcesslet.class);

    public CleanMapProcesslet() throws Exception {
        super();
    }

    /*
     * (non-Javadoc)
     * 
     * @see hoot.services.wps.WPSProcesslet#process(org.deegree.services.wps.
     * ProcessletInputs, org.deegree.services.wps.ProcessletOutputs,
     * org.deegree.services.wps.ProcessletExecutionInfo)
     */
    @Override
    public void process(ProcessletInputs inputParams, ProcessletOutputs outputParams,
            ProcessletExecutionInfo processletExecInfo) throws ProcessletException {

        try {
            String mapId = null;
            List<ProcessletInput> params = inputParams.getParameters();

            for (int i = 0; i < params.size(); i++) {
                ProcessletInput input = params.get(i);
                String id = input.getIdentifier().getCode();
                String value = ((LiteralInput) inputParams.getParameter(id)).getValue().trim();
                if (id.equalsIgnoreCase("mapId")) {
                    mapId = value;
                    break;
                }
            }

            if (mapId == null) {
                throw new Exception("Invalid mapId parameter.");
            }

            JSONObject command = new JSONObject();
            command.put("mapId", mapId);
            command.put("execImpl", "ResourcesCleanUtil");

            final String jobId = UUID.randomUUID().toString();

            (new JobExecutioner(jobId, command)).start();

            ((LiteralOutput) outputParams.getParameter("jobId")).setValue(jobId);
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }

    }
}
