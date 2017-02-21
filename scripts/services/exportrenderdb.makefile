#
#  Export hoot dataset to PostGIS render db
#
step1:
	export MAP_ID=$(MAP_ID) && bash $(HOOT_HOME)/scripts/services/exportrenderdb.sh
