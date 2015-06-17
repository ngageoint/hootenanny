#!/bin/bash
# script to make the REST services Asciidoc using the intermediate WADL output 

cd $HOOT_HOME/docs
mkdir -p restDocs 

for serviceGroup in "info" "ingest" "job" "ogr" "osm"; do 

  [ "$1" == "-v" ] && echo "Generating report for REST service group: "$serviceGroup

  # generate the asciidoc using the XSLT Style Sheet
  xsltproc WadlToAsciiDoc.xslt ../hoot-services/target/classes/hootenanny_rest_doc_${serviceGroup}.wadl > ./restDocs/HootenannyRestReference_${serviceGroup}.asciidoc

  cd ./restDocs

  # Create the HTML and PDF reference manuals
  #asciidoc -a data-uri -a icons HootenannyRestReference_${serviceGroup}.asciidoc
  #a2x --dblatex-opts "-P latex.output.revhistory=0 -P 'doc.layout=mainmatter' -P doc.publisher.show=0" --xsltproc-opts "stringparam chapter.autolabel 0" -f pdf $1 HootenannyRestReference_${serviceGroup}.asciidoc

  cd ..
done
