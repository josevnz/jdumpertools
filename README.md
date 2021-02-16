# JDUMPERTOOLS

I wanted to have a few very fast set of programs to dump easy to get information from my Linux system into a JSON enabled search engine. I got tired of writing Python wrappers for them so here is the result.

They do not intend to replace tools like 'du' or 'who' and their functionality is VERY limited on purpose. Their goal is:

* Write the output in JSON format
* Be easy to use
* Be fast

You can manipulate their output with other cool programing languajes like [Python](https://www.python.org/), or tools like [JQ](https://stedolan.github.io/jq/).

# Author
- Jose Vicente Nunez (kodegeek.com@protonmail.com)

# Running the tools

## Bare-metal

### jdu
```
/usr/bin/jdu /
[{"partition": "/", "free_space": 492667166720.000000, "total_space": 510405902336.000000}]
```

### jutmp
```
/usr/bin/jutmp && /bin/cat /tmp/jutmp.json
INFO: Opened '/var/log/wtmp' for reading (saving results to /tmp/jutmp.json)
[{ "timestamp": "Sun Jan 31 20:30:32 2021", "user": "reboot", "pid": 0, "tty": "~",
....
]}
```

## Docker container

You will need to pull the image from DockerHub:

```
docker pull josevnz/jdumpertools
```

Then you will need to use bind volumes so the tools can access the bare-metal files needed to produce the reports.


### jdu
```
docker run --interactive --tty --rm --mount type=bind,source=/var/log/wtmp,target=/var/log/wtmp,readonly --mount type=bind,source=/,target=/mnt/slash,readonly josevnz/jdumpertools:0 -c "/usr/bin/jdu /mnt/slash"
[{"partition": "/mnt/slash", "free_space": 492667166720.000000, "total_space": 510405902336.000000}]
```

### jutmp
```
docker run --interactive --tty --rm --mount type=bind,source=/var/log/wtmp,target=/var/log/wtmp,readonly --mount type=bind,source=/,target=/mnt/slash,readonly josevnz/jdumpertools:0 -c "/usr/bin/jutmp && /bin/cat /tmp/jutmp.json"
INFO: Opened '/var/log/wtmp' for reading (saving results to /tmp/jutmp.json)
[{ "timestamp": "Sun Jan 31 20:30:32 2021", "user": "reboot", "pid": 0, "tty": "~",
....
]}
```