# TODO
## Server
- POST method
	- `POST [path] http/1.1`
	- `200 OK / 201 Created`
- DELETE method
	- `DELETE [path] http/1.1`
	- `200 OK`
- File upload
- CGI

## ConfigParser
- Allowed methods
	- `allowed_methods [method]`
	- `405 Method Not Allowed`
- HTTP redirection
	- `return [code] [path]`
	- `301 Moved Permanently`
- Client max body size
	- `client_max_body_size [size]`
	- `413 Payload Too Large`


```cpp

while recv nothing and req_len = 0
	do nothing

if (header full done)
	check if req type is post
	check if content-length (handle normally)
	if have
		keep reading till content-length
	else
		header = done;
	
	else check if encoding-type = chunked
		handle diffrently

```