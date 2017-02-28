To hook up Jacoco with Tomcat, add the line to /etc/init.d/tomcat8 script.

JAVA_OPTS="$JAVA_OPTS -javaagent:/home/vagrant/hoot/hoot-services/jacoco/lib/jacocoagent.jar=destfile=/home/vagrant/hoot/jacoco.exec,append=true,includes=hoot.services.*"
