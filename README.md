# JDUMPERTOOLS

I wanted to have a few very fast set of programs to dump easy to get information from my Linux system into a JSON enabled search engine. I got tired of writting Python wrappers for them so here is the result.

They do not intend to replace tools like 'du' or 'who' and their functionality is VERY limited on purpouse. Their goal is:

* Write the output in JSON format
* Be easy to use
* Be fast

You can manipulate their output with other cool tools like [Python](https://www.python.org/), [JQ](https://stedolan.github.io/jq/), etc.


- Jose Vicente Nunez

# Compilation and installation

You will need:

* gcc 8.3.1+ and Make 4.2.1+ to compile
* RPM build 4.14+ if you want to create an RPM
* Docker 19.03+ if you want to create a docker image

## Example build session

I assume you have the necessary expertise to install and use tools like [Make](https://en.wikipedia.org/wiki/Makefile), [rpmbuild](https://www.redhat.com/sysadmin/create-rpm-package), [Docker build](https://docs.docker.com/engine/reference/commandline/build/) on your system. You are more than welcome to contribute documentation on how to do these tasks.


```
# To compile
make all

# To compile and install
make install

# To build an RPM
rpmbuild -ba jdumpertools.spec

# To create a Docker image
docker build -f Dockerfile
```

# Bugs / feature request

This software was tested on Fedora 29, Fedora 31 Linux. If you find a bug please report it on GitHub. Do the same if you want a feature to be added to the tools.
