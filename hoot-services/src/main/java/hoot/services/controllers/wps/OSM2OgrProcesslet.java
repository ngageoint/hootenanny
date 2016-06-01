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

import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * @author Jong Choi
 *         <p>
 *         WPS processlet for osm2ogr operation.
 */
public class OSM2OgrProcesslet extends BaseProcesslet {

    private static final Logger log = LoggerFactory.getLogger(OSM2OgrProcesslet.class);

    /**
     *
     */
    public OSM2OgrProcesslet() throws Exception {

    }

    /*
     * (non-Javadoc)
     *
     * @see hoot.services.wps.WPSProcesslet#destroy()
     */
    @Override
    public void destroy() {
        //
    }

    /*
     * (non-Javadoc)
     *
     * @see hoot.services.wps.WPSProcesslet#init()
     */
    @Override
    public void init() {
        //
    }

    /*
     * (non-Javadoc)
     *
     * @see hoot.services.wps.WPSProcesslet#process(org.deegree.services.wps.
     * ProcessletInputs, org.deegree.services.wps.ProcessletOutputs,
     * org.deegree.services.wps.ProcessletExecutionInfo)
     *
     * Process osm2ogr operation. For param filter see OSM2OgrProcesslet.xml
     */
    @Override
    public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
            throws ProcessletException {
        String jobIdStr = java.util.UUID.randomUUID().toString();
        JSONArray args = parseRequestParams(in);
        JSONObject command = new JSONObject();
        try {
            command.put("exectype", "hoot");
            command.put("exec", "osm2ogr");
            command.put("params", args);
            String commandStr = command.toString();

            // Async call to internal Native Interface servlet.

            CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
            httpclient.start();
            // Execute request

            HttpPost request1 = new HttpPost(coreJobServerUrl + "/hoot-services/job/" + jobIdStr);
            StringEntity se = new StringEntity(commandStr);
            request1.setEntity(se);
            httpclient.execute(request1, null);

            ((LiteralOutput) out.getParameter("jobId")).setValue(jobIdStr);
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }
    }
}
