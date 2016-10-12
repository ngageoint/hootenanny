package hoot.services.testsupport;

import javax.ws.rs.core.Application;

import org.glassfish.jersey.test.JerseyTest;
import org.glassfish.jersey.test.TestProperties;
import org.slf4j.bridge.SLF4JBridgeHandler;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.core.env.AbstractEnvironment;


public class HootServicesJerseyTestAbstract extends JerseyTest {
    protected static ApplicationContext applicationContext;

    static {
        // Jersey Framework uses JUL logging.  To force it to go through SLF4J we need to take a couple of steps.

        // 1) Optionally remove existing handlers attached to j.u.l root logger
        SLF4JBridgeHandler.removeHandlersForRootLogger(); // (since SLF4J 1.6.5)

        // 2) add SLF4JBridgeHandler to j.u.l's root logger, should be done once
        // during the initialization phase of your application
        SLF4JBridgeHandler.install();

        System.setProperty(AbstractEnvironment.ACTIVE_PROFILES_PROPERTY_NAME, "test");
        applicationContext = new AnnotationConfigApplicationContext(HootServicesSpringTestConfig.class);
    }

    @Override
    protected Application configure() {
        super.set(TestProperties.RECORD_LOG_LEVEL, Integer.MIN_VALUE);
        super.enable(TestProperties.LOG_TRAFFIC);
        super.enable(TestProperties.DUMP_ENTITY);
        return new HootServicesJerseyTestApplication(applicationContext);
    }
}
