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
package hoot.services.wps;

import java.io.IOException;
import java.lang.reflect.Method;

import javax.servlet.ServletConfig;
import javax.servlet.ServletContext;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.ws.rs.core.Response.Status;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpressionException;

import org.deegree.services.controller.OGCFrontController;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.mockito.Mockito;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.utils.MockServletInputStream;
import hoot.services.utils.MockServletOutputStream;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.utils.XmlUtils;


/*
 * Having this inherit from OsmResourceTestAbstract is kind of messy and confusing.  It is
 * necessary for now, however, because OsmResourceTestAbstract loads test map/user records before
 * tests and cleans the database out after tests, which this class needs.  Better separation 
 * should occur between the Jersey test classes and the WPS test classes.
 */
public abstract class WpsTestAbstract extends OsmResourceTestAbstract {
    public WpsTestAbstract(String controllerGroup) throws NumberFormatException, IOException {
        super(controllerGroup);
    }

    private static final Logger log = LoggerFactory.getLogger(WpsTestAbstract.class);

    protected static OGCFrontController wpsController;

    protected String processId;

    @BeforeClass
    public static void beforeClass() throws Exception {
        try {
            OsmResourceTestAbstract.beforeClass();

            System.setProperty("DEEGREE_WORKSPACE_ROOT",
                    System.getenv("HOOT_HOME") + "/hoot-services/src/main/webapp/WEB-INF");
            final ServletContext context = Mockito.mock(ServletContext.class);
            Mockito.when(context.getContextPath()).thenReturn("ogc");
            final ServletConfig config = Mockito.mock(ServletConfig.class);
            Mockito.when(config.getServletContext()).thenReturn(context);
            if (wpsController == null) {
                wpsController = new OGCFrontController();
                wpsController.init(config);
            }
        }
        catch (Exception e) {
            log.error(e.getMessage() + " ");
            throw e;
        }
    }

    @AfterClass
    public static void afterClass() throws Exception {
        OsmResourceTestAbstract.afterClass();
    }

    protected abstract String getWpsRequestXml(final String mapId) throws Exception;

    protected abstract String getWpsRequestString(final String mapId) throws Exception;

    protected String testWps(final boolean kvp, final boolean getRequest, final String mapId) throws Exception {
        final HttpServletRequest request = Mockito.mock(HttpServletRequest.class);
        Mockito.when(request.getRequestURL()).thenReturn(new StringBuffer("http://localhost:8080/hoot/ogc"));
        Mockito.when(request.getServletPath()).thenReturn("/ogc");
        if (getRequest) {
            String queryString = null;
            if (kvp) {
                queryString = getWpsRequestString(mapId);
            }
            else {
                queryString = getWpsRequestXml(mapId);
                // System.out.println(queryString);
                Assert.assertNotNull(XmlDocumentBuilder.parse(queryString));
                // queryString =
                // /*StringEscapeUtils.escapeXml(*/queryString/*)*/;
                ServletInputStream inputStream = new MockServletInputStream(queryString);
                Mockito.when(request.getInputStream()).thenReturn(inputStream);
            }
            Mockito.when(request.getQueryString()).thenReturn(queryString);
        }
        else {
            String postBody = null;
            if (kvp) {
                Mockito.when(request.getContentType()).thenReturn("application/x-www/form-urlencoded");
                postBody = getWpsRequestString(mapId);
            }
            else {
                postBody = getWpsRequestXml(mapId);
                // System.out.println(postBody);
                Assert.assertNotNull(XmlDocumentBuilder.parse(postBody));
            }
            ServletInputStream inputStream = new MockServletInputStream(postBody);
            Mockito.when(request.getInputStream()).thenReturn(inputStream);
        }
        HttpServletResponse response = Mockito.mock(HttpServletResponse.class);
        MockServletOutputStream outputStream = new MockServletOutputStream();
        Mockito.when(response.getOutputStream()).thenReturn(outputStream);

        Class<?>[] cArg = new Class[2];
        cArg[0] = HttpServletRequest.class;
        cArg[1] = HttpServletResponse.class;
        String methodName = "doGet";
        if (!getRequest) {
            methodName = "doPost";
        }
        Method method = OGCFrontController.class.getDeclaredMethod(methodName, cArg);
        method.setAccessible(true);
        method.invoke(wpsController, request, response);

        return outputStream.baos.toString();
    }

    protected abstract String verifyWpsResponse(final String responseStr) throws Exception;

    protected void verifyWpsErrorResponse(final String responseStr, final Status errorStatus, final String errorMessage)
            throws Exception {
        final Document responseData = XmlDocumentBuilder.parse(responseStr);
        Assert.assertNotNull(responseData);
        // System.out.println(XmlDocumentBuilder.toString(responseData));

        XPath xpath = XmlUtils.createXPath();
        try {
            Assert.assertEquals(processId, xpath.evaluate(".//Process/Identifier", responseData));
            Assert.assertEquals(errorStatus.getStatusCode() + ": " + errorStatus.getReasonPhrase(),
                    xpath.evaluate(".//Status/ProcessFailed/ExceptionReport/Exception/@exceptionCode", responseData));
            Assert.assertTrue(
                    xpath.evaluate(".//Status/ProcessFailed/ExceptionReport/Exception/ExceptionText", responseData)
                            .contains(errorMessage));
        }
        catch (XPathExpressionException e) {
            Assert.fail("Error parsing response document: " + e.getMessage());
        }
    }
}
