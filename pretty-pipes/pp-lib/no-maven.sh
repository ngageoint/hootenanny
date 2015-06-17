
mkdir -p target/classes

javac -sourcepath src/main/java/ -d target/classes/ src/main/java/pp/*

# Copy all the libs we need into a lib directory
#rm -rf target/jar/
#mkdir -p target/jar/lib/
#cp $HADOOP_HOME/hadoop-*-core.jar \
#   $HADOOP_HOME/lib/commons-logging-api-*.jar \
#   $JAVA_HOME/lib/tools.jar \
#   target/jar/lib/

# Create the jar
jar cf target/PrettyPipesLib-0.0.1.jar \
	-C target/classes pp/ \

cp target/PrettyPipesLib-0.0.1.jar ../lib/
