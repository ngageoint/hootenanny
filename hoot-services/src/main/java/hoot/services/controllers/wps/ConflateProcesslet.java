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

import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;


public class ConflateProcesslet extends JobProcesslet {
    private static final Logger log = LoggerFactory.getLogger(ConflateProcesslet.class);

    public ConflateProcesslet() throws Exception {
        String makefilePath = HootProperties.getProperty("ConflateMakefilePath");
        this.setProcessScriptName(makefilePath);
    }

    @Override
    public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
            throws ProcessletException {

        JSONArray args = parseRequestParams(in);

        try {
            String confOutputName = null;
            for (Object arg1 : args) {
                JSONObject arg = (JSONObject) arg1;
                Object val = arg.get("OUTPUT_NAME");

                if (val != null) {
                    confOutputName = val.toString();
                    break;
                }
            }
            JSONObject conflationCommand = _createPostBody(args);

            JSONArray jobArgs = new JSONArray();
            jobArgs.add(conflationCommand);

            postChainJobRquest(jobIdStr, jobArgs.toJSONString());

            ((LiteralOutput) out.getParameter("jobId")).setValue(jobIdStr);
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }
    }
}
