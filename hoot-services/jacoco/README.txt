To hook up Jacoco with Tomcat, add the line to /etc/init.d/tomcat6 script.

JAVA_OPTS="$JAVA_OPTS -javaagent:/home/vagrant/hoot/hoot-serices/jacoco/lib/jacocoagent.jar=destfile=/home/vagrant/hoot/jacoco.exec,append=true,includes=hoot.services.*"
