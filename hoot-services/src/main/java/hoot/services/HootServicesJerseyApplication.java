package hoot.services;

import java.util.logging.Logger;

import org.glassfish.jersey.filter.LoggingFilter;
import org.glassfish.jersey.server.ResourceConfig;

import hoot.services.controllers.filters.CorsResponseFilter;


public class HootServicesJerseyApplication extends ResourceConfig {
    private static final Logger logger = Logger.getLogger(HootServicesJerseyApplication.class.getName());

    public HootServicesJerseyApplication() {
        super.packages(true, "hoot.services");

        super.register(CorsResponseFilter.class);
/*
        super.registerInstances(new LoggingFeature(logger,
                                                     Level.ALL,
                                                     LoggingFeature.Verbosity.PAYLOAD_TEXT,
                                                     LoggingFeature.DEFAULT_MAX_ENTITY_SIZE));
*/
        super.registerInstances(new LoggingFilter(Logger.getLogger(HootServicesJerseyApplication.class.getName()), true));
    }
}