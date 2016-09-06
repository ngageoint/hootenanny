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

    private static Class<?> springConfigrationClass;


    public static void setSpringConfigationClass(Class<?> springConfigationClass) {
        HootServicesJerseyApplication.springConfigrationClass = springConfigationClass;
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

        AnnotationConfigApplicationContext ctx = null;

        if (springConfigrationClass == null) {
            ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        }
        else {
            ctx = new AnnotationConfigApplicationContext(springConfigrationClass);
        }

        super.property("contextConfig", ctx);
    }
}