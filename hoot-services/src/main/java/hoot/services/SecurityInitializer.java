package hoot.services;

import org.springframework.core.annotation.Order;
import org.springframework.security.web.context.AbstractSecurityWebApplicationInitializer;
import org.springframework.stereotype.Component;

@Component
@Order(2)
public class SecurityInitializer extends AbstractSecurityWebApplicationInitializer {

}