import os
from datetime import datetime as dt

def	append_icons(full_path, is_window):
	icons = ""

	for file in os.listdir("public/" + full_path):
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
	with open("public/homepage/homepage.html") as homepage:
		for line in homepage:
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
	with open("public/homepage/login.html") as login:
		for line in login:
			print(line, end = '')

if (os.environ.get("USERNAME")):
	full_path = "users/" + os.environ.get("USERNAME")
	os.makedirs("public/" + full_path, exist_ok = True)
	generate_homepage(full_path)
else:
	generate_login()
