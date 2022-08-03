
SHELL=/bin/bash

HOOT_OPTS=--status
# If the silent flag is passed to make then make hoot quiet too.
ifneq (,$(findstring s,$(MAKEFLAGS)))
  HOOT_OPTS=--warn
  TIME=
  ECHO=true
  PSQL_QUIET=1>&2
endif

# Check if we are using a different Tomcat8 port
ifeq ($(TOMCAT_PORT),)
  TOMCAT_PORT=8080
endif
# setup proxy port
ifeq ($(TOMCAT_PROXY_PORT),)
  TOMCAT_PROXY_PORT=9090
endif

CWD=$(shell pwd)

$(shell $(HOOT_HOME)/scripts/ReplaceEnvironmentVariables.sh DbSettings.inc.in DbSettings.inc)
-include DbSettings.inc

HOOT_OPTS+= -C Testing.conf
HOOT_OPTS+= -D convert.ops=DecomposeBuildingRelationsVisitor
HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true
HOOT_OPTS+= -D api.db.email=test@test.com
HOOT_OPTS+= -D reader.add.source.datetime=false

ifneq (,$(findstring s,$(MAKEFLAGS)))
  override CUKE_OPTS+= --format progress
else
  override CUKE_OPTS+= --format pretty
endif


test: load-data
	# killing faux-authentication proxy server...
	ps ux | grep nod[e] | grep proxy | awk "{print $$2}" | xargs kill >/dev/null 2>&1 || true
	# Installing node modules for proxy server...
	npm install --prefix $(HOOT_HOME)/test-files/ui/ $(HOOT_HOME)/test-files/ui/
	# Starting proxy server for faux-authentication...
	node $(HOOT_HOME)/test-files/ui/proxy.js --tomcat_port $(TOMCAT_PORT) 2>&1 >/dev/null &
	sleep 1
# The path used in VerifyTomcatStarted.sh requires auth; if this is giving you problems
# be sure that the load-data target is run prior to load the user session data.
ifneq (,$(findstring s,$(MAKEFLAGS)))
	$(HOOT_HOME)/scripts/tomcat/VerifyTomcatStarted.sh $(TOMCAT_PROXY_PORT) --quiet 2> >( $(FILTER) >&2 )
else
	$(HOOT_HOME)/scripts/tomcat/VerifyTomcatStarted.sh $(TOMCAT_PROXY_PORT)
endif
	# Running user interface tests...
	TOMCAT_PORT=$(TOMCAT_PROXY_PORT) \
	xvfb-run --server-args="-screen 0 1280x1024x24" bundle exec cucumber $(CUKE_OPTS) $(CUKE_FEATURES); \
	ret=$$?; \
	(ps ux | grep nod[e] | grep proxy | awk "{print $$2}" | xargs kill >/dev/null 2>&1 || true); \
	exit $$ret

	echo ""

	#Remove test datasets from db
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -t -A -c "SELECT display_name FROM maps WHERE display_name like '%Cucumber%'" \
	| while read mapname; do \
 	   echo hoot db-delete $(HOOT_OPTS) $(DB_URL)/$$mapname | bash ; \
	done; \
	true

clean:
	#Remove test datasets from db
	@if [[ `PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -t -A -c "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name='maps');"` = 't' ]]; then \
		PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -t -A -c "SELECT display_name FROM maps WHERE display_name like '%Cucumber%'" \
		| while read mapname; do \
			echo hoot db-delete $(HOOT_OPTS) $(DB_URL)/$$mapname | bash ; \
		done; \
		true; \
	fi

	# Remove test user and faux-session data from db
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -c "DELETE FROM users where email='test@test.com' or id=-1541432234" 2>/dev/null $(PSQL_QUIET) || true

	# Cleaning up proxy node modules...
	rm -rf $(HOOT_HOME)/test-files/ui/node_modules >/dev/null 2>&1 || true

coverage: clean-coverage
	echo Generating user interface coverage report...
ifneq (,$(findstring s,$(MAKEFLAGS)))
	$(HOOT_HOME)/scripts/cover/CoverHootUI.sh &> coverage.log
else
	$(HOOT_HOME)/scripts/cover/CoverHootUI.sh
endif
	echo ""

clean-coverage:
	rm -f coverage.log
	rm -f screenshot_*.png
	rm -f screenshot_*.html
	rm -f $(HOOT_HOME)/hoot-ui/test/istanbul_index.html
	rm -rf $(HOOT_HOME)/hoot-ui/istanbul
	rm -rf $(HOOT_HOME)/hoot-ui/mocha-coverage
	rm -rf $(HOOT_HOME)/hoot-ui/cucumber-coverage
	rm -rf $(HOOT_HOME)/hoot-ui/combined-coverage
