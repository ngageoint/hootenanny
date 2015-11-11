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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.wps;

import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.*;
import static org.junit.Assert.*;

import java.util.LinkedList;





import hoot.services.IntegrationTest;
import hoot.services.wps.WpsUtils;

import org.apache.http.HttpResponse;
import org.apache.http.ProtocolVersion;
import org.apache.http.entity.StringEntity;
import org.apache.http.message.BasicHttpResponse;
import org.apache.http.message.BasicStatusLine;
import org.deegree.process.jaxb.java.ProcessDefinition;
import org.deegree.process.jaxb.java.ProcessDefinition.OutputParameters;
import org.deegree.services.wps.ProcessExecution;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.input.ProcessletInput;
import org.deegree.services.wps.output.LiteralOutputImpl;
import org.deegree.services.wps.output.ProcessletOutput;
import org.json.simple.parser.JSONParser;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

public class CustomScriptSaveProcessletTest {

	@SuppressWarnings("unused")
  private JSONParser _parser = new JSONParser();
  
	@Test
	@Category(IntegrationTest.class)
	public void testProcess() throws Exception
	{
		CustomScriptSaveProcesslet processlet = Mockito.spy(new CustomScriptSaveProcesslet());

		HttpResponse mockResp = new BasicHttpResponse(
        new BasicStatusLine(new ProtocolVersion("HTTP", 1, 1), 200, "OK"));
		
		StringEntity se = new StringEntity( "[{\"NAME\":\"testName\",\"DESCRIPTION\":\"test description\"}]");  
		mockResp.setEntity(se);

		doReturn(mockResp).when(processlet).postRequest(anyString(), anyString(), anyString());

		LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();
		
		
		allInputs.add(WpsUtils.createLiteralInput("NAME", String.valueOf("testName")));
		allInputs.add(WpsUtils.createLiteralInput("DESCRIPTION", String.valueOf("test description")));
		allInputs.add(WpsUtils.createLiteralInput("SCRIPT", String.valueOf("test script")));		
		ProcessletInputs in = new ProcessletInputs(allInputs);
    
    ProcessDefinition def = new ProcessDefinition();
    def.setOutputParameters(new OutputParameters());		
		LinkedList<ProcessletOutput> allOutputs = new LinkedList<ProcessletOutput>();    
    allOutputs.add(WpsUtils.createLiteralOutput("SAVEDITEM"));   
    final ProcessletOutputs out = new ProcessletOutputs(def, allOutputs);
    
    processlet.process(in, out, new ProcessExecution(null, null, null, null, out));
    
    verify(processlet).postRequest(Matchers.matches("testName"), Matchers.matches("test description"), Matchers.matches("test script"));
		

    final String responseStr = 
        ((LiteralOutputImpl)out.getParameter("SAVEDITEM")).getValue();
    
    assertTrue(responseStr.equals("[{\"NAME\":\"testName\",\"DESCRIPTION\":\"test description\"}]"));
    
	}	
}
