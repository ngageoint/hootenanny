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

import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.verify;

import java.util.LinkedList;

import org.apache.http.ProtocolVersion;
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
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

import hoot.services.IntegrationTest;
import hoot.services.wps.WpsUtils;


public class CustomScriptDeleteScriptProcessletTest {

    @SuppressWarnings("unused")
    @Test
    @Category(IntegrationTest.class)
    public void testProcess() throws Exception {
        CustomScriptDeleteScriptProcesslet processlet = Mockito.spy(new CustomScriptDeleteScriptProcesslet());

        new BasicHttpResponse(new BasicStatusLine(new ProtocolVersion("HTTP", 1, 1), 200, "OK"));

        doReturn("[{\"NAME\":\"testName\",\"DESCRIPTION\":\"my description\"}]").when(processlet)
                .getRequest(anyString());

        LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();

        allInputs.add(WpsUtils.createLiteralInput("NAME", String.valueOf("testName")));
        ProcessletInputs in = new ProcessletInputs(allInputs);

        ProcessDefinition def = new ProcessDefinition();
        def.setOutputParameters(new OutputParameters());
        LinkedList<ProcessletOutput> allOutputs = new LinkedList<ProcessletOutput>();
        allOutputs.add(WpsUtils.createLiteralOutput("DELETED"));
        final ProcessletOutputs out = new ProcessletOutputs(def, allOutputs);

        processlet.process(in, out, new ProcessExecution(null, null, null, null, out));

        verify(processlet).getRequest(Matchers.matches("testName"));

        final String responseStr = ((LiteralOutputImpl) out.getParameter("DELETED")).getValue();

        assertTrue(responseStr.equals("[{\"NAME\":\"testName\",\"DESCRIPTION\":\"my description\"}]"));

    }
}
