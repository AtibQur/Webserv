#!/usr/bin/env python

print("<!DOCTYPE html>")
print("<html lang=""en"">")
print("<head>")
print("\t<title>CGI Script</title>")
print("<style>\nbody {\n\ttext-align: center;\n}")
print("</style>")
print("</head><body>")

print("<form action=\"/cgi-bin/cgi-script.py\" method=\"post\" enctype=\"text/plain\">")
print("\t<label for=\"text\">Enter your name</label>")
print("\t<input type=\"text\" id=\"text\" name=\"post\">")
print("\t<input type=\"submit\">")
print("</form>")


print("<body>")
import cgi
form = cgi.FieldStorage()
name = form.getvalue("name")

if name:
    message = f"Hello, {name.upper()}!"
else:
    message = "Please enter your name."

print("\t<h1>" + message + "</h1>")

print("</body>")
print("</html>")
