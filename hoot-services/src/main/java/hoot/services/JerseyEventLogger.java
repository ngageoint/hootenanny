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

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.Request;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.ext.Provider;

import org.glassfish.jersey.server.monitoring.ApplicationEvent;
import org.glassfish.jersey.server.monitoring.ApplicationEventListener;
import org.glassfish.jersey.server.monitoring.RequestEvent;
import org.glassfish.jersey.server.monitoring.RequestEventListener;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


@Provider
public class JerseyEventLogger implements ApplicationEventListener, RequestEventListener {
    private static final Logger logger = LoggerFactory.getLogger(JerseyEventLogger.class);

    @Context
    private Request request;

    @Context
    private UriInfo uriInfo;


    @Override
    public void onEvent(ApplicationEvent applicationEvent) {}

    @Override
    public RequestEventListener onRequest(RequestEvent requestEvent) {
        return this;
    }

    @Override
    public void onEvent(RequestEvent event) {
        if(event.getType() == RequestEvent.Type.ON_EXCEPTION) {
            logException(event.getException());
        }
        else if (event.getType() == RequestEvent.Type.RESOURCE_METHOD_START) {
            logger.debug("Entering {} endpoint's handler {}",
                    uriInfo.getAbsolutePath(), event.getUriInfo().getMatchedResourceMethod());
        }
        else if (event.getType() == RequestEvent.Type.RESOURCE_METHOD_FINISHED) {
            logger.debug("Exiting {} endpoint's handler {}",
                    uriInfo.getAbsolutePath(), event.getUriInfo().getMatchedResourceMethod());
        }
    }

    private void logException(Throwable exception) {
        String message = buildMessage();

        /* Primarily, we don't want to log client errors (i.e. 400's) as an error. */
        if (isServerError(exception)) {
            logger.error(message, exception);
        }
        else {
            logger.info(message, exception);
        }
    }

    private static boolean isServerError(Throwable throwable) {
        return (throwable instanceof WebApplicationException) && isServerError((WebApplicationException) throwable);
    }

    private static boolean isServerError(WebApplicationException exception) {
        return exception.getResponse().getStatusInfo().getFamily() == Response.Status.Family.SERVER_ERROR;
    }

    private String buildMessage() {
        StringBuilder message = new StringBuilder();

        message.append("Uncaught REST API exception:").append(System.lineSeparator());
        message.append("URL: ").append(uriInfo.getAbsolutePath()).append(System.lineSeparator());
        message.append("Method: ").append(this.request.getMethod()).append(System.lineSeparator());

        return message.toString();
    }
}