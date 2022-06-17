package hoot.services;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Profile;
import org.springframework.context.annotation.PropertySource;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.web.server.ServerHttpSecurity;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.server.SecurityWebFilterChain;


@Configuration
@EnableWebSecurity
@ComponentScan(basePackages = {"hoot.services"})
@PropertySource("classpath:oauth2-client.properties")
@Profile("production")
public class WebSecurityConfig {	
    @Bean
    public SecurityWebFilterChain springSecurityFilterChain(ServerHttpSecurity http) {
    	// make it so we only whitelist the non-auth handshake endpoints
    	
        http.authorizeExchange()
	        .anyExchange()
	        .permitAll();

        return http.build();
    }

}
