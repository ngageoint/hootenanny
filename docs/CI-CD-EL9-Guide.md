# CI/CD Configuration for EL9

## Overview

This document describes the CI/CD configuration changes for the Hootenanny EL9 migration, covering CircleCI, Jenkins, and related automation.

## CircleCI Configuration

### Updated Configuration File

**File:** `.circleci/config.yml`

### Key Changes

#### 1. Docker Images

The CircleCI configuration now uses EL9-based Docker images:

**Previous (EL7):**
```yaml
docker:
  - image: hootenanny/rpmbuild-hoot-release:latest  # CentOS 7
```

**Current (EL9):**
```yaml
docker:
  - image: hootenanny/rpmbuild-hoot-release-el9:latest  # Rocky Linux 9
```

#### 2. Required Docker Images

Three Docker images are required for CircleCI:

1. **hootenanny/rpmbuild-hoot-release-el9:latest**
   - Purpose: RPM build environment for releases
   - Base: Rocky Linux 9
   - Includes: GCC 11, Java 11, RPM build tools

2. **hootenanny/rpmbuild-repo-el9:latest**
   - Purpose: Repository management and publishing
   - Base: Rocky Linux 9
   - Includes: createrepo_c, rpm-sign, boto3

3. **hootenanny/rpmbuild-generic-el9:latest**
   - Purpose: Generic RPM builds with all dependencies
   - Base: Rocky Linux 9
   - Includes: Full development environment, geospatial libraries

### Building CircleCI Docker Images

#### Location

All CircleCI Docker image definitions are in:
```
docker/circleci/
├── rpmbuild-hoot-release-el9/
├── rpmbuild-repo-el9/
└── rpmbuild-generic-el9/
```

#### Build All Images

```bash
cd docker/circleci
./build_all_el9_images.sh
```

#### Build Individual Images

```bash
# Build release image
cd docker/circleci/rpmbuild-hoot-release-el9
./build_image.sh

# Build repo image
cd docker/circleci/rpmbuild-repo-el9
./build_image.sh

# Build generic image
cd docker/circleci/rpmbuild-generic-el9
./build_image.sh
```

#### Push to Docker Hub

**Prerequisites:**
- Docker Hub account with access to `hootenanny` organization
- Login: `docker login`

**Push all images:**
```bash
cd docker/circleci
./push_all_el9_images.sh
```

**Push individual images:**
```bash
docker push hootenanny/rpmbuild-hoot-release-el9:latest
docker push hootenanny/rpmbuild-repo-el9:latest
docker push hootenanny/rpmbuild-generic-el9:latest
```

### CircleCI Jobs

#### Primary Jobs (EL9)

**archive-create:**
- Uses: `hootenanny/rpmbuild-hoot-release-el9:latest`
- Purpose: Build RPM packages from source
- Environment: Rocky Linux 9, Java 11, GCC 11

**archive-upload:**
- Uses: `hootenanny/rpmbuild-repo-el9:latest`
- Purpose: Upload and publish RPM packages
- Environment: Rocky Linux 9, repository tools

**lint:**
- Uses: `cimg/base:ubuntu-2204:2023.10.1`
- Purpose: Code linting and style checks
- Updated to Ubuntu 22.04 for modern tooling

#### Legacy Jobs (EL7)

**archive-create-el7:**
- Uses: `hootenanny/rpmbuild-hoot-release:latest`
- Purpose: Legacy EL7 builds (optional)
- Can be removed once EL9 is stable

### Workflow Configuration

```yaml
workflows:
  version: 2
  tests:
    jobs:
      # Primary EL9 workflow
      - archive-create
      - archive-upload:
          requires:
            - archive-create
      - copyright
      - lint
      # Legacy EL7 workflow (optional)
      - archive-create-el7
```

## Jenkins Configuration

### Jenkinsfile Locations

- **Main:** `scripts/jenkins/Jenkinsfile`
- **Valgrind:** `scripts/valgrind/Jenkinsfile`

### Jenkins Updates Needed

Jenkins configurations may need updates for EL9. Key areas to review:

1. **Agent Labels:**
   - Update agent labels to use EL9 nodes
   - Example: `agent { label 'el9' }`

2. **Docker Images:**
   - Use EL9-based Docker images if Jenkins uses Docker agents
   - Update Dockerfile references

3. **Environment Variables:**
   - Ensure `JAVA_HOME` points to Java 11
   - Update Tomcat paths to use `tomcat9`

4. **Build Commands:**
   - No changes needed - scripts auto-detect OS version

### Example Jenkins Pipeline Update

**Before (EL7):**
```groovy
agent {
    docker {
        image 'centos:7'
    }
}
```

