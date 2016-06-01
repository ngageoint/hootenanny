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
package hoot.services.controllers.wfs;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.deegree.commons.config.DeegreeWorkspace;
import org.deegree.commons.config.ResourceState;
import org.deegree.commons.config.ResourceState.StateType;
import org.deegree.feature.persistence.FeatureStore;
import org.deegree.feature.persistence.FeatureStoreManager;
import org.deegree.services.OWS;
import org.deegree.services.controller.OGCFrontController;
import org.deegree.services.controller.WebServicesConfiguration;
import org.deegree.services.wfs.WebFeatureService;
import org.deegree.services.wfs.WfsFeatureStoreManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.HootProperties;
import hoot.services.db.DataDefinitionManager;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.utils.XmlDocumentBuilder;


public class WfsManager {
    private static final Logger log = LoggerFactory.getLogger(WfsManager.class);

    private static final String coreJobServerUrl = HootProperties.getProperty("coreJobServerUrl");
    private static final String wfsStoreConnName = HootProperties.getProperty("wfsStoreConnName");
    private static final String wfsStoreDb = HootProperties.getProperty("wfsStoreDb");

    public void createWfsResource(String wfsJobName) throws Exception {
        DataDefinitionManager ddlMan = new DataDefinitionManager();

        List<String> tblsList = ddlMan.getTablesList(wfsStoreDb, wfsJobName);
        createWFSDatasourceFeature(wfsJobName, wfsStoreConnName, tblsList);
        createService(wfsJobName);
    }

    public void removeWfsResource(String wfsJobName) throws Exception {
        removeFeatureStore(wfsJobName);
        removeService(wfsJobName);
    }

