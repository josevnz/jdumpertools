# Compilation

You will need:

* gcc 8.3.1+ and Make 4.2.1+ to compile
* RPM build 4.14+ if you want to create an RPM
* Docker 19.03+ if you want to create a docker image

## Example build session

I assume you have the necessary expertise to install and use tools like [Make](https://en.wikipedia.org/wiki/Makefile), [rpmbuild](https://www.redhat.com/sysadmin/create-rpm-package), [Docker build](https://docs.docker.com/engine/reference/commandline/build/) on your system. You are more than welcome to contribute documentation on how to do these tasks.


```
# To compile
make all

# To build an RPM
# rpmlint jdumpertools.spec
# rpmdev-setuptree
make rpm

# To create a Docker image
make docker
```

# Installation

```

```

# Bugs / feature request

This software was tested on Fedora 29 Linux. 

If you find a bug please report it on GitHub.

