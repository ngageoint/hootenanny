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

import java.util.Map;
import java.util.concurrent.Future;

import javax.ws.rs.core.Response.Status;

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

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;


public class JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(JobControllerBase.class);
    private static final String coreJobServerUrl = HootProperties.getProperty("coreJobServerUrl");
    protected String processScriptName;
    private static int jobResConnectionTimeout;

    static {
        try {
            jobResConnectionTimeout = Integer.parseInt(HootProperties.getProperty("internalJobRequestWaitTimeMilli"));
        }
        catch (NumberFormatException nfe) {
            jobResConnectionTimeout = 3000;
            logger.error("internalJobRequestWaitTimeMilli is not a valid number!  Defaulting to {}", jobResConnectionTimeout);
        }
    }

    public JobControllerBase(String processScriptName) {
        this.processScriptName = processScriptName;
    }

    /**
     * Post Job request to jobExecutioner Servlet
     * 
     * @param jobId
     * @param requestParams
     */
    public void postJobRquest(String jobId, String requestParams) throws Exception {
        logger.debug(jobId);
        logger.debug(requestParams);

        // Request should come back immediately but if something is wrong then timeout and clean up to make UI responsive
        RequestConfig requestConfig =
                RequestConfig.custom()
                .setConnectTimeout(jobResConnectionTimeout)
                .setSocketTimeout(jobResConnectionTimeout).build();

        try (CloseableHttpAsyncClient httpclient = HttpAsyncClients.custom().setDefaultRequestConfig(requestConfig).build()) {
            httpclient.start();

            HttpPost httpPost = new HttpPost(coreJobServerUrl + "/hoot-services/job/" + jobId);
            logger.debug("postJobRequest : {}/hoot-services/job/{}", coreJobServerUrl, jobId);
            StringEntity se = new StringEntity(requestParams);
            httpPost.setEntity(se);

            Future<HttpResponse> future = httpclient.execute(httpPost, null);

            // wait for response
            HttpResponse r = future.get();

            logger.debug("postJobRequest Response: {}", r.getStatusLine());
        }
        catch (Exception ee) {
            ResourceErrorHandler.handleError("Failed upload: " + ee, Status.INTERNAL_SERVER_ERROR, logger);
        }
    }

    public void postChainJobRquest(String jobId, String requestParams) throws Exception {
        // Request should come back immediately but if something is wrong then
        // timeout and clean up.to make UI responsive
        RequestConfig requestConfig =
                RequestConfig.custom()
                .setConnectTimeout(jobResConnectionTimeout)
                .setSocketTimeout(jobResConnectionTimeout).build();

        try (CloseableHttpAsyncClient httpclient = HttpAsyncClients.custom().setDefaultRequestConfig(requestConfig).build()) {
            httpclient.start();

            HttpPost httpPost = new HttpPost(coreJobServerUrl + "/hoot-services/job/chain/" + jobId);
            logger.debug("postChainJobRquest : {}/hoot-services/job/chain/{}", coreJobServerUrl, jobId);

            StringEntity se = new StringEntity(requestParams);
            httpPost.setEntity(se);

            Future<HttpResponse> future = httpclient.execute(httpPost, null);

            // wait till we get response
            HttpResponse r = future.get();

            logger.debug("postChainJobRquest Response x: {}", r.getStatusLine());
        }
        catch (Exception ee) {
            ResourceErrorHandler.handleError("Failed upload: " + ee, Status.INTERNAL_SERVER_ERROR, logger);
        }
    }

    protected String createPostBody(JSONArray args) {
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

    protected JSONObject createReflectionJobReq(JSONArray args, String className, String methodName) {
        this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "reflection");
        command.put("class", className);
        command.put("method", methodName);
        command.put("params", args);

        return command;
    }

    protected JSONObject createReflectionJobReq(JSONArray args, String className, String methodName,
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

    protected JSONObject createReflectionSycJobReq(JSONArray args, String className, String methodName) {
        this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "reflection_sync");
        command.put("class", className);
        command.put("method", methodName);
        command.put("params", args);

        return command;
    }

    protected String createBashPostBody(JSONArray args) {
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

    protected JSONObject createMakeScriptJobReq(JSONArray args) {
        JSONObject command = _createPostBody(args);
        return command;
    }

    protected static JSONArray parseParams(String params) throws Exception {
        JSONParser parser = new JSONParser();
        JSONObject command = (JSONObject) parser.parse(params);
        JSONArray commandArgs = new JSONArray();

        for (Object o : command.entrySet()) {
            Map.Entry mEntry = (Map.Entry) o;
            String key = (String) mEntry.getKey();
            String val = (String) mEntry.getValue();

            JSONObject arg = new JSONObject();
            arg.put(key, val);
            commandArgs.add(arg);

        }
        return commandArgs;
    }

    String getParameterValue(String key, JSONArray args) {
        for (Object arg : args) {
            JSONObject o = (JSONObject) arg;
            if (o.containsKey(key)) {
                return o.get(key).toString();
            }
        }
        return null;
    }
}