# Hootenanny EL9 Migration Test Plan

## Overview

This document outlines the comprehensive test plan for validating the Hootenanny migration from Enterprise Linux 7 (CentOS 7) to Enterprise Linux 9 (Rocky Linux 9).

**Test Plan Version:** 1.0  
**Last Updated:** 2026-02-26  
**Status:** Ready for Execution

## Test Objectives

1. Verify all dependencies install correctly on EL9
2. Confirm successful compilation of all components
3. Validate runtime functionality matches EL7 behavior
4. Ensure backward compatibility is maintained
5. Verify Docker and Vagrant environments work correctly
6. Confirm CI/CD pipelines function properly

## Test Environment

### Hardware Requirements
- **CPU:** 4+ cores recommended
- **RAM:** 8GB minimum, 16GB recommended
- **Disk:** 50GB free space minimum
- **Network:** Internet connection for package downloads

### Software Requirements
- **OS:** Rocky Linux 9.x (minimal installation)
- **Virtualization:** VirtualBox 6.1+ or VMware (for Vagrant tests)
- **Docker:** Docker Engine 20.10+ (for Docker tests)
- **Vagrant:** 2.2.19+ (for Vagrant tests)

## Test Categories

### 1. Installation Tests

#### 1.1 Vagrant RPM Installation (Quick Start)

**Test ID:** EL9-INST-001  
**Priority:** High  
**Objective:** Verify quick start RPM installation works

**Steps:**
```bash
git clone git@github.com:ngageoint/hootenanny.git hoot
cd hoot
vagrant up hoot_el9_rpm
```

**Expected Results:**
- VM provisions successfully
- All RPM packages install without errors
- Tomcat 9 starts successfully
- PostgreSQL 15 starts successfully
- Web UI accessible at https://localhost:8443/hootenanny-id/

**Pass Criteria:**
- [ ] VM boots and provisions without errors
- [ ] All services start successfully
- [ ] Web UI loads and displays correctly
- [ ] No error messages in logs

---

#### 1.2 Vagrant Source Build Installation

**Test ID:** EL9-INST-002  
**Priority:** High  
**Objective:** Verify building from source works

**Steps:**
```bash
git clone git@github.com:ngageoint/hootenanny.git hoot
cd hoot
git submodule init
git submodule update
vagrant up hoot_el9
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh
make -sj$(nproc)
```

**Expected Results:**
- All dependencies install correctly
- Configure step completes successfully
- Compilation completes without errors
- All binaries are created

**Pass Criteria:**
- [ ] Dependencies install successfully
- [ ] Configure completes without errors
- [ ] Make completes without errors
- [ ] `hoot --version` returns version information
- [ ] Build time is reasonable (< 60 minutes on 4-core system)

---

#### 1.3 Docker Installation

**Test ID:** EL9-INST-003  
**Priority:** High  
**Objective:** Verify Docker-based installation works

**Steps:**
```bash
git clone git@github.com:ngageoint/hootenanny.git hoot
cd hoot
make -f Makefile.docker up
```

**Expected Results:**
- All Docker images build successfully
- All containers start successfully
- Services are accessible

**Pass Criteria:**
- [ ] Docker images build without errors
- [ ] All containers start and remain running
- [ ] Web UI accessible at https://localhost:8080
- [ ] No container crashes or restarts

---

### 2. Dependency Verification Tests

#### 2.1 System Dependencies

**Test ID:** EL9-DEP-001  
**Priority:** High  
**Objective:** Verify all system dependencies are correct versions

**Steps:**
```bash
vagrant ssh hoot_el9
# Check versions
gcc --version
java -version
node --version
psql --version
python3 --version
```

**Expected Results:**
- GCC: 11.x or higher
- Java: OpenJDK 11.x
- Node.js: 18.x
- PostgreSQL: 15.x
- Python: 3.9.x

**Pass Criteria:**
- [ ] All versions match expected values
- [ ] No Python 2 available
- [ ] No devtoolset packages installed

---

#### 2.2 Geospatial Libraries

**Test ID:** EL9-DEP-002  
**Priority:** High  
**Objective:** Verify geospatial library versions

**Steps:**
```bash
vagrant ssh hoot_el9
gdal-config --version
geos-config --version
proj --version
```

**Expected Results:**
- GDAL: 3.8.3
- GEOS: 3.12.1
- PROJ: 9.3.1

**Pass Criteria:**
- [ ] All versions match expected values
- [ ] Libraries are functional (can be linked)

---

### 3. Build System Tests

#### 3.1 Clean Build

**Test ID:** EL9-BUILD-001  
**Priority:** High  
**Objective:** Verify clean build from scratch

**Steps:**
```bash
vagrant ssh hoot_el9
cd hoot
make clean
source ./SetupEnv.sh
aclocal && autoconf && autoheader && automake
./configure --with-services
make -sj$(nproc)
```

