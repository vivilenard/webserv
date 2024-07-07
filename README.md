## Webserv

This webserver can handle GET, POST and DELETE requests.
It is inspired by nginx.

## Build

This webserver runs on MacOS and ArchLinux.
```bash
git clone git@github.com:vivilenard/webserv.git
```

```bash
cd Webserv && make
```

Type ```http:localhost:8000``` in your favorite browser and you will see our default index page.

To change the port or other customisations, feel free to make changes in the config file.

If you choose to upload something, make sure it's less than 45KB.

## Which file types are supported for upload?

All supported types are listed in the config/mime.types file.






