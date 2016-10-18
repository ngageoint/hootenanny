package hoot.services;

import javax.sql.DataSource;

import org.apache.commons.dbcp.BasicDataSource;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.DependsOn;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;


@Configuration
@EnableTransactionManagement
@ComponentScan(basePackages = {"hoot.services"})
@PropertySource("classpath:db/db.properties")
public class HootServicesSpringConfig {

    @Autowired
    private Environment env;

    public HootServicesSpringConfig() {}

    @Bean
    public ApplicationContextUtils applicationContextUtils() {
        return new ApplicationContextUtils();
    }

    @Bean(name = "dataSource")
    public DataSource dataSource() {
        BasicDataSource dataSource = new BasicDataSource();
        dataSource.setDriverClassName("org.postgresql.Driver");
        dataSource.setUrl("jdbc:postgresql://" + env.getProperty("DB_HOST") + ":" +
                                                 env.getProperty("DB_PORT") + "/" +
                                                 env.getProperty("DB_NAME"));
        dataSource.setUsername(env.getProperty("DB_USER"));
        dataSource.setPassword(env.getProperty("DB_PASSWORD"));
        dataSource.setInitialSize(25);
        dataSource.setMaxActive(90);
        dataSource.setMaxIdle(30);
        return dataSource;
    }

    @Bean(name = "transactionManager")
    @Autowired
    @DependsOn("dataSource")
    public PlatformTransactionManager transactionManager(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }
}
