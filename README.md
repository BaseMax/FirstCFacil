# First C Facil

Building web service (API) using Facil in C programming language and test its benchmark and performance.

## Using

```
$ make
$ ./tmp/fioapp
INFO: Listening on port 3000
INFO: Server is running 5 workers X 6 threads with facil.io 0.7.4 (epoll)
* Detected capacity: 131056 open file limit
* Root pid: 101795
* Press ^C to stop

INFO: 101803 is running.
INFO: 101801 is running.
INFO: 101799 is running.
INFO: 101805 is running.
INFO: 101797 is running.
```

To learn more about using the [facil.io framework](http://facil.io), please read through the comments in the source code or the guides on the framework's website.

Good luck!

## Performance

### Internal web-server

Going to test with `WRK`:

```
for i in `seq 0 10`; do    wrk -c 400 -d 60s -t 8 http://127.0.0.1:3000 --latency > run-${i}.log; done
```

**Requests/sec: 440759.01**

**Transfer/sec:     95.00MB**

### Nginx
```
server {
	listen       81;
	server_name  localhost;
	location / {
		proxy_pass http://localhost:3000;
	}
}
```

Going to test with `WRK`:

```
for i in `seq 0 10`; do    wrk -c 400 -d 60s -t 8 http://127.0.0.1:81 --latency > run-${i}-nginx.log; done
```

**Requests/sec:  16087.84**

**Transfer/sec:      3.90MB**

### CaddyServer

The `Caddyfile` file:

```
:2015

reverse_proxy /* 127.0.0.1:3000
```

Going to test with `WRK`:

```
for i in `seq 0 10`; do    wrk -c 400 -d 60s -t 8 http://127.0.0.1:2015 --latency > run-${i}-caddy.log; done
```

**Requests/sec:  40090.88**

**Transfer/sec:      8.53MB**

### Nginx PHP-FPM (using port and not socket file)

#### Worker is 1

Test php-fpm with port in nginx (worker is 1):

The `/etc/nginx/nginx.conf` file:

```
worker_processes  1;
events {
    worker_connections  1024;
}
http {
	server {
		listen       80;
		server_name  localhost;
		root   /usr/share/nginx/html;
		location ~ \.php$ {
			fastcgi_pass   127.0.0.1:9000;
			fastcgi_index  index.php;
			fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
			include        fastcgi_params;
		}
	}
	....
}
```

```
Running 1m test @ http://localhost/test.php
  8 threads and 400 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    23.53ms    1.26ms 107.78ms   98.77%
    Req/Sec     2.14k   100.78     3.61k    74.56%
  Latency Distribution
     50%   23.45ms
     75%   23.72ms
     90%   23.96ms
     99%   24.84ms
  1020477 requests in 1.00m, 214.10MB read
Requests/sec:  16999.73
Transfer/sec:      3.57MB
```

**Requests/sec:  16999.73**

**Transfer/sec:      3.57MB**

#### Worker is `auto`

Test php-fpm with port in nginx (worker is auto):

The `/etc/nginx/nginx.conf` file:

```
worker_processes  auto;
events {
    worker_connections  1024;
}
http {
	server {
		listen       80;
		server_name  localhost;
		root   /usr/share/nginx/html;
		location ~ \.php$ {
			fastcgi_pass   127.0.0.1:9000;
			fastcgi_index  index.php;
			fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
			include        fastcgi_params;
		}
	}
	....
}
```

Going to test with `WRK`:

```
for i in `seq 0 10`; do    wrk -c 400 -d 60s -t 8 http://localhost/test.php --latency > run-${i}-nginx-phpfpm.log; done
```

**Requests/sec:  30119.64**

**Transfer/sec:      6.32MB**

### Nginx (pure html file)

**Requests/sec: 168234.33**

**Transfer/sec:    136.37MB**

---------

### What you're starting with

This folder contains the library code, some boilerplate application code, a complex (yet very helpful) `makefile` and some helper scripts.

### Temporary boilerplate application

The boilerplate code, which is a basic "Hello World!" HTTP application resides in the `src` folder.

If you wish to rename the folder, make sure to update the `MAIN_ROOT` in the  `makefile`.

It's also possible to use sub-folders using the `MAIN_SUBFOLDERS` variable in the `makefile` (i.e., add `foo/bar` to add the sub-folder `src/foo/bar`).

### The Library code

The facil.io library source code files reside in the `lib/facil` folder.

It's possible to remove any unused modules. For example, if you're not writing an HTTP application, it's safe to remove the `facil/http` folder and the source files it contains. If your application doesn't need Redis connectivity, it can be removed.

However, since code footprint is usually a minor concern (and can often be fixed by adding instructions to the compiler using the makefile), it is recommended that unused code be left alone in case it would be required at a later stage.

