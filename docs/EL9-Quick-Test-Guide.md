# Quick Test Guide for EL9 Build

## Your System

You're running **Rocky Linux 9.7** - perfect for testing the EL9 migration!

```
NAME="Rocky Linux"
VERSION="9.7 (Blue Onyx)"
```

## Quick Test Steps

### Step 1: Install Dependencies

Run the EL9 dependency installation script:

```bash
cd /home/ti003319/src/hootenanny
sudo ./VagrantProvisionEL9Deps.sh
```

This will install all required dependencies for EL9 including:
- GCC 11+ (native, no devtoolset needed)
- Java 11 OpenJDK
- Node.js 18
- PostgreSQL 15
- Tomcat 9
- GDAL 3.8.3, GEOS 3.12.1, PROJ 9.3.1
- All build tools

### Step 2: Verify Dependency Versions

After installation, verify the versions:

```bash
# Check compiler
gcc --version          # Should show GCC 11.x

# Check Java
java -version          # Should show OpenJDK 11.x

# Check Node.js
node --version         # Should show v18.x

# Check PostgreSQL
psql --version         # Should show 15.x

# Check Python
python3 --version      # Should show 3.9.x

# Check geospatial libraries
gdal-config --version  # Should show 3.8.3
geos-config --version  # Should show 3.12.1
proj --version         # Should show 9.3.1
```

### Step 3: Set Up Environment

```bash
cd /home/ti003319/src/hootenanny
source ./SetupEnv.sh
```

This will:
- Set JAVA_HOME to Java 11
- Set up HOOT_HOME
- Configure paths
- NOT load devtoolset (not needed on EL9)

### Step 4: Configure the Build

```bash
cd /home/ti003319/src/hootenanny

# Generate build files
aclocal && autoconf && autoheader && automake

# Configure with services
./configure --with-services
```

**Expected output:**
- No errors about missing dependencies
- GDAL version 3.8.3 detected
- GEOS version 3.12.1 detected
- Java 11 detected
- PostgreSQL 15 detected

### Step 5: Build Hootenanny

```bash
# Build using all available cores
make -sj$(nproc)
```

**Expected results:**
- Build completes without errors
- All binaries created in bin/
- Build time: ~30-60 minutes depending on system

**Watch for:**
- ✅ No compiler errors
- ✅ No missing library errors
- ✅ All components compile successfully

### Step 6: Verify the Build

```bash
# Check that hoot binary was created
./bin/hoot --version

# Should output version information
```

### Step 7: Run Core Tests

```bash
# Run the core test suite
make -sj$(nproc) test
```

**Expected results:**
- Most tests pass (>99% pass rate)
- No segmentation faults
- Test execution completes

### Step 8: Install Services (Optional)

If you want to test the web services:

```bash
# Build services
make -sj$(nproc) services-build

# Copy to Tomcat
sudo scripts/tomcat/CopyWebAppsToTomcat.sh

# Restart Tomcat 9
sudo systemctl restart tomcat9

# Check Tomcat status
sudo systemctl status tomcat9

# View Tomcat logs
sudo journalctl -eu tomcat9
```

### Step 9: Test Services (Optional)

```bash
# Test services endpoints
curl -k https://localhost:8443/hoot-services/info/about/servicesVersionInfo
curl -k https://localhost:8443/hoot-services/info/about/coreVersionInfo
```

### Step 10: Run Full Test Suite (Optional)

```bash
# Run all tests (takes 1-2 hours)
make -sj$(nproc) test-all
```

## Quick Verification Checklist

- [ ] Dependencies installed successfully
- [ ] GCC 11+ available (no devtoolset)
- [ ] Java 11 detected
- [ ] Configure completes without errors
- [ ] Make completes without errors
- [ ] `hoot --version` works
- [ ] Core tests pass
- [ ] Tomcat 9 starts (if testing services)
- [ ] Services respond (if testing services)

## Common Issues and Solutions

### Issue: Missing dependencies

**Solution:**
```bash
# Re-run dependency installation
sudo ./VagrantProvisionEL9Deps.sh
```

### Issue: Configure fails with library version mismatch

**Solution:**
Check that configure.ac accepts both EL7 and EL9 versions:
```bash
grep -A5 "GDAL" configure.ac
grep -A5 "GEOS" configure.ac
```

### Issue: Java not found

**Solution:**
```bash
# Verify Java 11 is installed
sudo dnf install java-11-openjdk java-11-openjdk-devel

# Set JAVA_HOME
export JAVA_HOME=/usr/lib/jvm/java-11-openjdk
```

### Issue: Tomcat service not found

**Solution:**
```bash
# Verify Tomcat 9 is installed
sudo dnf install tomcat9

# Check service status
sudo systemctl status tomcat9
```

## Expected Build Output

When everything works correctly, you should see:

```
✅ Dependencies: All installed
✅ Configure: Success
✅ Build: Success (30-60 minutes)
✅ Tests: >99% pass rate
✅ Services: Running on Tomcat 9
```

## Next Steps After Successful Build

1. Document any issues encountered
2. Run the full test suite
3. Test Docker builds
4. Build CircleCI images
5. Consider creating a PR

## Need Help?

- Check `docs/EL9-Migration-Guide.md` for detailed migration info
- Check `docs/EL9-Test-Plan.md` for comprehensive testing
- Check `docs/CI-CD-EL9-Guide.md` for CI/CD setup

