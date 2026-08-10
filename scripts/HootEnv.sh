#!/bin/bash

SCRIPT_FILE=`readlink -f ${BASH_SOURCE[0]}`
SCRIPT_PATH="$( cd "$( dirname "$SCRIPT_FILE" )" && pwd )"

export HOOT_HOME=`readlink -m $SCRIPT_PATH/../`
export HOOT_WORKING_NAME=`echo $HOOT_HOME | sed -e "s/.*\///g"`

# Enables precompiled headers with ccache
export CCACHE_SLOPPINESS=time_macros

# Ensure common local install paths are searched before probing for helper binaries.
export PATH=/usr/local/bin:/usr/local/sbin:$PATH

# Resolve the GDAL helper explicitly so startup doesn't depend on a fragile PATH.
GDAL_CONFIG_BIN="${GDAL_CONFIG:-}"
if [ -z "$GDAL_CONFIG_BIN" ]; then
  for candidate in \
    "$(command -v gdal-config 2>/dev/null || true)" \
    /usr/local/bin/gdal-config \
    /usr/bin/gdal-config \
    /usr/bin64/gdal-config
  do
    if [ -n "$candidate" ] && [ -x "$candidate" ]; then
      GDAL_CONFIG_BIN="$candidate"
      break
    fi
  done
fi

if [ -z "$GDAL_CONFIG_BIN" ] || [ ! -x "$GDAL_CONFIG_BIN" ]; then
  echo "Unable to locate an executable gdal-config. Checked PATH and common install locations." >&2
  return 1 2>/dev/null || exit 1
fi

# Determine the library path for GDAL
export GDAL_CONFIG="$GDAL_CONFIG_BIN"
export GDAL_LIB_DIR=`"$GDAL_CONFIG_BIN" --libs | sed -e "s/-L//g" | sed -e "s/ *-lgdal.*//g"`
export GDAL_DATA=`"$GDAL_CONFIG_BIN" --datadir`

find_jdk_home()
{
  local candidate
  for candidate in \
    "${JAVA_HOME:-}" \
    "/usr/lib/jvm/java-${JDK_VERSION:-1.8.0}" \
    /usr/lib/jvm/java-${JDK_VERSION:-1.8.0}-openjdk* \
    /usr/lib/jvm/java
  do
    if [ -n "$candidate" ] && [ -f "$candidate/include/jni.h" ]; then
      readlink -f "$candidate"
      return 0
    fi
  done

  while IFS= read -r candidate; do
    candidate="$(dirname "$(dirname "$candidate")")"
    if [ -f "$candidate/include/jni.h" ]; then
      readlink -f "$candidate"
      return 0
    fi
  done < <(find /usr/lib/jvm -path '*/include/jni.h' 2>/dev/null | sort -V)

  return 1
}

if JAVA_HOME_RESOLVED="$(find_jdk_home)"; then
  export JAVA_HOME="$JAVA_HOME_RESOLVED"
else
  echo "Unable to locate a JDK with JNI headers under /usr/lib/jvm. Install a java-*-openjdk-devel package." >&2
  return 1 2>/dev/null || exit 1
fi

JVM_LIB_DIR=""
for candidate in "$JAVA_HOME/lib/server" "$JAVA_HOME/jre/lib/amd64/server"; do
  if [ -d "$candidate" ]; then
    JVM_LIB_DIR="$candidate"
    break
  fi
done

if [ -z "$JVM_LIB_DIR" ]; then
  echo "Unable to locate libjvm under JAVA_HOME=$JAVA_HOME." >&2
  return 1 2>/dev/null || exit 1
fi

export LD_LIBRARY_PATH=$GDAL_LIB_DIR:$JVM_LIB_DIR:$HOOT_HOME/lib:${LD_LIBRARY_PATH:-}

export PATH=$HOOT_HOME/bin/:$PATH
export QT_SELECT=5

if [ -f /opt/rh/devtoolset-${DEVTOOLSET_VERSION}/enable ]; then
    source $HOOT_HOME/VagrantProvisionVars.sh
    source /opt/rh/devtoolset-$DEVTOOLSET_VERSION/enable
fi

# Keep the PGDG client libraries ahead of CentOS 7's libpq so newer PostgreSQL
# authentication methods such as SCRAM work when talking to modern servers.
if [ -n "${PG_MAJOR_VERSION:-}" ] && [ -d "/usr/pgsql-${PG_MAJOR_VERSION}" ]; then
  export PATH=/usr/pgsql-${PG_MAJOR_VERSION}/bin:$PATH
  export LD_LIBRARY_PATH=/usr/pgsql-${PG_MAJOR_VERSION}/lib:$LD_LIBRARY_PATH
fi

# If the user provided an argument then run as a command.
if [ ${#@} -gt 0 ]; then
    "$@"
fi
