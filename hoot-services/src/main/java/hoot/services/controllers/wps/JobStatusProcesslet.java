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
import org.deegree.services.wps.input.LiteralInput;
import org.deegree.services.wps.output.LiteralOutput;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class JobStatusProcesslet extends BaseProcesslet {
    private static final Logger log = LoggerFactory.getLogger(JobStatusProcesslet.class);

    public JobStatusProcesslet() throws Exception {
        super();

    }

    @Override
    public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info)
            throws ProcessletException {

        try {
            String jobId = ((LiteralInput) in.getParameter("jobid")).getValue().trim();
            String strStatus = getJobStatusRquest(jobId);
            ((LiteralOutput) out.getParameter("status")).setValue(strStatus);
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }

    }

    private static String getJobStatusRquest(String jobId) throws Exception {
        String ret = "";

        CloseableHttpClient httpclient = HttpClients.createDefault();
        HttpGet httpget = new HttpGet(coreJobServerUrl + "/hoot-services/job/status/" + jobId);
        CloseableHttpResponse response = httpclient.execute(httpget);
        try {
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
