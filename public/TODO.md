# TODO
## Server
- ~~file upload directory (DECIDED NOT TO DO)~~
- ~~`Transfer-Encoding: chunked`~~
- ~~2nd CGI~~
	- ~~Display env variables~~
	- ~~Display form data from POST~~
- ~~CGI with POST (homepage file upload)~~
- ~~recv/send set to received/sent on error~~
	- ~~check for return value <= 0~~

## Testing
- ~~siege~~
- ~~memory leak~~
- ~~hanging connections~~

### ~~Unsure / Need to ask~~
- ~~server hostname (NO NEED TO DO)~~
- ~~CGI run in correct directory for relative path access~~
- ~~CGI files with error~~

## Done
- ~~POST method~~
	- ~~`POST [path] http/1.1`~~
	- ~~`200 OK / 201 Created`~~
- ~~DELETE method~~
	- ~~`DELETE [path] http/1.1`~~
	- ~~`200 OK`~~
- ~~File upload~~
- ~~CGI~~
- ~~Allowed methods~~
	- ~~`allowed_methods [method]`~~
	- ~~`405 Method Not Allowed`~~
- ~~HTTP redirection~~
	- ~~`return [code] [path]`~~
	- ~~`301 Moved Permanently`~~
- ~~Client max body size~~
	- ~~`client_max_body_size [size]`~~
	- ~~`413 Payload Too Large`~~
