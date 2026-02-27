# Hootenanny EL9 Migration Guide

## Overview

This document describes the migration of Hootenanny from Enterprise Linux 7 (CentOS 7) to Enterprise Linux 9 (Rocky Linux 9). This migration brings Hootenanny to a modern, supported operating system with updated toolchains and dependencies.

## Executive Summary

**Migration Status:** Complete  
**Target OS:** Rocky Linux 9 (EL9)  
**Legacy OS:** CentOS 7 (EL7) - still supported for backward compatibility  
**Backward Compatibility:** All scripts auto-detect OS version and adapt accordingly

## Why Migrate to EL9?

1. **CentOS 7 End of Life:** CentOS 7 reached EOL on June 30, 2024
2. **Modern Toolchain:** Native GCC 11+ support without devtoolset
3. **Updated Dependencies:** Latest stable versions of all major dependencies
4. **Security:** Continued security updates and patches
5. **Performance:** Improved compiler optimizations and newer libraries

## Major Changes

### Operating System & Package Management

| Component | EL7 | EL9 |
|-----------|-----|-----|
| **OS Distribution** | CentOS 7 | Rocky Linux 9 |
| **Package Manager** | yum | dnf |
| **Init System** | systemd (v219) | systemd (v252) |

### Compiler & Build Tools

| Component | EL7 | EL9 |
|-----------|-----|-----|
| **GCC** | devtoolset-8 (GCC 8.x) | Native GCC 11.x |
| **C++ Standard** | C++11/14 | C++17 (default) |
| **CMake** | 2.8.12 | 3.20+ |
| **Autotools** | autoconf 2.69 | autoconf 2.71 |

**Breaking Change:** No longer requires devtoolset installation. GCC 11 is available natively.

### Runtime Dependencies

| Component | EL7 | EL9 | Notes |
|-----------|-----|-----|-------|
| **Java** | OpenJDK 1.8.0 | OpenJDK 11 | Java 8 no longer supported |
| **Java Library Path** | jre/lib/amd64/server | lib/server | Path structure changed |
| **Tomcat** | 8.5.x | 9.0.x | Major version upgrade |
| **Node.js** | 16.20.2 | 18.19.0 | LTS version |
| **PostgreSQL** | 13 | 15 | Major version upgrade |
| **Python** | 2.7 + 3.6 | 3.9 only | **Python 2 removed** |

**Breaking Change:** Python 2 is no longer available. All Python scripts must be Python 3 compatible.

### Geospatial Libraries

| Component | EL7 | EL9 | Notes |
|-----------|-----|-----|-------|
| **GDAL** | 3.8.0 | 3.8.3 | Minor update |
| **GEOS** | 3.11.3 | 3.12.1 | Minor update |
| **PROJ** | 7.2.1 | 9.3.1 | Major update |
| **Qt** | 5.15.x | 5.15.x | Same version |

### Other Dependencies

| Component | EL7 | EL9 |
|-----------|-----|-----|
| **Boost** | 1.84.0 | 1.84.0 |
| **Protocol Buffers** | 3.19.6 | 3.19.6 |
| **OpenCV** | 4.8.1 | 4.8.1 |
| **libphonenumber** | 8.13.27 | 8.13.27 |
| **libpostal** | 1.1 | 1.1 |

## File Changes by Category

### 1. Provisioning Scripts (New Files)

**Created:**
- `VagrantProvisionEL9.sh` - Main EL9 provisioning script
- `VagrantProvisionEL9Deps.sh` - EL9 dependency installation
- `VagrantProvisionEL9Rpm.sh` - EL9 RPM-based installation
- `scripts/tomcat/el9/tomcat9_install.sh` - Tomcat 9 installation for EL9

**Modified:**
- `VagrantProvisionVars.sh` - Updated all version numbers for EL9

### 2. Repository Configuration Scripts

**Modified (Auto-detection added):**
- `scripts/yum/geoint-repo.sh` - Detects EL version, uses appropriate repo URL
- `scripts/yum/hoot-repo.sh` - Detects EL version
- `scripts/yum/pgdg-repo.sh` - Detects package manager (yum vs dnf)

### 3. Environment Setup Scripts

**Modified:**
- `scripts/HootEnv.sh` - Java 11 detection, conditional devtoolset loading

### 4. Docker Infrastructure

