## Webserv

<img width="889" alt="WebservScreenshot" src="https://github.com/vivilenard/webserv/assets/78664024/9a65afb9-cd2b-4e9c-8897-9478a7eb0954">


This webserver can handle GET, POST and DELETE requests.
It is inspired by nginx.

## Build

It runs on MacOS and ArchLinux.
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






