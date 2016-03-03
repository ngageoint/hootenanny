
SHELL=/bin/bash
FOUO=/fouo
SO_DATA=$(FOUO)/gis-data/africom/somalia
UTP_DATA=$(SO_DATA)/utp/UTP_Gaalkacyo_Shapefiles_LIMDIS
UTP_LAYERS=Roads BuildingsOfInterest
UTP_SHP=$(foreach type,$(UTP_LAYERS),tmp/utp/$(type).shp)

MAG_MAAX_DATA=$(SO_DATA)/maax/SO_POI_Database_Mag.gdb

##########
test: create-inputs modules
	# only report errors
	mocha -R $$HOOT_HOME/test-files/services-system/QuietReporter.js


# Get ready for a stress test
stress-setup: modules create-inputs
	node node_modules/mocha/bin/mocha -g conflate


# Run 1 stress test
stress:
	rm -rf stress_1
	./Stress.sh 1 stress_1


# Run Lots of stress tests
#big_stress:



clean:
	rm -rf tmp

##########
# Install the node modules in the local directory
modules:
	npm install --silent xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler &> /dev/null


# Build all of the input files
#create-inputs: $(UTP_SHP) tmp/Utp.zip tmp/Maax.zip tmp/osm_cut.osm /DcGisRoads.osm
create-inputs: tmp/Utp.zip tmp/maax_gdb.zip tmp/maax.zip tmp/osm_cut.osm tmp/DcGisRoads.osm


$(UTP_SHP):
	mkdir -p tmp/utp/
	cp $(UTP_DATA)/$(subst .shp,.*,$(notdir $@)) tmp/utp


tmp/Utp.zip: $(UTP_SHP)
	mkdir -p tmp
	zip -q -j $@ tmp/utp/*


tmp/DcGisRoads.osm:
	mkdir -p tmp
	cp $$HOOT_HOME/test-files/DcGisRoads.osm tmp/

tmp/maax_gdb.zip: $(MAG_MAAX_DATA)
	mkdir -p tmp
	CURR=`pwd` ; \
	cd $(SO_DATA)/maax/ ; \
	zip -q -r $$CURR/$@ SO_POI_Database_Mag.gdb


# This is Ugly and relies on having /fouo/hoot-tests
tmp/maax.zip:
	mkdir -p tmp
	CURR=`pwd` ; \
	cd $(FOUO)/hoot-tests/gaalkacyo.child ; \
	$(MAKE) -s tmp/maax.zip ; \
	cp tmp/maax.zip $$CURR/$@


tmp/osm_cut.osm:
	mkdir -p tmp
	CURR=`pwd` ; \
	cd $(FOUO)/hoot-tests/gaalkacyo.child ; \
	$(MAKE) -s osm/osm_cut.osm ; \
	cp osm/osm_cut.osm $$CURR/$@


restart-tomcat:
	cd /fouo/$$USER/tomcat6; bin/catalina.sh stop || echo whatever
	rm -rf /fouo/$$USER/tomcat6/webapps/hoot-services/
	cp $$HOOT_HOME/hoot-services/target/hoot-services-*.war /fouo/$$USER/tomcat6/webapps/hoot-services.war
	sleep 10
	# Split tomcat6 so it doesn't match itself
	pkill -f "tom""cat6" || echo whatever
	cd /fouo/$$USER/tomcat6; bin/catalina.sh start
	sleep 10
	cp log4j.xml /fouo/$$USER/tomcat6/webapps/hoot-services/WEB-INF/classes/
	cd /fouo/$$USER/tomcat6; bin/catalina.sh stop -force 20
	cd /fouo/$$USER/tomcat6; bin/catalina.sh start
