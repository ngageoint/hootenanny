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

import org.apache.http.HttpEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class CustomScriptGetListProcesslet extends BaseProcesslet {
    private static final Logger log = LoggerFactory.getLogger(CustomScriptGetListProcesslet.class);

    public CustomScriptGetListProcesslet() throws Exception {

    }

    @Override
    public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
            throws ProcessletException {
        String resp = "";

        try {
            resp = getRequest();

        }
        catch (Exception e) {
            log.error(e.getMessage());
            ((LiteralOutput) out.getParameter("SCRIPT_LIST")).setValue("Failed: " + e.getMessage());
            return;
        }
        ((LiteralOutput) out.getParameter("SCRIPT_LIST")).setValue(resp);

    }

    protected String getRequest() throws Exception {
        String ret = "";

        CloseableHttpClient httpclient = HttpClients.createDefault();
        HttpGet httpget = new HttpGet(coreJobServerUrl + "/hoot-services/ingest/customscript/getlist");
        CloseableHttpResponse response = httpclient.execute(httpget);
        try {

            if (response.getStatusLine().getStatusCode() != 200) {
                String reason = response.getStatusLine().getReasonPhrase();
                if (reason == null) {
                    reason = "Unkown reason.";
                }
                throw new Exception(reason);
            }

            HttpEntity entity = response.getEntity();
            if (entity != null) {
                entity.getContentLength();
                ret = EntityUtils.toString(entity);
            }
        }
        finally {
            response.close();
            httpclient.close();
        }

        return ret;
    }

}
