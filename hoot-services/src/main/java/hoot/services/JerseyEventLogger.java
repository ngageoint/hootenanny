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