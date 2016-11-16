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
package hoot.services;

import java.util.logging.Logger;

import org.glassfish.jersey.filter.LoggingFilter;
import org.glassfish.jersey.media.multipart.MultiPartFeature;
import org.glassfish.jersey.server.ResourceConfig;
import org.glassfish.jersey.server.spring.scope.RequestContextFilter;

import hoot.services.controllers.filters.CorsResponseFilter;


public class HootServicesJerseyApplication extends ResourceConfig {
    private static final Logger logger = Logger.getLogger(HootServicesJerseyApplication.class.getName());

    public HootServicesJerseyApplication() {
        super.packages(true, "hoot.services");

        super.register(MultiPartFeature.class);
        super.register(CorsResponseFilter.class);
        super.register(RequestContextFilter.class);

        /*
        // Could not get LoggingFeature to work for some reason.  Falling back to the deprecated LoggingFilter!
        super.registerInstances(new LoggingFeature(logger,
                                                     Level.ALL,
                                                     LoggingFeature.Verbosity.PAYLOAD_TEXT,
                                                     LoggingFeature.DEFAULT_MAX_ENTITY_SIZE));
        */

        super.registerInstances(new LoggingFilter(logger, true));
    }
}