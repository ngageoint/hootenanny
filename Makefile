-include $(HOOT_HOME)/HOOT_VERSION_FILE  


archive:
	HOOT_TARNAME=hootenanny-translations-`$$HOOT_HOME/scripts/GitVersion.sh` ; \
	rm -f $$HOOT_TARNAME.tar.gz ; \
	tar czf $$HOOT_TARNAME.tar.gz plugins-local translations-local conf
