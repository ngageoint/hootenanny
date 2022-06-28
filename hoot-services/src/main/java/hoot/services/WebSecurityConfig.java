package hoot.services;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Profile;
import org.springframework.context.annotation.PropertySource;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.web.SecurityFilterChain;


@EnableWebSecurity
@ComponentScan(basePackages = {"hoot.services"})
@PropertySource("classpath:oauth2-client.properties")
@Profile("production")
public class WebSecurityConfig {

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            .csrf().disable()
            .authorizeRequests()
            .antMatchers("/auth/**").permitAll()
            .anyRequest().authenticated();

        return http.build();
    }

// Not sure we need any of this but it was reflected in security-applicationContext.xml
//    @Bean
//    public AuthenticationEntryPoint authenticationEntryPoint(){
//        AuthenticationEntryPoint entryPoint =
//          new AuthenticationEntryPointImpl();
//        return entryPoint;
//    }
//
//    @Bean
//    public AccessDeniedHandler accessDeniedHandler(){
//        AccessDeniedHandler handler =
//          new HootAccessDeniedHandler();
//        return handler;
//    }

//    @Bean
//    public RedirectStrategy redirectStrategy(){
//        RedirectStrategy strategy =
//          new HootInvalidSessionStrategy();
//        return strategy;
//    }
}
