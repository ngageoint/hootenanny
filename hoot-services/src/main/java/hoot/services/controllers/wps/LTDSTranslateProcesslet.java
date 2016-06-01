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

import java.util.concurrent.Future;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.apache.http.util.EntityUtils;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class LTDSTranslateProcesslet extends BaseProcesslet {

    private static final Logger log = LoggerFactory.getLogger(LTDSTranslateProcesslet.class);

    public LTDSTranslateProcesslet() throws Exception {

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

    @Override
    public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
            throws ProcessletException {
        JSONArray args = parseRequestParams(in);

        try {
            String commandStr = "";
            String osmId = "";
            String translation = "";
            for (int i = 0; i < args.size(); i++) {
                JSONObject oParam = (JSONObject) args.get(i);
                if (oParam.containsKey("OSM_INPUT")) {
                    commandStr = oParam.get("OSM_INPUT").toString();
                }

                if (oParam.containsKey("ELEMENT_ID")) {
                    osmId = oParam.get("ELEMENT_ID").toString();
                }

                if (oParam.containsKey("TRANSLATION")) {
                    translation = oParam.get("TRANSLATION").toString();
                }
            }
            String res = postJobRquest(osmId, translation, commandStr);

            ((LiteralOutput) out.getParameter("OGR")).setValue(res);

        }
        catch (Exception e) {
            log.error(e.getMessage());
        }
    }

    /**
     * Post Job request to jobExecutioner Servlet
     * 
     * @param jobId
     * @param requestParams
     * @throws Exception
     */
    private static String postJobRquest(String osmId, String translation, String requestParams) throws Exception {
        String ret = "";
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
        try {
            httpclient.start();
            // Execute request

            final HttpPost request1 = new HttpPost(
                    coreJobServerUrl + "/hoot-services/ogr/ltds/translate/" + osmId + "?translation=" + translation);
            StringEntity se = new StringEntity(requestParams);
            request1.setEntity(se);
            Future<HttpResponse> future = httpclient.execute(request1, null);
            HttpResponse response1 = future.get();
            HttpEntity entity = response1.getEntity();
            ret = EntityUtils.toString(entity, "UTF-8");

        }
        catch (Exception ee) {
            log.error(ee.getMessage());
        }
        finally {
            httpclient.close();
        }
        return ret;
    }

}
