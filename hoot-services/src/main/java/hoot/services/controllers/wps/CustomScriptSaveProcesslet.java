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

import java.net.URI;
import java.net.URL;
import java.net.URLEncoder;
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


public class CustomScriptSaveProcesslet extends BaseProcesslet {
    private static final Logger log = LoggerFactory.getLogger(CustomScriptSaveProcesslet.class);

    public CustomScriptSaveProcesslet() throws Exception {

    }

    @Override
    public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
            throws ProcessletException {
        String ret = "";
        JSONArray args = parseRequestParams(in);

        try {
            String scriptName = null;
            String scriptDesc = null;
            String script = null;
            for (int i = 0; i < args.size(); i++) {
                JSONObject arg = (JSONObject) args.get(i);
                Object val = arg.get("NAME");

                if (val != null) {
                    scriptName = val.toString();

                }

                arg = (JSONObject) args.get(i);
                val = arg.get("DESCRIPTION");

                if (val != null) {
                    scriptDesc = val.toString();

                }

                arg = (JSONObject) args.get(i);
                val = arg.get("SCRIPT");

                if (val != null) {
                    script = val.toString();

                }
            }

            HttpResponse resp = postRequest(scriptName, scriptDesc, script);

            if (resp.getStatusLine().getStatusCode() != 200) {
                String reason = resp.getStatusLine().getReasonPhrase();
                if (reason == null) {
                    reason = "Unkown reason.";
                }
                throw new Exception(reason);
            }

            HttpEntity entity = resp.getEntity();
            if (entity != null) {
                entity.getContentLength();
                ret = EntityUtils.toString(entity);
            }

        }
        catch (Exception e) {
            log.error(e.getMessage());
            ((LiteralOutput) out.getParameter("SAVEDITEM")).setValue("Failed To Save: " + e.getMessage());
            return;
        }
        ((LiteralOutput) out.getParameter("SAVEDITEM")).setValue(ret);

    }

    protected HttpResponse postRequest(String scriptName, String scriptDesc, String script) throws Exception {
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
        httpclient.start();

        String url = coreJobServerUrl + "/hoot-services/ingest/customscript/save?SCRIPT_NAME="
                + URLEncoder.encode(scriptName, "UTF-8") + "&SCRIPT_DESCRIPTION="
                + URLEncoder.encode(scriptDesc, "UTF-8");
        URL urlParser = new URL(url);
        URI uri = urlParser.toURI();
        // Execute request
        final HttpPost request1 = new HttpPost(uri);

        StringEntity se = new StringEntity(script);
        request1.setEntity(se);
        Future<HttpResponse> future = httpclient.execute(request1, null);
        return future.get();
    }

}
