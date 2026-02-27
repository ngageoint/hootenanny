## Docker development environment cheat sheet

### System Requirements

**Operating System:** The Docker images are now based on Rocky Linux 9 (EL9) which provides:
* Modern compiler toolchain (GCC 11+)
* Java 11
* Tomcat 9
* Node.js 18
* PostgreSQL 15
* Python 3 (Python 2 removed)

**Note:** Legacy CentOS 7 / EL7 Docker images are no longer maintained. All Docker configurations have been migrated to Rocky Linux 9.

### Basic Commands

To start the containers
>make -f Makefile.docker up


To stop the containers
>make -f Makefile.docker down


To tail tomcat logs
>docker-compose logs --follow core-services


To bash to the core-services container
>docker-compose exec core-services bash


To force clean/build on container start, modify hoot.env _e.g._
```
HOOT_CLEAN=0
HOOT_BUILD_CORE=1
HOOT_BUILD_HOOT_SERVICES=1
HOOT_BUILD_JS_SCHEMA=0
HOOT_BUILD_TRANSLATION_SERVER=0
HOOT_BUILD_NODE_EXPORT_SERVER=0
HOOT_SERVICES_HOST=core-services
HOOT_SERVICES_PORT=8080
HOOT_NPM_INSTALL=0
```
to set any properties from 0 to 1 and then stop/start the containers.

Once the images are built and the containers are up and running, open the UI at https://localhost:8080
