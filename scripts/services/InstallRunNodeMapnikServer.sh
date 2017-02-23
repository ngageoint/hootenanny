# Install and run node-mapnik-server
if [ -z "$NODE_MAPNIK_SERVER_PORT" ]; then
    echo "Error: NODE_MAPNIK_SERVER_PORT not set."
    false
else
    rm -rf ~/tmp/npm*
    cd $HOOT_HOME
    source SetupEnv.sh
    source conf/database/DatabaseConfig.sh
    #source conf/ServerConfig.sh
    cd node-mapnik-server
    npm install -q
    NPID=`ps aux | grep node | grep "app.js hoot_style.xml $NODE_MAPNIK_SERVER_PORT" | awk '{print $2}'`
    # send kill
    if [ -z "$NPID" ]; then
        #Do nothing
        :
    else
        kill $NPID;
    fi
    mkdir -p log
    node app.js hoot_style.xml $NODE_MAPNIK_SERVER_PORT 1>log/app.log 2>&1 &
fi

