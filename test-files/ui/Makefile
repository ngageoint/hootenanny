
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
HOOT_OPTS+= -D convert.ops=hoot::DecomposeBuildingRelationsVisitor
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
	xvfb-run --server-args="-screen 0, 1024x768x24" cucumber $(CUKE_OPTS) $(CUKE_FEATURES); \
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
	    'H0xC4KbUNRIUBJ8HrJIpdGz4WfouNiS0Lh1Yd50z', \
	    'miYcdFFakicoYH7Xkw6Sop0mSM1fCzlRWK8eaHoL', \
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

	# add rememberme attribute data for the spring session
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -c \
		"INSERT INTO spring_session_attributes( \
		session_id, \
		attribute_name, \
		attribute_bytes) \
		\
		VALUES ( \
		'ff47f751-c831-41ee-800f-5ef8b9371ee3', \
		'org.springframework.security.oauth.consumer.rememberme.HttpSessionOAuthRememberMeServices#REMEMBERED_TOKENS', \
		'\xaced0005737200116a6176612e7574696c2e486173684d61700507dac1c31660d103000246000a6c6f6164466163746f724900097468726573686f6c6478703f400000000000007708000000100000000078')"

	# add openStreetMap attribute data for the spring session
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -c \
		"INSERT INTO spring_session_attributes( \
		session_id, \
		attribute_name, \
		attribute_bytes) \
		\
		VALUES ( \
		'ff47f751-c831-41ee-800f-5ef8b9371ee3', \
		'OAUTH_TOKEN#openStreetMap', \
		'\xaced00057372003e6f72672e737072696e676672616d65776f726b2e73656375726974792e6f617574682e636f6e73756d65722e4f41757468436f6e73756d6572546f6b656ec7af2259bac92fe90200055a000b616363657373546f6b656e4c00146164646974696f6e616c506172616d657465727374000f4c6a6176612f7574696c2f4d61703b4c000a7265736f7572636549647400124c6a6176612f6c616e672f537472696e673b4c000673656372657471007e00024c000576616c756571007e00027870017074000d6f70656e5374726565744d61707400286d695963644646616b69636f594837586b7736536f70306d534d3166437a6c52574b386561486f4c74002848307843344b62554e524955424a3848724a497064477a3457666f754e6953304c6831596435307a')"

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
