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

import java.util.Iterator;
import java.util.Map;

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


public class JobProcesslet extends BaseProcesslet {

    private static final Logger log = LoggerFactory.getLogger(JobProcesslet.class);
    private String processScriptName = null;
    private JSONObject customParams = null;
    protected String jobIdStr = java.util.UUID.randomUUID().toString();

    public JobProcesslet() throws Exception {
        super();

    }

    public String getProcessScriptName() {
        return processScriptName;
    }

    public void setProcessScriptName(String processScriptName) {
        this.processScriptName = processScriptName;
    }

    public void setCustomParams(JSONObject params) {
        customParams = params;
    }

    public JSONObject getCustomParams() {
        return customParams;
    }

    @Override
    public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
            throws ProcessletException {

        JSONArray args = parseRequestParams(in);

        try {
            String commandStr = createPostBody(args);
            postJobRquest(jobIdStr, commandStr);

            ((LiteralOutput) out.getParameter("jobId")).setValue(jobIdStr);
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
     */
    protected void postJobRquest(String jobId, String requestParams) throws Exception {
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
        try {
            httpclient.start();
            // Execute request

            final HttpPost request1 = new HttpPost(coreJobServerUrl + "/hoot-services/job/" + jobId);
            StringEntity se = new StringEntity(requestParams);
            request1.setEntity(se);
            httpclient.execute(request1, null);
        }
        catch (Exception ee) {
            log.error(ee.getMessage());
        }
        finally {
            httpclient.close();
        }
    }

    public void postChainJobRquest(String jobId, String requestParams) throws Exception {
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
        try {
            httpclient.start();
            final HttpPost request1 = new HttpPost(coreJobServerUrl + "/hoot-services/job/chain/" + jobId);
            StringEntity se = new StringEntity(requestParams);
            request1.setEntity(se);
            httpclient.execute(request1, null);

        }
        catch (Exception ee) {
            log.error(ee.getMessage());
        }
        finally {
            httpclient.close();
        }
    }

    public String createPostBody(JSONArray args) {
        return _createPostBody(args).toString();
    }

    protected JSONObject _createPostBody(JSONArray args) {
        String resourceName = this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "make");
        command.put("exec", processScriptName);
        command.put("caller", resourceName);

        if (customParams != null) {
            Iterator it = customParams.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry pairs = (Map.Entry) it.next();
                String k = pairs.getKey().toString();
                String v = pairs.getValue().toString();

                boolean found = false;
                for (int i = 0; i < args.size(); i++) {
                    JSONObject arg = (JSONObject) args.get(i);
                    Object o = arg.get(k);
                    if (o != null) {
                        String oStr = o.toString();
                        if (oStr.length() > 0) {
                            found = true;
                            break;
                        }
                    }
                }

                if (!found) {
                    JSONObject newParam = new JSONObject();
                    newParam.put(k, v);
                    args.add(newParam);
                }

            }
        }

        command.put("params", args);

        return command;
    }

    protected JSONObject createReflectionJobReq(JSONArray args, String className, String methodName) {
        this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "reflection");
        command.put("class", className);
        command.put("method", methodName);
        command.put("params", args);

        return command;
    }

    protected JSONObject createReflectionSycJobReq(JSONArray args, String className, String methodName) {
        this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "reflection_sync");
        command.put("class", className);
        command.put("method", methodName);
        command.put("params", args);

        return command;
    }

    /*
     * (non-Javadoc)
     * 
     * @see hoot.services.controllers.wps.BaseProcesslet#destroy()
     */
    @Override
    public void destroy() {
        //
    }

    /*
     * (non-Javadoc)
     * 
     * @see hoot.services.controllers.wps.BaseProcesslet#init()
     */
    @Override
    public void init() {
        //
    }
}
