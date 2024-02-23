#!/usr/bin/env python3

# Print the CGI Content-Type header
print("Content-Type: text/html\n")

# Print the HTML content
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Simple CGI Script</title>")
print("</head>")
print("<body>")
print("<h1>Simple CGI Script in Python</h1>")
print("<p>This is a simple CGI script written in Python.</p>")

# Retrieve form data
import cgi
form = cgi.FieldStorage()

# Check if the form data contains a 'name' field
if "name" in form:
    # Retrieve the value of the 'name' field
    name = form["name"].value
    # Print the name back to the user
    print("<p>Hello, {}!</p>".format(name))
else:
    print("<p>No name provided.</p>")

print("</body>")
print("</html>")
