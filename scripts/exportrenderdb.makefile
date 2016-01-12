#
#  Export hoot dataset to PostGIS render db
#
step1:
	export INPUT=$(INPUT) && bash $(HOOT_HOME)/scripts/exportrenderdb.sh
