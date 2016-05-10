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
package hoot.services.controllers.info;

import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import hoot.services.HootProperties;
import hoot.services.info.BuildInfo;
import hoot.services.info.CoreDetail;
import hoot.services.info.ServicesDetail;
import hoot.services.info.ServicesDetail.Property;
import hoot.services.info.ServicesDetail.ServicesResource;
import hoot.services.info.VersionInfo;
import hoot.services.nativeInterfaces.JobExecutionManager;
import hoot.services.utils.ClassLoaderUtil;
import hoot.services.utils.ResourceErrorHandler;


/**
 * Endpoint for returning information about Hootenanny core and services
 */
@Path("/about")
public class AboutResource {
    private static final Logger log = LoggerFactory.getLogger(AboutResource.class);

    private static ClassPathXmlApplicationContext appContext;


    public AboutResource() {
        appContext = new ClassPathXmlApplicationContext("hoot/spring/CoreServiceContext.xml");
    }

    private static Properties getBuildInfo() {
        Properties buildInfo;

        try {
            buildInfo = BuildInfo.getInstance();
        }
        catch (Exception e) {
            log.warn("About Resource unable to find the services build.info file.  " +
                     "Web Services version information will be unavailable.");

            buildInfo = new Properties();
            buildInfo.setProperty("name", "unknown");
            buildInfo.setProperty("version", "unknown");
            buildInfo.setProperty("user", "unknown");
        }

        return buildInfo;
    }

    /**
     * Service method endpoint for retrieving the Hootenanny services version.
     * <p>
     * GET hoot-services/info/about/servicesVersionInfo
     *
     * @return JSON containing Hoot service version information
     */
    @GET
    @Path("/servicesVersionInfo")
    @Produces(MediaType.APPLICATION_JSON)
    public VersionInfo getServicesVersionInfo() {
        VersionInfo versionInfo = null;
        try {
            log.debug("Retrieving services version...");

            Properties buildInfo = getBuildInfo();
            versionInfo = new VersionInfo();
            versionInfo.setName(buildInfo.getProperty("name"));
            versionInfo.setVersion(buildInfo.getProperty("version"));
            versionInfo.setBuiltBy(buildInfo.getProperty("user"));

            log.debug("Returning response: " + versionInfo + " ...");
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError(
                    "Error retrieving services version info: " + e.getMessage(),
                    Status.INTERNAL_SERVER_ERROR,
                    log);
        }

        return versionInfo;
    }

    /**
     * Service method endpoint for retrieving withDetails information about the Hootenanny Services
     * environment.
     * <p>
     * GET hoot-services/info/about/servicesVersionDetail
     *
     * @return JSON Array containing Hoot service configuration detail
     */
    @GET
    @Path("/servicesVersionDetail")
    @Produces(MediaType.APPLICATION_JSON)
    public ServicesDetail getServicesVersionDetail() {
        ServicesDetail servicesDetail = null;
        try {
            log.debug("Retrieving services version...");

            servicesDetail = new ServicesDetail();

            List<Property> properties = new ArrayList<>();
            Properties props = HootProperties.getInstance();

            for (Object o : props.entrySet()) {
                Property prop = new Property();

                @SuppressWarnings("rawtypes")
                Map.Entry parsedProp = (Map.Entry) o;

                prop.setName((String) parsedProp.getKey());
                prop.setValue((String) parsedProp.getValue());
                properties.add(prop);
            }

            servicesDetail.setProperties(properties.toArray(new Property[properties.size()]));
            servicesDetail.setClassPath(System.getProperty("java.class.path", null));

            List<ServicesResource> resources = new ArrayList<>();

            for (String url : ClassLoaderUtil.getMostJars()) {
                ServicesResource servicesResource = new ServicesResource();
                servicesResource.setType("jar");
                servicesResource.setUrl(url);
                resources.add(servicesResource);
            }

            ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
            Enumeration<URL> urls = classLoader.getResources("");
            while (urls.hasMoreElements()) {
                URL resource = urls.nextElement();
                ServicesResource servicesResource = new ServicesResource();
                servicesResource.setType(resource.getProtocol());
                servicesResource.setUrl(resource.toString());
                resources.add(servicesResource);
            }

            servicesDetail.setResources(resources.toArray(new ServicesResource[resources.size()]));

            log.debug("Returning response: " + servicesDetail + " ...");
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError(
                    "Error retrieving services version info: " + e.getMessage(),
                    Status.INTERNAL_SERVER_ERROR,
                    log);
        }

        return servicesDetail;
    }

