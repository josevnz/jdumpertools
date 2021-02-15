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

## RPM
```
rpm -Uhv jdumpertools-0-1.fc33.x86_64.rpm
```

## Docker

__Comming soon__

## From source

Just copy the libjdumpertools.so to your $$HOME/lib directory and jdu, jutmp to $HOME/bin. You will need to update your LD_LIBRARY_PATH:
```
# Add this to your ~/.bash_profile
export LD_LIBRARY_PATH=$HOME/lib:$LD_LIBRARY_PATH
```


# Bugs / feature request

This software was tested on Fedora 33.

If you find a bug please report it on GitHub.

