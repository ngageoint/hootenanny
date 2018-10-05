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
