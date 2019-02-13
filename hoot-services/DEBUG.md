### How to debug services code interactively...

1. Update your VagrantFileLocal with the following line and (if already running) reload your vm

```
config.vm.network :forwarded_port, :guest => 8585, :host => 8585
```

...in your terminal

```
vagrant reload
```


2. ssh'd into your vagrant vm change the JAVA_OPTS variable to...

```
JAVA_OPTS="-Djava.awt.headless=true -Djava.security.egd=file:/dev/./urandom -Djdk.tls.ephemeralDHKeySize=2048 -Xdebug -Xrunjdwp:transport=dt_socket,address=8585,server=y,suspend=n"
```

3. Restart tomcat8 (in the vagrant box)

```
sudo service tomcat8 restart
```

4. (Using eclipse) add a new debug configuration that attaches to the service at port 8585

 - debug configurations -> Remove Java Application ...
 - set host as localhost
 - port as 8585
 - click apply
 - click debug