**Expected Results:**
- Configure completes successfully
- All components compile without errors
- No warnings about missing dependencies

**Pass Criteria:**
- [ ] Configure succeeds
- [ ] Make succeeds
- [ ] No critical warnings
- [ ] All expected binaries created

---

#### 3.2 Incremental Build

**Test ID:** EL9-BUILD-002  
**Priority:** Medium  
**Objective:** Verify incremental builds work correctly

**Steps:**
```bash
vagrant ssh hoot_el9
cd hoot
# Make a small change to a source file
echo "// test comment" >> hoot-core/src/main/cpp/hoot/core/Hoot.cpp
make -sj$(nproc)
```

**Expected Results:**
- Only changed files recompile
- Build completes quickly

**Pass Criteria:**
- [ ] Incremental build succeeds
- [ ] Build time < 5 minutes
- [ ] Only affected files recompile

---

### 4. Runtime Functionality Tests

#### 4.1 Command Line Interface

**Test ID:** EL9-RUN-001  
**Priority:** High  
**Objective:** Verify CLI commands work correctly

**Steps:**
```bash
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh
hoot --version
hoot help
hoot conflate test-files/ToyTestA.osm test-files/ToyTestB.osm /tmp/output.osm
```

**Expected Results:**
- Version displays correctly
- Help displays all commands
- Conflation completes successfully
- Output file is created

**Pass Criteria:**
- [ ] `hoot --version` works
- [ ] `hoot help` works
- [ ] Conflation completes without errors
- [ ] Output file is valid OSM format

---

#### 4.2 Web Services

**Test ID:** EL9-RUN-002  
**Priority:** High  
**Objective:** Verify web services function correctly

**Steps:**
```bash
# Access from host machine
curl -k https://localhost:8443/hoot-services/info/about/servicesVersionInfo
curl -k https://localhost:8443/hoot-services/info/about/coreVersionInfo
```

**Expected Results:**
- Services respond with version information
- No 500 errors
- JSON responses are valid

**Pass Criteria:**
- [ ] Services endpoints respond
- [ ] Version information is correct
- [ ] No errors in Tomcat logs

---

#### 4.3 Web UI

**Test ID:** EL9-RUN-003  
**Priority:** High  
**Objective:** Verify web UI loads and functions

**Steps:**
1. Open browser to https://localhost:8443/hootenanny-id/
2. Accept self-signed certificate
3. Login (if OAuth configured)
4. Navigate through UI

**Expected Results:**
- UI loads without errors
- Map displays correctly
- All menu items accessible

**Pass Criteria:**
- [ ] UI loads successfully
- [ ] No JavaScript errors in console
- [ ] Map tiles load
- [ ] UI is responsive

---

### 5. Test Suite Execution

#### 5.1 Core Tests

**Test ID:** EL9-TEST-001  
**Priority:** High  
**Objective:** Verify core test suite passes

**Steps:**
```bash
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh
make -sj$(nproc) test
```

**Expected Results:**
- All core tests pass
- No segmentation faults
- No memory leaks detected

**Pass Criteria:**
- [ ] Test suite completes
- [ ] Pass rate >= 99%
- [ ] No critical failures

---

#### 5.2 Services Tests

**Test ID:** EL9-TEST-002  
**Priority:** High  
**Objective:** Verify services test suite passes

**Steps:**
```bash
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh
make -sj$(nproc) services-test
```

**Expected Results:**
- All services tests pass
- Database operations work correctly

**Pass Criteria:**
- [ ] Test suite completes
- [ ] Pass rate >= 99%
- [ ] No database connection errors

---

#### 5.3 Full Test Suite

**Test ID:** EL9-TEST-003  
**Priority:** High  
**Objective:** Verify complete test suite passes

**Steps:**
```bash
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh
make -sj$(nproc) test-all
```

**Expected Results:**
- All tests pass
- Test execution time is reasonable

**Pass Criteria:**
- [ ] All test categories pass
- [ ] Pass rate >= 99%
- [ ] Execution time < 2 hours

---

### 6. Service Management Tests

#### 6.1 Tomcat 9 Service

**Test ID:** EL9-SVC-001  
**Priority:** High  
**Objective:** Verify Tomcat 9 service management

**Steps:**
```bash
vagrant ssh hoot_el9
sudo systemctl status tomcat9
sudo systemctl stop tomcat9
sudo systemctl start tomcat9
sudo systemctl restart tomcat9
sudo journalctl -eu tomcat9
```

**Expected Results:**
- Service starts/stops/restarts successfully
- Logs are accessible
- No errors in logs

**Pass Criteria:**
- [ ] All systemctl commands work
- [ ] Service starts cleanly
- [ ] Logs show no errors
- [ ] Web apps deploy correctly

