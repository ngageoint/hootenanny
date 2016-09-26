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
