package hoot.services;

import java.util.logging.Logger;

import org.glassfish.jersey.filter.LoggingFilter;
import org.glassfish.jersey.media.multipart.MultiPartFeature;
import org.glassfish.jersey.server.ResourceConfig;
import org.glassfish.jersey.server.spring.scope.RequestContextFilter;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import hoot.services.controllers.filters.CorsResponseFilter;


public class HootServicesJerseyApplication extends ResourceConfig {
    private static final Logger logger = Logger.getLogger(HootServicesJerseyApplication.class.getName());

    private static Class<?> springConfigurationClass;


    public static void setSpringConfigationClass(Class<?> springConfigurationClass) {
        HootServicesJerseyApplication.springConfigurationClass = springConfigurationClass;
    }

    public HootServicesJerseyApplication() {
        super.packages(true, "hoot.services", "org.glassfish.jersey.examples.multipart");

        super.register(MultiPartFeature.class);
        super.register(CorsResponseFilter.class);
        super.register(RequestContextFilter.class);

        /*
        super.registerInstances(new LoggingFeature(logger,
                                                     Level.ALL,
                                                     LoggingFeature.Verbosity.PAYLOAD_TEXT,
                                                     LoggingFeature.DEFAULT_MAX_ENTITY_SIZE));
        */

        super.registerInstances(new LoggingFilter(Logger.getLogger(HootServicesJerseyApplication.class.getName()), true));

        if (springConfigurationClass != null) {
            AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(springConfigurationClass);
            super.property("contextConfig", ctx);
        }
    }
}