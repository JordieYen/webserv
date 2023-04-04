import os
import sys

def	parse_body(body):
	if body:
		return (body[1][body[1].find('=') + 1:-2] + " = \"" + body[3][:-2] + "\"")
	else:
		return ("")

def	generate_page(body):
	with open("cgi.html") as cgi:
		for line in cgi:
			if line.startswith("\t\t\t\t<h5 class=\"cgi_header\">Request Method"):
				line = line[:45] + os.environ.get("REQUEST_METHOD") + "</h5>"
			if line.startswith("\t\t\t\t<h5 class=\"cgi_header\">Request Path"):
				line = line[:43] + os.environ.get("PATH_INFO") + "</h5>"
			if line.startswith("\t\t\t\t<h5 class=\"cgi_header\">Content-Type"):
				line = line[:43] + os.environ.get("CONTENT_TYPE") + "</h5>"
			if line.startswith("\t\t\t\t<h5 class=\"cgi_header\">Content-Length"):
				line = line[:45] + os.environ.get("CONTENT_LENGTH") + "</h5>"
			if line.startswith("\t\t\t\t<h5 class=\"cgi_header\">Request Body"):
				line = line[:43] + parse_body(body) + "</h5>"
			print(line, end = '')

if (os.environ.get("REQUEST_METHOD") == "GET"):
	generate_page([])
elif (os.environ.get("REQUEST_METHOD") == "POST"):
	body = []
	for line in sys.stdin:
		body.append(line)
	generate_page(body)
