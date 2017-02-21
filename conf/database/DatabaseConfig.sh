# Source all of the defaults from the default config file
source $HOOT_HOME/conf/database/DatabaseConfigDefault.sh

# Check for any local user database config changes
if [ -e $HOOT_HOME/conf/database/DatabaseConfigLocal.sh ]; then
	source $HOOT_HOME/conf/database/DatabaseConfigLocal.sh
fi
