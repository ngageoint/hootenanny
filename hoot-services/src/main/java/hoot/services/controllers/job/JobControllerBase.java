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

import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.Future;

import javax.ws.rs.core.Response.Status;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;

import org.apache.http.HttpResponse;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class JobControllerBase {
    private static final Logger log = LoggerFactory.getLogger(JobControllerBase.class);
    private String coreJobServerUrl = null;
    protected String processScriptName = null;
    private int jobResConnectionTimeout = 3000;

    public JobControllerBase() {
        try {
            if (coreJobServerUrl == null) {
                coreJobServerUrl = HootProperties.getProperty("coreJobServerUrl");

                jobResConnectionTimeout = Integer.parseInt(HootProperties
                        .getProperty("internalJobRequestWaitTimeMilli"));

            }

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    /**
     * Post Job request to jobExecutioner Servlet
     * 
     * @param jobId
     * @param requestParams
     */
    public void postJobRquest(String jobId, String requestParams) throws Exception {

        log.debug(jobId);
        log.debug(requestParams);

        // Request should come back immediately but if something is wrong then
        // timeout and clean up.
        // To make UI responsive
        RequestConfig requestConfig = RequestConfig.custom().setConnectTimeout(jobResConnectionTimeout)
                .setSocketTimeout(jobResConnectionTimeout).build();
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.custom().setDefaultRequestConfig(requestConfig).build();
        try {
            httpclient.start();
            final HttpPost request1 = new HttpPost(coreJobServerUrl + "/hoot-services/job/" + jobId);
            log.debug("postJobRequest : " + coreJobServerUrl + "/hoot-services/job/" + jobId);
            StringEntity se = new StringEntity(requestParams);
            request1.setEntity(se);
            Future<HttpResponse> future = httpclient.execute(request1, null);
            // wait for response
            HttpResponse r = future.get();

            log.debug("postJobRequest Response: " + r.getStatusLine());
        }
        catch (Exception ee) {
            ResourceErrorHandler.handleError("Failed upload: " + ee.toString(), Status.INTERNAL_SERVER_ERROR, log);
        }
        finally {
            log.debug("postJobRequest Closing");
            httpclient.close();
        }
    }

    public void postChainJobRquest(String jobId, String requestParams) throws Exception {

        // log.debug(jobId);
        // log.debug(requestParams);

        // Request should come back immediately but if something is wrong then
        // timeout and clean up.
        // To make UI responsive
        RequestConfig requestConfig = RequestConfig.custom().setConnectTimeout(jobResConnectionTimeout)
                .setSocketTimeout(jobResConnectionTimeout).build();
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.custom().setDefaultRequestConfig(requestConfig).build();

        try {
            httpclient.start();
            final HttpPost request1 = new HttpPost(coreJobServerUrl + "/hoot-services/job/chain/" + jobId);
            log.debug("postChainJobRquest : " + coreJobServerUrl + "/hoot-services/job/chain/" + jobId);
            StringEntity se = new StringEntity(requestParams);
            request1.setEntity(se);
            Future<HttpResponse> future = httpclient.execute(request1, null);
            // wait till we get response
            HttpResponse r = future.get();

            log.debug("postChainJobRquest Response x: " + r.getStatusLine());

        }
        catch (Exception ee) {
            ResourceErrorHandler.handleError("Failed upload: " + ee.toString(), Status.INTERNAL_SERVER_ERROR, log);
        }
        finally {
            log.debug("Closing postChainJobRquest");
            // Clean file handles and thread
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
        command.put("params", args);

        return command;
    }

    protected JSONObject _createReflectionJobReq(JSONArray args, String className, String methodName) {
        this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "reflection");
        command.put("class", className);
        command.put("method", methodName);
        command.put("params", args);

        return command;
    }

    protected JSONObject _createReflectionJobReq(JSONArray args, String className, String methodName,
            String internalJobId) {
        this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "reflection");
        command.put("class", className);
        command.put("method", methodName);
        command.put("internaljobid", internalJobId);
        command.put("params", args);

        return command;
    }

    protected JSONObject _createReflectionSycJobReq(JSONArray args, String className, String methodName) {
        this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "reflection_sync");
        command.put("class", className);
        command.put("method", methodName);
        command.put("params", args);

        return command;
    }

    public String createBashPostBody(JSONArray args) {
        return _createBashPostBody(args).toString();
    }

    protected JSONObject _createBashPostBody(JSONArray args) {

        String resourceName = this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "bash");
        command.put("exec", processScriptName);
        command.put("caller", resourceName);
        command.put("params", args);

        return command;
    }

    protected JSONObject _createMakeScriptJobReq(JSONArray args) {
        JSONObject command = _createPostBody(args);
        return command;
    }

    public static JSONArray parseParams(String params) throws Exception {
        JSONParser parser = new JSONParser();
        JSONObject command = (JSONObject) parser.parse(params);
        Iterator iter = command.entrySet().iterator();

        JSONArray commandArgs = new JSONArray();

        while (iter.hasNext()) {
            Map.Entry mEntry = (Map.Entry) iter.next();
            String key = (String) mEntry.getKey();
            String val = (String) mEntry.getValue();

            JSONObject arg = new JSONObject();
            arg.put(key, val);
            commandArgs.add(arg);

        }
        return commandArgs;
    }

    public String getParameterValue(String key, JSONArray args) {
        for (int i = 0; i < args.size(); i++) {
            JSONObject o = (JSONObject) args.get(i);
            if (o.containsKey(key)) {
                return o.get(key).toString();
            }
        }
        return null;
    }
}