**After (EL9):**
```groovy
agent {
    docker {
        image 'rockylinux:9'
    }
}
```

## Vagrant CI/CD

### Vagrant Box Updates

New Vagrant boxes for EL9:

**Available Boxes:**
- `hoot_el9` - Rocky Linux 9, build from source
- `hoot_el9_rpm` - Rocky Linux 9, RPM installation

**Legacy Boxes:**
- `hoot_centos7` - CentOS 7, build from source
- `hoot_centos7_rpm` - CentOS 7, RPM installation

### Vagrantfile Changes

The `Vagrantfile` now includes:

1. **OS Version Parameter:**
   ```ruby
   def set_provisioners(config, os_version = 'el7')
   ```

2. **Conditional Provisioning:**
   ```ruby
   if os_version == 'el9'
     config.vm.provision "hoot", type: "shell", 
       :privileged => false, 
       :path => "VagrantProvisionEL9.sh"
   else
     config.vm.provision "hoot", type: "shell", 
       :privileged => false, 
       :path => "VagrantProvisionCentOS7.sh"
   end
   ```

3. **Service Names:**
   - EL9 uses `tomcat9`
   - EL7 uses `tomcat8`

## Automated Testing in CI/CD

### Test Execution

All CI/CD pipelines should run the test suite:

```bash
source ./SetupEnv.sh
make -sj$(nproc) test-all
```

### Test Requirements

- **Core Tests:** Must pass
- **Services Tests:** Must pass
- **UI Tests:** Optional (require additional setup)

### Test Reporting

- JUnit XML format for test results
- Code coverage reports (if enabled)
- Performance metrics

## Deployment Automation

### RPM Deployment

1. **Build RPMs:**
   ```bash
   make rpm
   ```

2. **Sign RPMs:**
   ```bash
   rpm --addsign *.rpm
   ```

3. **Create Repository:**
   ```bash
   createrepo_c /path/to/repo
   ```

4. **Upload to Repository:**
   - S3, HTTP server, or package repository

### Docker Deployment

1. **Build Images:**
   ```bash
   make -f Makefile.docker build
   ```

2. **Tag Images:**
   ```bash
   docker tag hootenanny/core-services:latest hootenanny/core-services:el9-latest
   ```

3. **Push to Registry:**
   ```bash
   docker push hootenanny/core-services:el9-latest
   ```

## Environment Variables

### CircleCI Environment Variables

Set in CircleCI project settings:

- `DOCKER_USER` - Docker Hub username
- `DOCKER_PASS` - Docker Hub password/token
- `AWS_ACCESS_KEY_ID` - For S3 uploads (if used)
- `AWS_SECRET_ACCESS_KEY` - For S3 uploads (if used)

### Jenkins Environment Variables

Set in Jenkins configuration:

- `JAVA_HOME=/usr/lib/jvm/java-11-openjdk`
- `TOMCAT_HOME=/usr/share/tomcat9` (EL9)
- `HOOT_HOME=/path/to/hootenanny`

## Troubleshooting CI/CD

### CircleCI Issues

**Issue:** Docker image not found

**Solution:**
```bash
# Verify image exists on Docker Hub
docker pull hootenanny/rpmbuild-hoot-release-el9:latest

# If not, build and push
cd docker/circleci
./build_all_el9_images.sh
./push_all_el9_images.sh
```

**Issue:** Build fails with Java version error

**Solution:** Ensure Docker image has Java 11 installed

### Jenkins Issues

**Issue:** Agent not compatible

**Solution:** Update Jenkins agents to Rocky Linux 9 or use Docker agents

**Issue:** Tomcat service not found

**Solution:** Update scripts to use `tomcat9` instead of `tomcat8`

## Migration Checklist

### Pre-Migration
- [ ] Build all CircleCI Docker images
- [ ] Push images to Docker Hub
- [ ] Update Jenkins agents (if applicable)
- [ ] Test builds locally on EL9

### Migration
- [ ] Update `.circleci/config.yml`
- [ ] Update Jenkins pipelines
- [ ] Update Vagrantfile
- [ ] Test CI/CD pipelines

### Post-Migration
- [ ] Monitor first few builds
- [ ] Verify all tests pass
- [ ] Check deployment artifacts
- [ ] Update documentation

## Additional Resources

- [CircleCI Docker Images README](../docker/circleci/README.md)
- [EL9 Migration Guide](EL9-Migration-Guide.md)
- [EL9 Test Plan](EL9-Test-Plan.md)
- [CircleCI Configuration](.circleci/config.yml)

---

**Document Version:** 1.0  
**Last Updated:** 2026-02-26

