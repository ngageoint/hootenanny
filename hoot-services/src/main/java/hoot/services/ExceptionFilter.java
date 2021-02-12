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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;
import javax.ws.rs.core.Response.Status;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.security.web.firewall.RequestRejectedException;


public class ExceptionFilter implements Filter {
	private static final Logger logger = LoggerFactory.getLogger(ExceptionFilter.class);
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        try {
            chain.doFilter(request, response);
        }
        // Convert exceptions from URLs w/ double slashes from 500 to 404
        // e.g. hoot-services/osm/api//0.6/map/layers
        catch (RequestRejectedException e) {
            HttpServletResponse filteredResponse = (HttpServletResponse) response;
            response.resetBuffer();
            filteredResponse.setStatus(Status.NOT_FOUND.getStatusCode());
            filteredResponse.setContentType("text/plain");
            filteredResponse.getWriter().write("invalid uri");
        }
        catch(Exception e) {
            logger.warn("doFilter(): uncaught exception was of type: " + e.getClass().getCanonicalName());
            throw e;
        }
    }

    @Override
    public void destroy() {
    }

}
