package hoot.services;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.HttpHeaders;
import javax.ws.rs.core.Request;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status.Family;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.ext.Provider;

import org.glassfish.jersey.spi.ExtendedExceptionMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


@Provider
public class JerseyExceptionLogger implements ExtendedExceptionMapper<Throwable> {
    private static final Logger logger = LoggerFactory.getLogger(JerseyExceptionLogger.class);

    @Context
    private Request request;

    @Context
    private UriInfo uriInfo;

    @Context
    private HttpHeaders headers;


    @Override
    public boolean isMappable(Throwable exception) {
        String message = buildMessage();

        /* Primarily, we don't want to log client errors (i.e. 400's) as an error. */
        if (isServerError(exception)) {
            logger.error(message, exception);
        }
        else {
            logger.info(message, exception);
        }

        return false;
    }

    @Override
    public Response toResponse(Throwable exception) {
        logger.error("This should not have been called.");
        throw new RuntimeException("This should not have been called");
    }

    private static boolean isServerError(Throwable throwable) {
        /* Note: We consider anything that is not an instance of WebApplicationException a server error. */
        return (throwable instanceof WebApplicationException) && isServerError((WebApplicationException) throwable);
    }

    private static boolean isServerError(WebApplicationException exception) {
        return exception.getResponse().getStatusInfo().getFamily() == Family.SERVER_ERROR;
    }

    private String buildMessage() {
        StringBuilder message = new StringBuilder();

        message.append("Uncaught REST API exception:").append(System.lineSeparator());
        message.append("URL: ").append(listQueryParamNames()).append(System.lineSeparator());
        message.append("Method: ").append(this.request.getMethod()).append(System.lineSeparator());
        message.append("Headers: ").append(listHeaderNames()).append(System.lineSeparator());

        return message.toString();
    }

    private String listQueryParamNames() {
        StringBuilder buffer = new StringBuilder();
        for (String param: this.uriInfo.getQueryParameters().keySet()) {
            buffer.append(param);
            buffer.append(System.lineSeparator());
        }
        return buffer.toString();
    }

    private String listHeaderNames() {
        StringBuilder buffer = new StringBuilder();
        for (String header: this.headers.getRequestHeaders().keySet()) {
            buffer.append(header);
            buffer.append(System.lineSeparator());
        }
        return buffer.toString();
    }
}