#!/bin/bash
# This file is part of Hootenanny 
# This file is responsible for starting the flask application for communication across containers

echo "Launching Flask application for container communication..."
flask run --host=0.0.0.0