    void createWFSDatasourceFeature(String wfsDatasetName, String connectionName, List<String> features)
            throws Exception {
        DocumentBuilderFactory dbFactory = XmlDocumentBuilder.getSecureDocBuilderFactory();
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
        Document doc = dBuilder.newDocument();

        Element root = doc.createElement("SQLFeatureStore");
        doc.appendChild(root);
        root.setAttribute("configVersion", "3.2.0");
        root.setAttribute("xmlns", "http://www.deegree.org/datasource/feature/sql");
        root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        root.setAttribute("xsi:schemaLocation", "http://www.deegree.org/datasource/feature/sql"
                + "  http://schemas.deegree.org/datasource/feature/sql/3.2.0/sql.xsd");

        Element elem = doc.createElement("JDBCConnId");
        elem.appendChild(doc.createTextNode(connectionName));
        root.appendChild(elem);

        for (String featureName : features) {
            elem = doc.createElement("FeatureTypeMapping");
            elem.setAttribute("table", featureName);
            root.appendChild(elem);
        }

        TransformerFactory transformerFactory = TransformerFactory.newInstance();
        // Fortify may require this, but it doesn't work.
        // TransformerFactory transformerFactory =
        // XmlDocumentBuilder.getSecureTransformerFactory();
        Transformer transformer = transformerFactory.newTransformer();
        // transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION,
        // "yes");
        StringWriter writer = new StringWriter();
        transformer.transform(new DOMSource(doc), new StreamResult(writer));
        String content = writer.getBuffer().toString();
        /*
         * _putRequest(coreJobServerUrl +
         * "/hoot-services/config/upload/datasources/feature/" + wfsDatasetName
         * + ".xml", content);
         */

        try {
            FeatureStore newFeatureStore = createFeatureStore(wfsDatasetName, content);
            addToWfsStore(newFeatureStore);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

    }

    private void removeFeatureStore(String name) throws Exception {
        try {
            // get current workspace
            DeegreeWorkspace wksp = OGCFrontController.getServiceWorkspace();

            FeatureStoreManager fsMan = wksp.getSubsystemManager(FeatureStoreManager.class);
            if (fsMan == null) {
                throw new Exception("Failed to get FeatureStoreManager.");
            }

            FeatureStore fStore = fsMan.get(name);
            if (fStore != null) {
                fsMan.deactivate(name);
                fsMan.deleteResource(name);
            }
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
    }

    FeatureStore createFeatureStore(String name, String content) throws Exception {
        // get current workspace
        DeegreeWorkspace wksp = OGCFrontController.getServiceWorkspace();

        FeatureStoreManager fsMan = wksp.getSubsystemManager(FeatureStoreManager.class);
        if (fsMan == null) {
            throw new Exception("Failed to get FeatureStoreManager.");
        }

        InputStream stream = new ByteArrayInputStream(content.getBytes("UTF-8"));
        ResourceState<?> resStatus = fsMan.createResource(name, stream);

        if (resStatus.getType() == StateType.deactivated) {
            ResourceState<FeatureStore> fsActivationState = fsMan.activate(name);
            if (fsActivationState.getType() == StateType.init_ok) {
                return fsActivationState.getResource();
            }
            else if (fsActivationState.getType() == StateType.init_error) {
                if (fsActivationState.getLastException() != null) {
                    log.error("Error encountered during [" + name + "] resource activation.",
                            fsActivationState.getLastException());
                    throw new Exception("Error encountered during [" + name + "] resource activation",
                            fsActivationState.getLastException());
                }
            }
        }

        if (resStatus.getLastException() != null) {
            log.error("Error encountered during [" + name + "] resource creation.", resStatus.getLastException());
            throw new Exception("Failed to create [" + name + "] resource!", resStatus.getLastException());
        }
        else {
            throw new Exception("Failed to create [" + name + "] resource!");
        }
    }

    void addToWfsStore(FeatureStore fstore) throws Exception {
        try {
            if (fstore == null) {
                throw new Exception("fstore argument can not be null");
            }
            DeegreeWorkspace wksp = OGCFrontController.getServiceWorkspace();

            WebServicesConfiguration wsConfig = wksp.getSubsystemManager(WebServicesConfiguration.class);

            Map<String, List<OWS>> all = wsConfig.getAll();
            List<OWS> owsList = all.get("WFS");
            if (owsList != null) {
                for (OWS curows : owsList) {
                    if (curows instanceof WebFeatureService) {
                        WebFeatureService wfsows = (WebFeatureService) curows;
                        WfsFeatureStoreManager wfsStoreMan = wfsows.getStoreManager();
                        wfsStoreMan.addStore(fstore);
                        break;
                    }
                }
            }
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
    }

    private void removeService(String wfsResourceName) throws Exception {
        DeegreeWorkspace wksp = OGCFrontController.getServiceWorkspace();

        WebServicesConfiguration wsMan = wksp.getSubsystemManager(WebServicesConfiguration.class);
        try {
            wsMan.deactivate(wfsResourceName);
            wsMan.deleteResource(wfsResourceName);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
    }

    private void createService(String wfsResourceName) throws Exception {
        DeegreeWorkspace wksp = OGCFrontController.getServiceWorkspace();

        WebServicesConfiguration wsMan = wksp.getSubsystemManager(WebServicesConfiguration.class);
        try {
            String content = "<deegreeWFS configVersion=\"3.1.0\" xmlns=\"http://www.deegree.org/services/wfs\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                    + "  xsi:schemaLocation=\"http://www.deegree.org/services/wfs http://schemas.deegree.org/services/wfs/3.1.0/wfs_configuration.xsd\">\n"
                    + "  <SupportedVersions>\n" + "    <Version>1.0.0</Version>\n" + "    <Version>1.1.0</Version>\n"
                    + "    <Version>2.0.0</Version>\n" + "  </SupportedVersions>\n"
                    + "  <!-- [0..n] FeatureStoreId: If ommited all featurestores will be published as feature types  -->\n"
                    +

                    "  <FeatureStoreId>" + wfsResourceName + "</FeatureStoreId>\n" + "  <!--  \n"
                    + "  <FeatureStoreId>MyStore2</FeatureStoreId>\n" + "   -->\n"
                    + "  <EnableTransactions>false</EnableTransactions>\n" + "  <QueryCRS>EPSG:26912</QueryCRS>\n"
                    + "  <QueryCRS>EPSG:4326</QueryCRS>\n" + "  <QueryMaxFeatures>-1</QueryMaxFeatures>\n"
                    + "</deegreeWFS>";
            InputStream stream = new ByteArrayInputStream(content.getBytes("UTF-8"));
            ResourceState<?> resStatus = wsMan.createResource(wfsResourceName, stream);

            if (resStatus.getType() == StateType.init_error) {
                throw new Exception("Failed to create WFS Service resource.");
            }
            if (resStatus.getType() == StateType.deactivated) {
                ResourceState<OWS> owsResStat = wsMan.activate(wfsResourceName);
                if (owsResStat.getType() == StateType.init_error) {
                    throw new Exception("Failed to activate WFS Service resource.");
                }
            }
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
    }

    public List<String> getAllWfsServices() throws Exception {
        List<String> services = new ArrayList<>();
        try {
            DeegreeWorkspace wksp = OGCFrontController.getServiceWorkspace();

            WebServicesConfiguration wsConfig = wksp.getSubsystemManager(WebServicesConfiguration.class);

            Map<String, List<OWS>> all = wsConfig.getAll();
            List<OWS> owsList = all.get("WFS");

            if (owsList != null) {
                for (OWS curows : owsList) {
                    if (curows instanceof WebFeatureService) {
                        WebFeatureService wfsows = (WebFeatureService) curows;
                        services.add(wfsows.getId());
                    }
                }
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error retrieving WFS services: " + ex, Status.INTERNAL_SERVER_ERROR, log);
        }
        return services;
    }
}
