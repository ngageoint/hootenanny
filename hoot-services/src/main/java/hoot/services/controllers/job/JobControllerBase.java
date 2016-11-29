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

import static hoot.services.HootProperties.CORE_JOB_SERVER_URL;
import static hoot.services.HootProperties.INTERNAL_JOB_REQUEST_WAIT_TIME_MILLI;

import java.util.Map;
import java.util.concurrent.Future;

import org.apache.http.HttpResponse;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(JobControllerBase.class);
    private static final int JOB_RES_CONNECTION_TIMEOUT = Integer.parseInt(INTERNAL_JOB_REQUEST_WAIT_TIME_MILLI);

    protected String processScriptName;


    public JobControllerBase(String processScriptName) {
        this.processScriptName = processScriptName;
    }

    /**
     * Post Job request to jobExecutioner Servlet
     * 
     * @param jobId
     * @param requestParams
     */
    public void postJobRequest(String jobId, String requestParams) {
        logger.debug(jobId);
        logger.debug(requestParams);

        // Request should come back immediately but if something is wrong then timeout and clean up to make UI responsive
        RequestConfig requestConfig =
                RequestConfig.custom()
                        .setConnectTimeout(JOB_RES_CONNECTION_TIMEOUT)
                        .setSocketTimeout(JOB_RES_CONNECTION_TIMEOUT)
                        .build();

        try (CloseableHttpAsyncClient httpclient = HttpAsyncClients.custom().setDefaultRequestConfig(requestConfig).build()) {
            httpclient.start();

            HttpPost httpPost = new HttpPost(CORE_JOB_SERVER_URL + "/hoot-services/job/" + jobId);
            logger.debug("postJobRequest : {}/hoot-services/job/{}", CORE_JOB_SERVER_URL, jobId);
            StringEntity se = new StringEntity(requestParams);
            httpPost.setEntity(se);

            Future<HttpResponse> future = httpclient.execute(httpPost, null);

            // wait for response
            HttpResponse r = future.get();

            logger.debug("postJobRequest Response: {}", r.getStatusLine());
        }
        catch (Exception e) {
            String msg = "postJobRequest() failed!  Cause: " + e.getMessage();
            throw new RuntimeException(msg, e);
        }
    }

    public void postChainJobRequest(String jobId, String requestParams) {
        // Request should come back immediately but if something is wrong then
        // timeout and clean up.to make UI responsive
        RequestConfig requestConfig =
                RequestConfig.custom()
                        .setConnectTimeout(JOB_RES_CONNECTION_TIMEOUT)
                        .setSocketTimeout(JOB_RES_CONNECTION_TIMEOUT)
                        .build();

        try (CloseableHttpAsyncClient httpclient = HttpAsyncClients.custom().setDefaultRequestConfig(requestConfig).build()) {
            httpclient.start();

            HttpPost httpPost = new HttpPost(CORE_JOB_SERVER_URL + "/hoot-services/job/chain/" + jobId);
            logger.debug("postChainJobRequest : {}/hoot-services/job/chain/{}", CORE_JOB_SERVER_URL, jobId);

            StringEntity se = new StringEntity(requestParams);
            httpPost.setEntity(se);

            Future<HttpResponse> future = httpclient.execute(httpPost, null);

            // wait till we get response
            HttpResponse r = future.get();

            logger.debug("postChainJobRequest Response x: {}", r.getStatusLine());
        }
        catch (Exception e) {
            String msg = "postChainJobRequest() failed.  Cause: " + e.getMessage();
            throw new RuntimeException(msg, e);
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

    protected static JSONObject createReflectionJobReq(JSONArray args, String className, String methodName) {
        JSONObject command = new JSONObject();
        command.put("exectype", "reflection");
        command.put("class", className);
        command.put("method", methodName);
        command.put("params", args);

        return command;
    }

    protected static JSONObject createReflectionJobReq(JSONArray args, String className, String methodName,
            String internalJobId) {
        JSONObject command = new JSONObject();
        command.put("exectype", "reflection");
        command.put("class", className);
        command.put("method", methodName);
        command.put("internaljobid", internalJobId);
        command.put("params", args);

        return command;
    }

    static JSONObject createReflectionSycJobReq(JSONArray args, String className, String methodName) {
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

    protected static JSONArray parseParams(String params) throws ParseException {
        JSONParser parser = new JSONParser();
        JSONObject command = (JSONObject) parser.parse(params);
        JSONArray commandArgs = new JSONArray();

        for (Object o : command.entrySet()) {
            Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) o;
            String key = (String) mEntry.getKey();
            String val = (String) mEntry.getValue();

            JSONObject arg = new JSONObject();
            arg.put(key, val);
            commandArgs.add(arg);

        }

        return commandArgs;
    }

    static String getParameterValue(String key, JSONArray args) {
        for (Object arg : args) {
            JSONObject o = (JSONObject) arg;
            if (o.containsKey(key)) {
                return o.get(key).toString();
            }
        }

        return null;
    }
}