
# A quick search didn't show the right config macro, I'm sure it is out there...
TAR=tar
EXCLUDE_DIRS=grep -E -v ".*/$$"
confdir=$(prefix)/conf
plugindir=$(prefix)/plugins

always:

# Don't try to build these, the old makefile will do it.
bin/hoot:
bin/hoot.bin:
pretty-pipes/bin/PrettyPipes:

osmapidb:
	$(MAKE) -f Makefile.old osmapidb

archive:
	$(MAKE) -f Makefile.old dist

clean-db:
	$(MAKE) -f Makefile.old clean-db

clean-all:
	$(MAKE) -f Makefile.old clean-all

coverage:
	$(MAKE) -f Makefile.old coverage

eclipse:
	$(MAKE) -f Makefile.old eclipse

licenses:
	$(MAKE) -f Makefile.old licenses

docs: always
	$(MAKE) -f Makefile.old docs

requirements:
	cd docs; $(MAKE) requirements

tmp/schema.png:
	$(MAKE) -f Makefile.old tmp/schema.png

help:
	@echo "help       - This help message."
	@echo "build      - Build all source and populated/upgrade the DB if necessary."
	@echo "clean      - Clean out all binaries. This will not modify the DB."
	@echo "clean-all  - Clean out all binaries and remove all tables from the DB defined in liquibase.properties"
	@echo "clean-db   - Remove all tables from the DB defined in liquibase.properties"
	@echo "check      - Run all the tests that should be run before commit."
	@echo "test-all   - Run all tests. Could take hours."
	@echo "dist       - Make a distributable tarball."
	@echo "coverage   - If configure was run with '--with-coverage' then it creates coverage reports."
	@echo "eclipse    - Builds eclipse project files."

test:
	scripts/HootEnv.sh $(MAKE) -f Makefile.old test

test-all:
	scripts/HootEnv.sh $(MAKE) -f Makefile.old test-all

test-all-no-core:
	scripts/HootEnv.sh $(MAKE) -f Makefile.old test-all-no-core

test-quick:
	scripts/HootEnv.sh $(MAKE) -f Makefile.old test-quick

conf/DatabaseConfig.sh:
	$(MAKE) -f Makefile.old conf/DatabaseConfig.sh

# Use some tar fanciness to grab all the files in conf, plugins, translations and scripts
install-other:
	mkdir -p tmp
	$(TAR) cf tmp/other.tar conf plugins scripts translations
	mkdir -p $(localstatedir)
	cat tmp/other.tar | $(TAR) x -C $(localstatedir)
	rm -f tmp/other.tar
	mkdir -p $(sysconfdir)
	echo "export HOOT_HOME=$(localstatedir)" > $(sysconfdir)/profile.d/hoot.sh

install-data-local: install-other

install-exec-local:
	mkdir -p $(bindir) $(libdir) $(confdir)
	$(INSTALL) bin/* $(bindir)
	$(INSTALL) lib/* $(libdir)
	test $(HAS_HADOOP) && $(INSTALL) pretty-pipes/bin/* $(bindir) || true
	test $(HAS_HADOOP) && $(INSTALL) pretty-pipes/lib/* $(libdir) || true

# Clean out all install files and then remove the empty directories.
uninstall-local:

SUBDIRS=stub