---

#### 6.2 PostgreSQL 15 Service

**Test ID:** EL9-SVC-002  
**Priority:** High  
**Objective:** Verify PostgreSQL 15 service management

**Steps:**
```bash
vagrant ssh hoot_el9
sudo systemctl status postgresql-15
sudo -u postgres psql -c "SELECT version();"
```

**Expected Results:**
- PostgreSQL 15 is running
- Can connect and query

**Pass Criteria:**
- [ ] Service is running
- [ ] Version is 15.x
- [ ] Can execute queries

---

### 7. Backward Compatibility Tests

#### 7.1 Script Auto-Detection

**Test ID:** EL9-COMPAT-001  
**Priority:** High  
**Objective:** Verify scripts auto-detect OS version

**Steps:**
```bash
vagrant ssh hoot_el9
cd hoot
# Check that scripts detect EL9
grep -r "EL_VERSION" scripts/yum/*.sh
source scripts/HootEnv.sh
echo $JAVA_HOME
```

**Expected Results:**
- Scripts detect EL9 correctly
- Appropriate paths are set

**Pass Criteria:**
- [ ] EL version detected as 9
- [ ] Java 11 paths used
- [ ] Tomcat 9 paths used
- [ ] No devtoolset loaded

---

### 8. Docker Tests

#### 8.1 Docker Image Build

**Test ID:** EL9-DOCKER-001  
**Priority:** High  
**Objective:** Verify Docker images build correctly

**Steps:**
```bash
cd docker
docker build -t test-core -f Dockerfile.core-services .
docker build -t test-frontend -f Dockerfile.frontend .
docker build -t test-postgres -f Dockerfile.postgres .
```

**Expected Results:**
- All images build successfully
- Rocky Linux 9 base is used

**Pass Criteria:**
- [ ] All builds succeed
- [ ] Images use rockylinux:9
- [ ] No build errors

---

### 9. CI/CD Tests

#### 9.1 CircleCI Docker Images

**Test ID:** EL9-CI-001  
**Priority:** Medium  
**Objective:** Verify CircleCI Docker images build

**Steps:**
```bash
cd docker/circleci
./build_all_el9_images.sh
```

**Expected Results:**
- All three images build successfully

**Pass Criteria:**
- [ ] rpmbuild-hoot-release-el9 builds
- [ ] rpmbuild-repo-el9 builds
- [ ] rpmbuild-generic-el9 builds

---

### 10. Performance Tests

#### 10.1 Build Performance

**Test ID:** EL9-PERF-001  
**Priority:** Low  
**Objective:** Compare build times EL7 vs EL9

**Steps:**
```bash
# On EL9
time make -sj$(nproc)
# Compare with EL7 baseline
```

**Expected Results:**
- EL9 build time is similar or faster than EL7

**Pass Criteria:**
- [ ] Build completes in reasonable time
- [ ] Performance is acceptable

---

## Test Execution Checklist

### Pre-Test Setup
- [ ] Rocky Linux 9 system available
- [ ] VirtualBox/VMware installed (for Vagrant)
- [ ] Docker installed (for Docker tests)
- [ ] Git repository cloned
- [ ] Network connectivity verified

### Test Execution Order
1. [ ] Installation Tests (1.1, 1.2, 1.3)
2. [ ] Dependency Tests (2.1, 2.2)
3. [ ] Build Tests (3.1, 3.2)
4. [ ] Runtime Tests (4.1, 4.2, 4.3)
5. [ ] Test Suite (5.1, 5.2, 5.3)
6. [ ] Service Tests (6.1, 6.2)
7. [ ] Compatibility Tests (7.1)
8. [ ] Docker Tests (8.1)
9. [ ] CI/CD Tests (9.1)
10. [ ] Performance Tests (10.1)

### Post-Test Activities
- [ ] Document all failures
- [ ] Create bug reports for issues
- [ ] Update documentation based on findings
- [ ] Archive test results

## Test Results Template

For each test, record:
- **Test ID:**
- **Date Executed:**
- **Tester:**
- **Result:** PASS / FAIL / BLOCKED
- **Notes:**
- **Issues Found:**

## Success Criteria

The EL9 migration is considered successful when:
- [ ] All High priority tests pass
- [ ] >= 90% of Medium priority tests pass
- [ ] All critical functionality works
- [ ] No data loss or corruption
- [ ] Performance is acceptable
- [ ] Documentation is complete

## Known Issues / Limitations

(To be filled in during testing)

## Sign-Off

- **Test Lead:** _____________________ Date: _______
- **Development Lead:** _____________________ Date: _______
- **Project Manager:** _____________________ Date: _______

---

**Document Version:** 1.0  
**Last Updated:** 2026-02-26

