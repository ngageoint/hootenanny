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
import static org.mockito.Mockito.doThrow;

import java.util.LinkedList;

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
import org.mockito.Mockito;

import hoot.services.IntegrationTest;
import hoot.services.wps.WpsUtils;


public class CustomScriptGetListProcessletTest {

    @Test
    @Category(IntegrationTest.class)
    public void testProcess() throws Exception {
        CustomScriptGetListProcesslet processlet = Mockito.spy(new CustomScriptGetListProcesslet());

        doThrow(new Exception("Mock Test Error for CustomScriptGetListProcessletTest. (Not real error)"))
                .when(processlet).getRequest();
        LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();

        ProcessletInputs in = new ProcessletInputs(allInputs);

        ProcessDefinition def = new ProcessDefinition();
        def.setOutputParameters(new OutputParameters());
        LinkedList<ProcessletOutput> allOutputs = new LinkedList<ProcessletOutput>();
        allOutputs.add(WpsUtils.createLiteralOutput("SCRIPT_LIST"));
        final ProcessletOutputs out = new ProcessletOutputs(def, allOutputs);

        processlet.process(in, out, new ProcessExecution(null, null, null, null, out));

        final String responseStr = ((LiteralOutputImpl) out.getParameter("SCRIPT_LIST")).getValue();

        assertTrue(
                responseStr.equals("Failed: Mock Test Error for CustomScriptGetListProcessletTest. (Not real error)"));

    }
}
