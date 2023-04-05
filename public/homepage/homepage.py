import os
import sys
from datetime import datetime as dt

def	append_icons(full_path, is_window):
	icons = ""

	for file in os.listdir(full_path):
		if is_window:
			icons += "\t\t\t\t\t<div class=\"icon\">\n"
			icons += "\t\t\t\t\t\t<input type=\"checkbox\" id=_" + file + " onclick=\"toggle_trash(this)\"/>\n"
			icons += "\t\t\t\t\t\t<label for=_" + file + " style=\"background-image: url('" + full_path + "/" + file + "');\"></label>\n"
			icons += "\t\t\t\t\t\t<div class=\"finder_label\">\n"
			icons += "\t\t\t\t\t\t\t<label onclick=\"redirect_to_file(this)\">" + file + "</label>\n"
			icons += "\t\t\t\t\t\t</div>\n"
			icons += "\t\t\t\t\t</div>\n"
		else:
			icons += "\t\t\t\t<div class=\"icon\">\n"
			icons += "\t\t\t\t\t<input type=\"checkbox\" id=" + file + " onclick=\"toggle_trash(this)\"/>\n"
			icons += "\t\t\t\t\t<label for=" + file + " style=\"background-image: url('" + full_path + "/" + file + "');\"></label>\n"
			icons += "\t\t\t\t\t<div>\n"
			icons += "\t\t\t\t\t\t<label onclick=\"redirect_to_file(this)\">" + file + "</label>\n"
			icons += "\t\t\t\t\t</div>\n"
			icons += "\t\t\t\t</div>\n"
	return (icons)

def generate_homepage(full_path):
	with open("homepage.html") as homepage:
		for line in homepage:
			if line.startswith("\t\t\t\tbackground-image: url('eulee_background.jpg');") and os.environ.get("USERNAME") == "jking-ye":
				line = "\t\t\t\tbackground-image: url('marin_black.png');"
			if line.startswith("\t\t\t\twindow.location.href"):
				line = line[:28] + "/" + full_path + line[28:]
			if line.startswith("\t\t\t\t\tfetch('/' + file"):
				line = line[:12] + "/" + full_path + line[12:]
			if line.startswith("\t\t\t\t<p class=\"header_title\">"):
				line = line[:-5] + os.environ.get("USERNAME") + "</p>\n"
			if line.startswith("\t\t\t\t<p class=\"header_time\">"):
				line = line[:-5] + dt.now().strftime("%a %H:%M") + "</p>\n"
			if line.startswith("\t\t\t<div class=\"table\">"):
				line += append_icons(full_path, False)
			if line.startswith("\t\t\t\t<div class=\"table\" id=\"window_table\">"):
				line += append_icons(full_path, True)
			print(line, end = '')

def	generate_login():
	with open("login.html") as login:
		for line in login:
			print(line, end = '')

def	save_file(full_path, body, boundary):
	header_done = False
	for line in body:
		if "filename" in line:
			name = line[line.rfind('=') + 2:-3]
		if header_done:
			if ("--" + boundary + "--") in line:
				new_file.close()
			elif boundary not in line:
				new_file.write(line.encode("utf-8", "surrogateescape"))
		elif line == "\r\n":
			new_file = open(full_path + "/" + name, 'wb')
			header_done = True
	

full_path = "../users/" + os.environ.get("USERNAME")
os.makedirs(full_path, exist_ok = True)

if (os.environ.get("REQUEST_METHOD") == "GET"):
	if (os.environ.get("USERNAME")):
		generate_homepage(full_path)
	else:
		generate_login()
elif (os.environ.get("REQUEST_METHOD") == "POST"):
	body = []
	for line in sys.stdin:
		body.append(line)
	if (os.environ.get("CONTENT_TYPE") == "multipart/form-data"):
		save_file(full_path, body, os.environ.get("FORM_BOUNDARY"))