#!/bin/bash
# script to make the REST services Asciidoc using the intermediate WADL output 

#test for HOOT_HOME defined
if [ -z $HOOT_HOME ]; then cd ..; source SetupEnv.sh; cd scripts; fi

cd $HOOT_HOME/docs
mkdir -p wpsDocs 

# get list of the WPS xml files to convert
pushd . > /dev/null
cd ../hoot-services/src/main/webapp/WEB-INF/workspace/processes
FILES=`ls *.xml`
popd > /dev/null

cd ./wpsDocs
for f in $FILES
do
  # generate the asciidoc using the XSLT Style Sheet
  FILE=${f:0:(-4)}
  xsltproc ../WpsToAsciiDoc.xslt ../../hoot-services/src/main/webapp/WEB-INF/workspace/processes/$f > $FILE.asciidoc

  # Create the HTML and PDF reference manuals
  #asciidoc -a data-uri -a icons $FILE.asciidoc
  #mv ${FILE}.asciidoc ${FILE}tmp.asciidoc
  #a2x --dblatex-opts "-P latex.output.revhistory=0 -P 'doc.layout=mainmatter' -P doc.publisher.show=0" -f pdf $1 ${FILE}tmp.asciidoc
  #mv ${FILE}tmp.asciidoc ${FILE}.asciidoc
  #mv ${FILE}tmp.pdf ${FILE}.pdf
done
