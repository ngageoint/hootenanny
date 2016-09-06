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
package hoot.services;

import static hoot.services.HootProperties.TILE_SERVER_PATH;

import java.io.File;
import java.io.IOException;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import org.apache.commons.io.FileUtils;
import org.slf4j.bridge.SLF4JBridgeHandler;

import hoot.services.controllers.ogr.TranslatorResource;
import hoot.services.controllers.ogr.P2PResource;


public class HootServletContext implements ServletContextListener {

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        TranslatorResource.startTranslationService();
        P2PResource.startP2PService();

        // Bridge/route all JUL log records to the SLF4J API.
        // Some third-party components use Java Util Logging (JUL). We want to
        // route those calls through SLF4J.
        initSLF4JBridgeHandler();

        HootProperties.init();

        // Doing this to make sure we create ingest folder
        try {
            createTileServerPath(TILE_SERVER_PATH);
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error creating tile server path: " + TILE_SERVER_PATH, ioe);
        }
    }

    private static void initSLF4JBridgeHandler() {
        // Optionally remove existing handlers attached to j.u.l root logger
        SLF4JBridgeHandler.removeHandlersForRootLogger(); // (since SLF4J 1.6.5)

        // add SLF4JBridgeHandler to j.u.l's root logger, should be done once
        // during the initialization phase of your application
        SLF4JBridgeHandler.install();
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        TranslatorResource.stopTranslationService();
        P2PResource.stopP2PService();
    }

    private static void createTileServerPath(String path) throws IOException {
        File file = new File(path);
        if (!file.exists()) {
            FileUtils.forceMkdir(file);
        }
    }
}
