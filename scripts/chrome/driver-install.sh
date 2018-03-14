#!/usr/bin/env bash
set -e

# Embed version into ChromeDriver ZIP filename.
CHROMEDRIVER_ZIP=chromedriver_linux64_v$CHROMEDRIVER_VERSION.zip
CHROMEDRIVER_URL=https://chromedriver.storage.googleapis.com/$CHROMEDRIVER_VERSION/chromedriver_linux64.zip

if [ ! -f $HOME/$CHROMEDRIVER_ZIP ]; then
    echo "### Downloading ChromeDriver v${CHROMEDRIVER_VERSION}..."
    curl -sSL -o $HOME/$CHROMEDRIVER_ZIP $CHROMEDRIVER_URL
fi

if [ ! -f $HOME/bin/chromedriver ] || ! $HOME/bin/chromedriver --version | grep -q "^ChromeDriver ${CHROMEDRIVER_VERSION//\./\\.}"; then
    echo "### Installing ChromeDriver v${CHROMEDRIVER_VERSION}..."
    mkdir -p $HOME/bin
    unzip -o -d $HOME/bin $HOME/$CHROMEDRIVER_ZIP
fi