**Modified (Rocky Linux 9 base):**
- `docker/Dockerfile.core-services` - Rocky Linux 9, dnf, Tomcat 9
- `docker/Dockerfile.postgres` - Rocky Linux 9, dnf
- `docker/Dockerfile.frontend` - Rocky Linux 9, dnf, Node.js 18
- `docker/scripts/core-services-entrypoint.sh` - Conditional devtoolset

### 5. Build System

**Modified:**
- `configure.ac` - Accepts both EL7 and EL9 library versions
- `scripts/tomcat/CopyWebAppsToTomcat.sh` - Auto-detects Tomcat 8 or 9
- `docker/scripts/tomcat_configure.sh` - Auto-detects Tomcat version
- `scripts/tomcat/configure_tomcat_ssl.sh` - Auto-detects Tomcat version
- `scripts/tomcat/configure_oauth_8443.sh` - Auto-detects Tomcat version
- `docker/scripts/postgres-install.sh` - Auto-detects package manager

### 6. CI/CD Configuration

**Modified:**
- `Vagrantfile` - Added `hoot_el9` and `hoot_el9_rpm` VM definitions
- `.circleci/config.yml` - Updated for EL9 Docker images

### 7. Documentation

**Modified:**
- `docs/install/Vagrant.asciidoc` - Added EL9 instructions
- `docs/install/InstallationHeader.asciidoc` - Mentions EL9 support
- `Docker.md` - Added EL9 system requirements
- `README.md` - Added supported OS section

## Installation Instructions

### Option 1: Vagrant with RPM (Quickest)

**EL9 (Recommended):**
```bash
git clone git@github.com:ngageoint/hootenanny.git hoot
cd hoot
vagrant up hoot_el9_rpm
```

**EL7 (Legacy):**
```bash
vagrant up hoot_centos7_rpm
```

### Option 2: Vagrant Build from Source

**EL9 (Recommended):**
```bash
git clone git@github.com:ngageoint/hootenanny.git hoot
cd hoot
git submodule init
git submodule update
vagrant up hoot_el9
```

**EL7 (Legacy):**
```bash
vagrant up hoot_centos7
```

### Option 3: Docker

```bash
git clone git@github.com:ngageoint/hootenanny.git hoot
cd hoot
make -f Makefile.docker up
```

**Note:** Docker images now use Rocky Linux 9 by default.

## Upgrading from EL7 to EL9

### For Vagrant Users

1. **Destroy existing EL7 VM:**
   ```bash
   vagrant destroy hoot_centos7
   ```

2. **Bring up EL9 VM:**
   ```bash
   vagrant up hoot_el9
   ```

3. **Migrate data if needed:**
   - Export PostgreSQL databases from EL7 VM
   - Import into EL9 VM after provisioning

### For Docker Users

1. **Stop existing containers:**
   ```bash
   make -f Makefile.docker down
   ```

2. **Remove old images (optional):**
   ```bash
   docker images | grep centos | awk '{print $3}' | xargs docker rmi
   ```

3. **Rebuild with EL9:**
   ```bash
   make -f Makefile.docker up
   ```

### For Bare Metal Installations

1. **Backup your data:**
   - PostgreSQL databases
   - Configuration files
   - Custom translations/scripts

2. **Install Rocky Linux 9**

3. **Clone Hootenanny:**
   ```bash
   git clone git@github.com:ngageoint/hootenanny.git hoot
   cd hoot
   ```

4. **Run EL9 provisioning:**
   ```bash
   ./VagrantProvisionEL9.sh
   ```

5. **Build Hootenanny:**
   ```bash
   source ./SetupEnv.sh
   ./configure
   make -sj$(nproc)
   ```

## Breaking Changes & Migration Notes

### 1. Python 2 Removal

**Impact:** Any custom Python scripts using Python 2 will fail.

**Migration:**
- Update all Python scripts to Python 3 syntax
- Change shebangs from `#!/usr/bin/python` to `#!/usr/bin/python3`
- Update print statements: `print "text"` → `print("text")`
- Update imports: `import ConfigParser` → `import configparser`

### 2. Java 11 Migration

**Impact:** Java 8 specific code may need updates.

**Migration:**
- Review deprecated APIs removed in Java 11
- Update Maven compiler plugin to target Java 11
- Test all Java services thoroughly

### 3. Tomcat 8 → Tomcat 9

**Impact:** Configuration file locations and some APIs changed.

