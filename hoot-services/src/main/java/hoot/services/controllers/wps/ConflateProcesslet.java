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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.wps;

import hoot.services.HootProperties;

import java.io.IOException;

import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ConflateProcesslet extends JobProcesslet {
  private static final Logger log = LoggerFactory.getLogger(ConflateProcesslet.class);
  
  private String makefilePath = null;

	public ConflateProcesslet() throws Exception
	{
    try {
    	makefilePath = HootProperties.getProperty("ConflateMakefilePath");
    	this.setProcessScriptName(makefilePath);
		} catch (IOException e) {
			log.error(e.getMessage());
		}
	}
	
	@Override
	public void process(ProcessletInputs in, ProcessletOutputs out, ProcessletExecutionInfo info) 
			throws ProcessletException {
		
		JSONArray args = parseRequestParams(in);
		
		try {		
			String confOutputName = null;
			for(int i=0; i<args.size(); i++)
			{
				JSONObject arg = (JSONObject)args.get(i);
				Object val = arg.get("OUTPUT_NAME");
				
				if(val != null)
				{
					confOutputName = val.toString();
					break;
				}
			}
			JSONObject conflationCommand = _createPostBody(args);
			JSONArray reviewArgs = new JSONArray();
			JSONObject param = new JSONObject();
			param.put("value", confOutputName);
			param.put("paramtype", String.class.getName());
			param.put("isprimitivetype", "false");
			reviewArgs.add(param);
			
			param = new JSONObject();
			param.put("value", false);
			param.put("paramtype", Boolean.class.getName());
			param.put("isprimitivetype", "true");
			reviewArgs.add(param);
			
			JSONObject prepareItemsForReviewCommand = _createReflectionJobReq(reviewArgs, "hoot.services.controllers.job.ReviewResource",
					"prepareItemsForReview");
			
			JSONArray jobArgs = new JSONArray();
			jobArgs.add(conflationCommand);
			jobArgs.add(prepareItemsForReviewCommand);
			
			
			postChainJobRquest( jobIdStr,  jobArgs.toJSONString());	
			
			((LiteralOutput)out.getParameter("jobId")).setValue(jobIdStr);
		} catch (Exception e) {
			log.error(e.getMessage());
		}
		

	}
}
