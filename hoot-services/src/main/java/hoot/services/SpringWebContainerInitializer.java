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
 * @copyright Copyright (C) 2016, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services;

import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.ServletRegistration;

import org.springframework.core.annotation.Order;
import org.springframework.session.jdbc.config.annotation.web.http.JdbcHttpSessionConfiguration;
import org.springframework.web.WebApplicationInitializer;
import org.springframework.web.context.ContextLoaderListener;
import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.context.support.AnnotationConfigWebApplicationContext;

// TODO: Review this initializer when Hootnanny's is upgraded to Tomcat 8.5 which supports Servlet Spec 3.0
// WebApplicationInitializer to be implemented in Servlet 3.0+
@Order(1)
public class SpringWebContainerInitializer implements WebApplicationInitializer {

    @Override
    public void onStartup(ServletContext servletContext) throws ServletException {
        registerContextLoaderListener(servletContext);

        // Set the Jersey used property to it won't load a ContextLoaderListener
        servletContext.setInitParameter("contextConfigLocation", "<NONE>");

        ServletRegistration.Dynamic jerseyServlet = servletContext.addServlet("jerseyServlet",
                "org.glassfish.jersey.servlet.ServletContainer");
            //note "javax.ws.rs.Application" doesn't have "core"
            jerseyServlet.setInitParameter("javax.ws.rs.Application", HootServicesJerseyApplication.class.getName());
            jerseyServlet.addMapping("/osm/*");
            jerseyServlet.addMapping("/job/*");
            jerseyServlet.addMapping("/jobs/*");
            jerseyServlet.addMapping("/auth/*");
            jerseyServlet.addMapping("/info/*");
            jerseyServlet.addMapping("/ogr/*");
            jerseyServlet.addMapping("/ingest/*");
            jerseyServlet.addMapping("/grail/*");
            jerseyServlet.addMapping("/language/*");
            jerseyServlet.setLoadOnStartup(2);

    }

    private void registerContextLoaderListener(ServletContext servletContext) {
        WebApplicationContext webContext;
        webContext = createWebApplicationContext(
                WebSecurityConfig.class,
                HootServicesSpringConfig.class,
                JdbcHttpSessionConfiguration.class
            );
        servletContext.addListener(new ContextLoaderListener(webContext));
        servletContext.addListener(new HootServletContext());
    }

    public WebApplicationContext createWebApplicationContext(Class... configClasses) {
        AnnotationConfigWebApplicationContext context;
        context = new AnnotationConfigWebApplicationContext();
        context.getEnvironment().setActiveProfiles("production");
        context.register(configClasses);
        return context;
    }

}