**Migration:**
- Configuration files moved from `/etc/tomcat8/` to `/etc/tomcat9/`
- Web apps directory: `/usr/share/tomcat8/` → `/usr/share/tomcat9/`
- Service name: `tomcat8` → `tomcat9`
- All scripts auto-detect the version, but manual commands need updating

**Commands:**
```bash
# EL7
sudo systemctl restart tomcat8

# EL9
sudo systemctl restart tomcat9
```

### 4. PostgreSQL 13 → PostgreSQL 15

**Impact:** Database dump/restore required for migration.

**Migration:**
```bash
# On EL7 system - export data
pg_dumpall -U postgres > all_databases.sql

# On EL9 system - import data
psql -U postgres -f all_databases.sql
```

### 5. Devtoolset Removal

**Impact:** No longer need to enable devtoolset.

**Migration:**
- Remove `scl enable devtoolset-8 bash` commands
- GCC 11 is available by default
- Scripts automatically handle this via `HootEnv.sh`

### 6. Package Manager Change (yum → dnf)

**Impact:** Manual package installation commands need updating.

**Migration:**
```bash
# EL7
sudo yum install package-name

# EL9
sudo dnf install package-name
```

**Note:** Most scripts auto-detect and use the correct package manager.

## Testing the Migration

### 1. Verify Installation

```bash
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh

# Check versions
java -version          # Should show OpenJDK 11
node --version         # Should show v18.x
psql --version         # Should show 15.x
gcc --version          # Should show 11.x
python3 --version      # Should show 3.9.x
```

### 2. Build Test

```bash
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh
make clean
make -sj$(nproc)
```

### 3. Run Tests

```bash
vagrant ssh hoot_el9
cd hoot
source ./SetupEnv.sh
make -sj$(nproc) test-all
```

### 4. Web Services Test

```bash
# Start Tomcat
sudo systemctl start tomcat9

# Check status
sudo systemctl status tomcat9

# Access UI
# Open browser to https://localhost:8443/hootenanny-id/
```

## Troubleshooting

### Issue: "devtoolset-8 not found"

**Solution:** This is expected on EL9. The scripts should automatically skip devtoolset. If you see this error, ensure you're using the latest `HootEnv.sh`.

### Issue: "tomcat8 service not found"

**Solution:** On EL9, use `tomcat9`:
```bash
sudo systemctl status tomcat9
```

### Issue: "Python 2 command not found"

**Solution:** Update scripts to use `python3` instead of `python` or `python2`.

### Issue: Java library path errors

**Solution:** EL9 uses `lib/server` instead of `jre/lib/amd64/server`. The scripts should auto-detect this. If issues persist, check `JAVA_HOME` and library paths.

### Issue: PostgreSQL connection failures

**Solution:** Ensure PostgreSQL 15 is running:
```bash
sudo systemctl status postgresql-15
```

## CircleCI Docker Images

The following Docker images need to be built for CircleCI:

1. **hootenanny/rpmbuild-hoot-release-el9:latest**
2. **hootenanny/rpmbuild-repo-el9:latest**
3. **hootenanny/rpmbuild-generic-el9:latest**

See `docs/CircleCI-EL9-Docker-Images.md` for build instructions.

## Backward Compatibility

All scripts have been designed to maintain backward compatibility with EL7:

- **Auto-detection:** Scripts detect OS version and adapt
- **Dual support:** Both EL7 and EL9 can be used
- **Gradual migration:** Teams can migrate at their own pace

## Performance Improvements

Expected performance improvements on EL9:

- **Faster compilation:** GCC 11 optimizations
- **Better runtime performance:** Updated libraries
- **Improved I/O:** Newer kernel and filesystem improvements
- **Enhanced security:** Modern security features enabled by default

## Support & Maintenance

- **EL9:** Primary supported platform, receives all updates
- **EL7:** Legacy support, critical fixes only
- **EOL Timeline:** EL7 support will be phased out in future releases

## Additional Resources

- [Vagrant Installation Guide](install/Vagrant.asciidoc)
- [Docker Installation Guide](../Docker.md)
- [CircleCI Configuration](.circleci/config.yml)
- [Hootenanny User Guide](HootenannyUserGuide.pdf)

## Changelog

### Version 1.0 - Initial EL9 Migration
- Complete migration to Rocky Linux 9
- All 5 phases completed
- Backward compatibility maintained
- Documentation updated

---

**Document Version:** 1.0
**Last Updated:** 2026-02-26
**Authors:** Hootenanny Development Team


