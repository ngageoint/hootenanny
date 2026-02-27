#!/bin/bash
set -e

# EL9 Build Test Script
# This script tests the Hootenanny build on Rocky Linux 9

echo "=========================================="
echo "Hootenanny EL9 Build Test"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print status
print_status() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓${NC} $2"
    else
        echo -e "${RED}✗${NC} $2"
    fi
}

# Function to print info
print_info() {
    echo -e "${YELLOW}ℹ${NC} $1"
}

echo "Step 1: Checking OS Version"
echo "----------------------------"
if grep -q "Rocky Linux" /etc/os-release && grep -q "VERSION_ID=\"9" /etc/os-release; then
    print_status 0 "Running Rocky Linux 9"
    grep "PRETTY_NAME" /etc/os-release
else
    print_status 1 "Not running Rocky Linux 9"
    cat /etc/os-release
    exit 1
fi
echo ""

echo "Step 2: Checking Installed Tools"
echo "---------------------------------"

# Check GCC
if gcc --version | grep -q "11\."; then
    GCC_VER=$(gcc --version | head -1)
    print_status 0 "GCC 11 installed: $GCC_VER"
else
    print_status 1 "GCC 11 not found"
    gcc --version | head -1 || echo "GCC not installed"
fi

# Check Java
if java -version 2>&1 | grep -q "11\."; then
    JAVA_VER=$(java -version 2>&1 | head -1)
    print_status 0 "Java 11 installed: $JAVA_VER"
elif java -version 2>&1 | grep -q "1\.8"; then
    JAVA_VER=$(java -version 2>&1 | head -1)
    print_status 1 "Java 8 found, but Java 11 required: $JAVA_VER"
    print_info "Install Java 11 with: sudo dnf install java-11-openjdk java-11-openjdk-devel"
else
    print_status 1 "Java not found"
    print_info "Install Java 11 with: sudo dnf install java-11-openjdk java-11-openjdk-devel"
fi

# Check Node.js
if command -v node &> /dev/null; then
    NODE_VER=$(node --version)
    if [[ "$NODE_VER" == v18.* ]]; then
        print_status 0 "Node.js 18 installed: $NODE_VER"
    else
        print_status 1 "Node.js found but not version 18: $NODE_VER"
    fi
else
    print_status 1 "Node.js not found"
fi

# Check PostgreSQL
if command -v psql &> /dev/null; then
    PSQL_VER=$(psql --version)
    if echo "$PSQL_VER" | grep -q "15\."; then
        print_status 0 "PostgreSQL 15 installed: $PSQL_VER"
    else
        print_status 1 "PostgreSQL found but not version 15: $PSQL_VER"
    fi
else
    print_status 1 "PostgreSQL not found"
fi

# Check Python
if python3 --version | grep -q "3\.9"; then
    PY_VER=$(python3 --version)
    print_status 0 "Python 3.9 installed: $PY_VER"
else
    PY_VER=$(python3 --version 2>&1 || echo "Not installed")
    print_status 1 "Python 3.9 not found: $PY_VER"
fi

echo ""

echo "Step 3: Checking Geospatial Libraries"
echo "--------------------------------------"

# Check GDAL
if command -v gdal-config &> /dev/null; then
    GDAL_VER=$(gdal-config --version)
    if [[ "$GDAL_VER" == 3.8.3* ]]; then
        print_status 0 "GDAL 3.8.3 installed: $GDAL_VER"
    else
        print_status 1 "GDAL found but not version 3.8.3: $GDAL_VER"
    fi
else
    print_status 1 "GDAL not found"
fi

# Check GEOS
if command -v geos-config &> /dev/null; then
    GEOS_VER=$(geos-config --version)
    if [[ "$GEOS_VER" == 3.12.1* ]]; then
        print_status 0 "GEOS 3.12.1 installed: $GEOS_VER"
    else
        print_status 1 "GEOS found but not version 3.12.1: $GEOS_VER"
    fi
else
    print_status 1 "GEOS not found"
fi

# Check PROJ
if command -v proj &> /dev/null; then
    PROJ_VER=$(proj 2>&1 | grep "Rel\." | awk '{print $2}' | tr -d ',')
    if [[ "$PROJ_VER" == 9.3.1* ]]; then
        print_status 0 "PROJ 9.3.1 installed: $PROJ_VER"
    else
        print_status 1 "PROJ found but not version 9.3.1: $PROJ_VER"
    fi
else
    print_status 1 "PROJ not found"
fi

echo ""

echo "Step 4: Recommendations"
echo "-----------------------"
print_info "To install missing dependencies, run:"
echo "  sudo ./VagrantProvisionEL9Deps.sh"
echo ""
print_info "To build Hootenanny:"
echo "  source ./SetupEnv.sh"
echo "  aclocal && autoconf && autoheader && automake"
echo "  ./configure --with-services"
echo "  make -sj\$(nproc)"
echo ""
print_info "For detailed testing instructions, see:"
echo "  docs/EL9-Quick-Test-Guide.md"
echo ""

echo "=========================================="
echo "Test Complete"
echo "=========================================="