    private static String getCoreInfo(boolean withDetails) throws Exception {
        JSONObject command = new JSONObject();
        command.put("exectype", "hoot");
        command.put("exec", "version");

        JSONArray params = new JSONArray();

        if (withDetails) {
            JSONObject param = new JSONObject();
            param.put("", "--debug");
            params.add(param);
        }

        command.put("params", params);
        command.put("caller", AboutResource.class.getSimpleName());

        JobExecutionManager jobExecutionManager = ((JobExecutionManager) appContext.getBean("jobExecutionManagerNative"));

        String output = jobExecutionManager.execWithResult(command).get("stdout").toString();

        return parseCoreVersionOutOf(output, withDetails);
    }

    private static String parseCoreVersionOutOf(String text, boolean withDetails) {
        String coreVersion = "Unable to determine!";

        // "\\r?\\n" regexp will cover Unix, Linux, and Windows
        String[] lines = text.split("\\r?\\n");
        for (int i = 0; i < lines.length; i++) {
            String line = lines[i].trim();
            if (line.startsWith("Hootenanny") && line.contains("Built By:")) {
                coreVersion = line;
                if (withDetails) {
                    if ((i + 1) < lines.length) {
                        coreVersion += System.lineSeparator();
                        for (int j = i + 1; j < lines.length; j++) {
                            coreVersion += lines[j];
                            coreVersion += System.lineSeparator();
                        }
                    }
                }
                break;
            }
        }

        return coreVersion;
    }

    /**
     * Service method endpoint for retrieving the Hootenanny core (command line application) version.
     * <p>
     * GET hoot-services/info/about/coreVersionInfo</URL>
     *
     * @return JSON containing Hoot core version information
     */
    @GET
    @Path("/coreVersionInfo")
    @Produces(MediaType.APPLICATION_JSON)
    public VersionInfo getCoreVersionInfo() {
        VersionInfo versionInfo = null;
        try {
            log.debug("Retrieving services version...");

            String versionStr = getCoreInfo(false);
            String[] versionInfoParts = versionStr.split(" ");
            versionInfo = new VersionInfo();
            versionInfo.setName("Hootenanny Core");
            versionInfo.setVersion(versionInfoParts[1]);
            versionInfo.setBuiltBy(versionInfoParts[4]);

            log.debug("Returning response: " + versionInfo + " ...");
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError(
                    "Error retrieving core version info: " + e.getMessage(),
                    Status.INTERNAL_SERVER_ERROR,
                    log);
        }

        return versionInfo;
    }

    /**
     * Service method endpoint for retrieving withDetails environment information about the Hootenanny
     * core (command line application)
     * <p>
     * GET hoot-services/info/about/coreVersionDetail</URL>
     *
     * @return JSON Array containing Hoot core version detail
     */
    @GET
    @Path("/coreVersionDetail")
    @Produces(MediaType.APPLICATION_JSON)
    public CoreDetail getCoreVersionDetail() {
        CoreDetail coreDetail = null;
        try {
            log.debug("Retrieving services version...");

            String versionStr = getCoreInfo(true);

            //get rid of the first line that has the hoot core version info in it; call coreVersionInfo for that
            String[] versionInfoParts = versionStr.split(System.lineSeparator());
            List<String> versionInfoPartsModified = new ArrayList<>();

            for (int i = 1; i < versionInfoParts.length; i++) {
                versionInfoPartsModified.add(versionInfoParts[i]);
            }

            coreDetail = new CoreDetail();
            coreDetail.setEnvironmentInfo(versionInfoPartsModified.toArray(new String[versionInfoPartsModified.size()]));

            log.debug("Returning response: " + coreDetail + " ...");
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError(
                    "Error retrieving core version info: " + e.getMessage(),
                    Status.INTERNAL_SERVER_ERROR,
                    log);
        }

        return coreDetail;
    }
}
