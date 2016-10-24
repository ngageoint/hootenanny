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


// TODO: Review this initializer when Hootnanny's is upgraded to Tomcat 8.5 which supports Servlet Spec 3.0
// WebApplicationInitializer to be implemented in Servlet 3.0+
//@Order(1)
public class SpringWebContainerInitializer /*implements WebApplicationInitializer*/ {
/*
    @Override
    public void onStartup(ServletContext servletContext) throws ServletException {
        registerContextLoaderListener(servletContext);

        // Set the Jersey used property to it won't load a ContextLoaderListener
        servletContext.setInitParameter("contextConfigLocation", "");
    }

    private void registerContextLoaderListener(ServletContext servletContext) {
        WebApplicationContext webContext;
        webContext = createWebAplicationContext(HootServicesSpringConfig.class);
        servletContext.addListener(new ContextLoaderListener(webContext));
    }

    public WebApplicationContext createWebAplicationContext(Class... configClasses) {
        AnnotationConfigWebApplicationContext context;
        context = new AnnotationConfigWebApplicationContext();
        //context.getEnvironment().setActiveProfiles("production");
        context.register(configClasses);
        return context;
    }
    */
}
