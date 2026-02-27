# CircleCI Docker Images for EL9

This directory contains Dockerfiles and build scripts for the CircleCI Docker images required for Hootenanny's EL9 migration.

## Overview

Three Docker images are required for CircleCI to build and test Hootenanny on Rocky Linux 9:

1. **rpmbuild-hoot-release-el9** - RPM build environment for Hootenanny releases
2. **rpmbuild-repo-el9** - Repository management and publishing
3. **rpmbuild-generic-el9** - Generic RPM build environment with all dependencies

## Directory Structure

```
docker/circleci/
├── README.md                           # This file
├── build_all_el9_images.sh            # Build all images
├── push_all_el9_images.sh             # Push all images to Docker Hub
├── rpmbuild-hoot-release-el9/
│   ├── Dockerfile
│   └── build_image.sh
├── rpmbuild-repo-el9/
│   ├── Dockerfile
│   └── build_image.sh
└── rpmbuild-generic-el9/
    ├── Dockerfile
    └── build_image.sh
```

## Building the Images

### Build All Images

The easiest way to build all three images:

```bash
cd docker/circleci
./build_all_el9_images.sh
```

### Build Individual Images

To build a specific image:

```bash
# Build rpmbuild-hoot-release-el9
cd docker/circleci/rpmbuild-hoot-release-el9
./build_image.sh

# Build rpmbuild-repo-el9
cd docker/circleci/rpmbuild-repo-el9
./build_image.sh

# Build rpmbuild-generic-el9
cd docker/circleci/rpmbuild-generic-el9
./build_image.sh
```

## Pushing to Docker Hub

### Prerequisites

1. Docker Hub account with access to the `hootenanny` organization
2. Login to Docker Hub:
   ```bash
   docker login
   ```

### Push All Images

```bash
cd docker/circleci
./push_all_el9_images.sh
```

### Push Individual Images

```bash
docker push hootenanny/rpmbuild-hoot-release-el9:latest
docker push hootenanny/rpmbuild-repo-el9:latest
docker push hootenanny/rpmbuild-generic-el9:latest
```

## Image Details

### rpmbuild-hoot-release-el9

**Purpose:** RPM build environment for Hootenanny releases

**Base Image:** rockylinux:9

**Installed Packages:**
- Development Tools group
- RPM build tools (rpm-build, rpmdevtools, rpmlint)
- Repository tools (createrepo)
- Java 11 OpenJDK
- Git, wget, curl, tar, compression tools

**User:** `builder` (non-root user for RPM building)

**Usage in CircleCI:**
```yaml
docker:
  - image: hootenanny/rpmbuild-hoot-release-el9:latest
```

### rpmbuild-repo-el9

**Purpose:** Repository management and publishing

**Base Image:** rockylinux:9

**Installed Packages:**
- createrepo_c (repository creation)
- rpm-sign, gnupg2 (package signing)
- Python 3 with boto3 (AWS S3 integration)
- rsync, git, wget, curl

**User:** `repobuilder` (non-root user for repository management)

**Usage in CircleCI:**
```yaml
docker:
  - image: hootenanny/rpmbuild-repo-el9:latest
```

### rpmbuild-generic-el9

**Purpose:** Generic RPM build environment with comprehensive dependencies

**Base Image:** rockylinux:9

**Installed Packages:**
- Development Tools group
- RPM build tools
- Java 11, Maven, Node.js
- Python 3 with development headers
- Qt5 development libraries
- Geospatial libraries (GDAL, GEOS, PROJ)
- Database libraries (PostgreSQL)
- Common build dependencies (Boost, Protocol Buffers, OpenCV)

**User:** `builder` (non-root user for building)

**Usage in CircleCI:**
```yaml
docker:
  - image: hootenanny/rpmbuild-generic-el9:latest
```

## Testing the Images

### Interactive Testing

Test an image interactively:

```bash
# Test rpmbuild-hoot-release-el9
docker run -it --rm hootenanny/rpmbuild-hoot-release-el9:latest /bin/bash

# Inside the container, verify tools
java -version          # Should show OpenJDK 11
rpm --version          # Should show RPM version
createrepo --version   # Should show createrepo version
```

### Verify Build Environment

```bash
# Test rpmbuild-generic-el9
docker run -it --rm hootenanny/rpmbuild-generic-el9:latest /bin/bash

# Inside the container
gcc --version          # Should show GCC 11.x
cmake --version        # Should show CMake 3.20+
node --version         # Should show Node.js
mvn --version          # Should show Maven
```

## Updating the Images

When dependencies need to be updated:

1. Edit the appropriate `Dockerfile`
2. Rebuild the image:
   ```bash
   cd docker/circleci/<image-name>
   ./build_image.sh
   ```
3. Test the image
4. Push to Docker Hub:
   ```bash
   docker push hootenanny/<image-name>:latest
   ```

## CircleCI Integration

These images are referenced in `.circleci/config.yml`:

```yaml
jobs:
  archive-create:
    docker:
      - image: hootenanny/rpmbuild-hoot-release-el9:latest
    # ... job configuration

  archive-upload:
    docker:
      - image: hootenanny/rpmbuild-repo-el9:latest
    # ... job configuration
```

## Troubleshooting

### Build Failures

**Issue:** Docker build fails with "No space left on device"

**Solution:** Clean up Docker:
```bash
docker system prune -a
```

**Issue:** Package installation fails

**Solution:** Update the base image:
```bash
docker pull rockylinux:9
```

### Push Failures

**Issue:** "denied: requested access to the resource is denied"

**Solution:** Ensure you're logged in and have access to the hootenanny organization:
```bash
docker login
```

## Maintenance

- **Update Frequency:** Review and update quarterly or when major dependency updates are needed
- **Security:** Rebuild images when Rocky Linux 9 security updates are released
- **Testing:** Always test images locally before pushing to Docker Hub

## Additional Resources

- [CircleCI Configuration](.circleci/config.yml)
- [EL9 Migration Guide](docs/EL9-Migration-Guide.md)
- [Docker Documentation](../Docker.md)

