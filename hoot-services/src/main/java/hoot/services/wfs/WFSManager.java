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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.wfs;

import static hoot.services.HootProperties.WFS_STORE_CONN_NAME;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
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

import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;


public final class WFSManager {
    private static final Logger logger = LoggerFactory.getLogger(WFSManager.class);

    private WFSManager() {}

    public static void createWfsResource(String wfsJobName) throws Exception {
        List<String> tblsList = DbUtils.getTablesList(wfsJobName);
        createWFSDatasourceFeature(wfsJobName, WFS_STORE_CONN_NAME, tblsList);
        createService(wfsJobName);
    }

    public static void removeWfsResource(String wfsJobName) throws Exception {
        removeFeatureStore(wfsJobName);
        removeService(wfsJobName);
    }

    private static void createWFSDatasourceFeature(String wfsDatasetName, String connectionName, List<String> features)
            throws Exception {
        DocumentBuilderFactory dbFactory = XmlDocumentBuilder.getSecureDocBuilderFactory();
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
        Document doc = dBuilder.newDocument();

        Element root = doc.createElement("SQLFeatureStore");
        doc.appendChild(root);
        root.setAttribute("configVersion", "3.2.0");
        root.setAttribute("xmlns", "http://www.deegree.org/datasource/feature/sql");
        root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        root.setAttribute("xsi:schemaLocation", "http://www.deegree.org/datasource/feature/sql" +
                          "  http://schemas.deegree.org/datasource/feature/sql/3.2.0/sql.xsd");

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
        // transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION,"yes");

        String content;

        try (StringWriter writer = new StringWriter()) {
            transformer.transform(new DOMSource(doc), new StreamResult(writer));
            content = writer.getBuffer().toString();
        }

        FeatureStore newFeatureStore = createFeatureStore(wfsDatasetName, content);
        addToWfsStore(newFeatureStore);
    }

    private static void removeFeatureStore(String name) throws Exception {
        // get current workspace
        DeegreeWorkspace workspace = OGCFrontController.getServiceWorkspace();

        FeatureStoreManager featureStoreManager = workspace.getSubsystemManager(FeatureStoreManager.class);
        if (featureStoreManager == null) {
            throw new Exception("Failed to get FeatureStoreManager.");
        }

        FeatureStore featureStore = featureStoreManager.get(name);
        if (featureStore != null) {
            featureStoreManager.deactivate(name);
            featureStoreManager.deleteResource(name);
        }
    }

    private static FeatureStore createFeatureStore(String name, String content) throws Exception {
        // get current workspace
        DeegreeWorkspace wksp = OGCFrontController.getServiceWorkspace();

        FeatureStoreManager fsMan = wksp.getSubsystemManager(FeatureStoreManager.class);
        if (fsMan == null) {
            throw new Exception("Failed to get FeatureStoreManager.");
        }

        ResourceState<?> resourceState;
        try (InputStream stream = new ByteArrayInputStream(content.getBytes("UTF-8"))) {
            resourceState = fsMan.createResource(name, stream);
        }

        if (resourceState.getType() == StateType.deactivated) {
            ResourceState<FeatureStore> fsActivationState = fsMan.activate(name);
            if (fsActivationState.getType() == StateType.init_ok) {
                return fsActivationState.getResource();
            }
            else if (fsActivationState.getType() == StateType.init_error) {
                if (fsActivationState.getLastException() != null) {
                    logger.error("Error encountered during [{}] resource activation.", name, fsActivationState.getLastException());
                    throw new Exception("Error encountered during [" + name + "] resource activation",
                            fsActivationState.getLastException());
                }
            }
        }

        if (resourceState.getLastException() != null) {
            logger.error("Error encountered during [{}] resource creation.", name, resourceState.getLastException());
            throw new Exception("Failed to create [" + name + "] resource!", resourceState.getLastException());
        }
        else {
            throw new Exception("Failed to create [" + name + "] resource!");
        }
    }

    private static void addToWfsStore(FeatureStore fstore) throws Exception {
        if (fstore == null) {
            throw new Exception("fstore argument can not be null");
        }

        DeegreeWorkspace workspace = OGCFrontController.getServiceWorkspace();
        WebServicesConfiguration wsConfig = workspace.getSubsystemManager(WebServicesConfiguration.class);

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

    private static void removeService(String wfsResourceName) {
        DeegreeWorkspace workspace = OGCFrontController.getServiceWorkspace();
        WebServicesConfiguration webServicesConfiguration = workspace.getSubsystemManager(WebServicesConfiguration.class);
        webServicesConfiguration.deactivate(wfsResourceName);
        webServicesConfiguration.deleteResource(wfsResourceName);
    }

    private static void createService(String wfsResourceName) throws Exception {
        DeegreeWorkspace deegreeWorkspace = OGCFrontController.getServiceWorkspace();
        WebServicesConfiguration webServicesConfiguration = deegreeWorkspace.getSubsystemManager(WebServicesConfiguration.class);
        String content = "<deegreeWFS configVersion=\"3.1.0\" xmlns=\"http://www.deegree.org/services/wfs\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                + "  xsi:schemaLocation=\"http://www.deegree.org/services/wfs http://schemas.deegree.org/services/wfs/3.1.0/wfs_configuration.xsd\">\n"
                + "  <SupportedVersions>\n" + "    <Version>1.0.0</Version>\n" + "    <Version>1.1.0</Version>\n"
                + "    <Version>2.0.0</Version>\n" + "  </SupportedVersions>\n"
                + "  <!-- [0..n] FeatureStoreId: If ommited all featurestores will be published as feature types  -->\n"
                + "  <FeatureStoreId>" + wfsResourceName + "</FeatureStoreId>\n" + "  <!--  \n"
                + "  <FeatureStoreId>MyStore2</FeatureStoreId>\n" + "   -->\n"
                + "  <EnableTransactions>false</EnableTransactions>\n" + "  <QueryCRS>EPSG:26912</QueryCRS>\n"
                + "  <QueryCRS>EPSG:4326</QueryCRS>\n" + "  <QueryMaxFeatures>-1</QueryMaxFeatures>\n"
                + "</deegreeWFS>";

        ResourceState<?> resourceState;
        try (InputStream stream = new ByteArrayInputStream(content.getBytes("UTF-8"))) {
            resourceState = webServicesConfiguration.createResource(wfsResourceName, stream);
        }

        if (resourceState.getType() == StateType.init_error) {
            throw new Exception("Failed to create WFS Service resource.");
        }

        if (resourceState.getType() == StateType.deactivated) {
            ResourceState<OWS> owsResStat = webServicesConfiguration.activate(wfsResourceName);
            if (owsResStat.getType() == StateType.init_error) {
                throw new Exception("Failed to activate WFS Service resource.");
            }
        }
    }

    public static List<String> getAllWfsServices() {
        List<String> services = new ArrayList<>();

        try {
            DeegreeWorkspace deegreeWorkspace = OGCFrontController.getServiceWorkspace();
            WebServicesConfiguration wsConfig = deegreeWorkspace.getSubsystemManager(WebServicesConfiguration.class);

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
            String msg = "Error retrieving WFS services: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return services;
    }
}
