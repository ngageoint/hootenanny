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

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import org.slf4j.bridge.SLF4JBridgeHandler;

import hoot.services.controllers.ingest.BasemapResource;
import hoot.services.controllers.ogr.TranslatorResource;
import hoot.services.controllers.services.P2PResource;


public class HootServletContext implements ServletContextListener {

    private TranslatorResource transRes;
    private P2PResource p2PRes;

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        transRes = new TranslatorResource();
        transRes.startTranslationService();

        p2PRes = new P2PResource();
        p2PRes.startP2PService();

        // Doing this to make sure we create ingest folder
        BasemapResource bRes = new BasemapResource();
        bRes.createTileServerPath();

        // Bridge/route all JUL log records to the SLF4J API.
        // Some third-party components use Java Util Logging (JUL). We want to
        // route those calls
        // through SLF4J.
        initSLF4JBridgeHandler();
    }

    private void initSLF4JBridgeHandler() {
        // Optionally remove existing handlers attached to j.u.l root logger
        SLF4JBridgeHandler.removeHandlersForRootLogger(); // (since SLF4J 1.6.5)

        // add SLF4JBridgeHandler to j.u.l's root logger, should be done once
        // during
        // the initialization phase of your application
        SLF4JBridgeHandler.install();
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        transRes.stopTranslationService();
        p2PRes.stopP2PService();
    }
}