load-data: clean
	# Add a test user for tests
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -c \
	    "INSERT INTO users( \
	    email,id,display_name, \
	    \
	    provider_access_key, \
	    provider_access_token, \
	    \
	    hootservices_last_authorize, \
	    hootservices_created_at, \
	    provider_created_at, \
	    privileges) \
	    \
	    VALUES ( \
	    'test@test.com', \
	    -1541432234, \
	    'Cucumber', \
	    \
	    'GXmPslrsulOO_43UiAdf6MQXEUI7TrqL9SI0pJd75UA', \
	    'Bearer', \
	    \
	    now(), \
	    now(), \
	    now(), \
	    '\"advanced\"=>\"true\"')"

	# Add a spring session for our test user
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -c \
		"INSERT INTO spring_session( \
		session_id, \
		creation_time, \
		last_access_time, \
		max_inactive_interval, \
		principal_name, \
		user_id) \
		\
		VALUES ( \
		'ff47f751-c831-41ee-800f-5ef8b9371ee3', \
		cast(extract(epoch from (now())) as bigint)*1000, \
		cast(extract(epoch from (now() + INTERVAL '1 second')) as bigint)*1000, \
		31536000, \
		'', \
		-1541432234)"

	# add security context attribute data for the spring session
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -c \
		"INSERT INTO spring_session_attributes( \
		session_id, \
		attribute_name, \
		attribute_bytes) \
		\
		VALUES ( \
		'ff47f751-c831-41ee-800f-5ef8b9371ee3', \
		'SPRING_SECURITY_CONTEXT', \
		'\xaced00057372003d6f72672e737072696e676672616d65776f726b2e73656375726974792e636f72652e636f6e746578742e5365637572697479436f6e74657874496d706c000000000000023a0200014c000e61757468656e7469636174696f6e7400324c6f72672f737072696e676672616d65776f726b2f73656375726974792f636f72652f41757468656e7469636174696f6e3b7870737200536f72672e737072696e676672616d65776f726b2e73656375726974792e6f61757468322e636c69656e742e61757468656e7469636174696f6e2e4f417574683241757468656e7469636174696f6e546f6b656e000000000000023a0200024c001e617574686f72697a6564436c69656e74526567697374726174696f6e49647400124c6a6176612f6c616e672f537472696e673b4c00097072696e636970616c74003a4c6f72672f737072696e676672616d65776f726b2f73656375726974792f6f61757468322f636f72652f757365722f4f4175746832557365723b787200476f72672e737072696e676672616d65776f726b2e73656375726974792e61757468656e7469636174696f6e2e416273747261637441757468656e7469636174696f6e546f6b656ed3aa287e6e47640e0200035a000d61757468656e746963617465644c000b617574686f7269746965737400164c6a6176612f7574696c2f436f6c6c656374696f6e3b4c000764657461696c737400124c6a6176612f6c616e672f4f626a6563743b7870017372001f6a6176612e7574696c2e436f6c6c656374696f6e7324456d7074794c6973747ab817b43ca79ede02000078707074002b4d2d4d705756516f50425a6d7257536a6176637a6a4473587062554e4967696437775a566d41314d5132517372003f6f72672e737072696e676672616d65776f726b2e73656375726974792e6f61757468322e636f72652e757365722e44656661756c744f417574683255736572000000000000023a0200034c000a6174747269627574657374000f4c6a6176612f7574696c2f4d61703b4c000b617574686f72697469657374000f4c6a6176612f7574696c2f5365743b4c00106e616d654174747269627574654b657971007e00047870737200256a6176612e7574696c2e436f6c6c656374696f6e7324556e6d6f6469666961626c654d6170f1a5a8fe74f507420200014c00016d71007e000e7870737200176a6176612e7574696c2e4c696e6b6564486173684d617034c04e5c106cc0fb0200015a000b6163636573734f72646572787200116a6176612e7574696c2e486173684d61700507dac1c31660d103000246000a6c6f6164466163746f724900097468726573686f6c6478703f40000000000003770800000004000000027400046e616d6574000776616772616e7474000642656172657274002b7272694b6e5546416d314758636f384477703739597562756f522d31795745437974614d32625861646f557800737200256a6176612e7574696c2e436f6c6c656374696f6e7324556e6d6f6469666961626c65536574801d92d18f9b80550200007872002c6a6176612e7574696c2e436f6c6c656374696f6e7324556e6d6f6469666961626c65436f6c6c656374696f6e19420080cb5ef71e0200014c00016371007e00077870737200176a6176612e7574696c2e4c696e6b656448617368536574d86cd75a95dd2a1e020000787200116a6176612e7574696c2e48617368536574ba44859596b8b7340300007870770c000000103f4000000000000078740006426561726572')"

	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcGisRoads.osm $(DB_URL)/DcGisRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcTigerRoads.osm $(DB_URL)/DcTigerRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcOsmRoads.osm $(DB_URL)/DcOsmRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesA.osm $(DB_URL)/AllDataTypesACucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesB.osm $(DB_URL)/AllDataTypesBCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/ToyTestA.osm $(DB_URL)/ToyTestACucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/ToyTestB.osm $(DB_URL)/ToyTestBCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/ToyTestCauseReview.osm $(DB_URL)/TTCauseReviewCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/Merged_Brazil_daba15e4.osm $(DB_URL)/BrazilMergedCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/cmd/glacial/DiffConflateCmdTest/input1.osm $(DB_URL)/DiffInput1Cucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/cmd/glacial/DiffConflateCmdTest/input2.osm $(DB_URL)/DiffInput2Cucumber
