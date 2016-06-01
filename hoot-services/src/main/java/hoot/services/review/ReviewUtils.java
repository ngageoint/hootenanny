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
package hoot.services.review;

import java.sql.SQLException;

import javax.ws.rs.core.Response.Status;

import org.apache.commons.lang3.StringUtils;
import org.deegree.commons.ows.exception.OWSException;
import org.deegree.services.wps.ProcessletException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.utils.ResourceErrorHandler;


/**
 * Various utilities for conflated data reviewing
 */
public class ReviewUtils {
    private static final Logger log = LoggerFactory.getLogger(ReviewUtils.class);

    /**
     * Handles all thrown exceptions from both WPS and non-WPS review services
     *
     * @param e
     *            a thrown exception
     * @param errorMessageStart
     *            text to prepend to the error message
     * @param throwWpsError
     *            if true; throws a deegree ProcessletException; otherwise a
     *            Jersey WebApplicationException is thrown
     * @throws Exception
     *             //TODO: go through and clean out these message text checks
     */
    public static void handleError(final Exception e, final String errorMessageStart, final boolean throwWpsError)
            throws Exception {
        Status status = null;
        if (!StringUtils.isEmpty(e.getMessage())) {
            if (e.getMessage().contains("Invalid input parameter") || e.getMessage().contains("Invalid reviewed item")
                    || e.getMessage().contains("Error parsing unique ID tag") || e.getMessage().contains("empty String")
                    || e.getMessage().contains("Invalid coordinate")) {
                status = Status.BAD_REQUEST;
            }
            else if (e.getMessage().contains("record exists") || e.getMessage().contains("records exist")
                    || e.getMessage().contains("to be updated does not exist")
                    || e.getMessage().contains("does not exist")) {
                status = Status.NOT_FOUND;
            }
            else if (e.getMessage().contains("Invalid version") || e.getMessage().contains("Invalid changeset ID")
                    || e.getMessage().contains("references itself")
                    || e.getMessage().contains("Changeset maximum element threshold exceeded")
                    || e.getMessage().contains("was closed at") || e.getMessage().contains("has become out of sync")) {
                status = Status.CONFLICT;
            }
            else if (e.getMessage().contains("exist specified for") || e.getMessage().contains("exist for")
                    || e.getMessage().contains("is still used by")) {
                status = Status.PRECONDITION_FAILED;
            }
        }
        if (status == null) {
            status = Status.INTERNAL_SERVER_ERROR;
        }
        String message = "Error " + errorMessageStart + ": ";
        if (e.getMessage() != null && e.getMessage().contains("empty String")) {
            // added for giving a better error message when passing invalid
            // params to jersey
            message += "Invalid input parameter";
        }
        else {
            message += e.getMessage();
        }
        if (e instanceof SQLException) {
            SQLException sqlException = (SQLException) e;
            if (sqlException.getNextException() != null) {
                message += "  " + sqlException.getNextException().getMessage();
            }
        }
        if (e.getCause() instanceof SQLException) {
            SQLException sqlException = (SQLException) e.getCause();
            if (sqlException.getNextException() != null) {
                message += "  " + sqlException.getNextException().getMessage();
            }
        }
        final String exceptionCode = status.getStatusCode() + ": " + status.getReasonPhrase();
        log.error(exceptionCode + " " + message);
        if (throwWpsError) {
            throw new ProcessletException(new OWSException(message, exceptionCode));
        }
        ResourceErrorHandler.handleError(message, status, log);
    }
}
