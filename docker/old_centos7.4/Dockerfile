FROM centos:centos7.4.1708
ENV container docker
ENV LANG en_US.UTF-8
ENV LC_ALL en_US.UTF-8
#RUN (cd /lib/systemd/system/sysinit.target.wants/; for i in *; do [ $i == \
#systemd-tmpfiles-setup.service ] || rm -f $i; done); \
#rm -f /lib/systemd/system/multi-user.target.wants/*;\
#rm -f /etc/systemd/system/*.wants/*;\
#rm -f /lib/systemd/system/local-fs.target.wants/*; \
#rm -f /lib/systemd/system/sockets.target.wants/*udev*; \
#rm -f /lib/systemd/system/sockets.target.wants/*initctl*; \
#rm -f /lib/systemd/system/basic.target.wants/*;\
#rm -f /lib/systemd/system/anaconda.target.wants/*;
RUN yum -y install openssh openssh-server openssh-clients openssl-libs; yum clean all; systemctl enable sshd.service
RUN rpm --rebuilddb && yum install -y nano wget ntp curl sudo vim python mlocate bt file GConf2
RUN chkconfig ntpd on
RUN chkconfig sshd on
RUN useradd vagrant
RUN mkdir -m 0700 -p /home/vagrant/.ssh
RUN curl https://raw.githubusercontent.com/mitchellh/vagrant/master/keys/vagrant.pub >> /home/vagrant/.ssh/authorized_keys
RUN chmod 600 /home/vagrant/.ssh/authorized_keys
RUN chown -R vagrant:vagrant /home/vagrant/.ssh
RUN sed -i 's/^\(Defaults.*requiretty\)/#\1/' /etc/sudoers
RUN cat /etc/sudoers

RUN localedef --quiet -c -i en_US -f UTF-8 en_US.UTF-8
RUN echo "export LANG=en_US.UTF-8" >> ~/.bashrc
RUN echo "" >> ~/.bashrc

RUN echo "vagrant ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
#RUN vim /etc/sysconfig/network-scripts/ifcfg-eth0
#RUN vi /etc/sysconfig/network-scripts/ifcfg-eth0
RUN rm -f /etc/udev/rules.d/70-persistent-net.rules
RUN yum clean all
RUN rm -rf /tmp/*
RUN rm -f /var/log/wtmp /var/log/btmp
EXPOSE 22
EXPOSE 80

VOLUME [ "/sys/fs/cgroup" ]
CMD ["/usr/sbin/init"]

