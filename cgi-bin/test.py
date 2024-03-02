#!/usr/bin/python

import os

# Print HTTP headers
print("Content-Type: text/html\n")

# Extract query string from environment variables
query_string = os.environ.get("QUERY_STRING", "")

# Parse query parameters
params = {}
if query_string:
    pairs = query_string.split("&")
    for pair in pairs:
        key, value = pair.split("=")
        params[key] = value

# Display the query parameters
print("<html>")
print("<head>")
print("<title>GET Method Example</title>")
print("</head>")
print("<link rel=\"stylesheet\" type=\"text/css\" href=\"../css/style.css\">")
print("<body>")
print("<h1>GET Method Example</h1>")
print("<p>First Name: {}</p>".format(params.get("first_name", "")))
print("<p>Last Name: {}</p>".format(params.get("last_name", "")))
print("<p>This is an example of a CGI script serving an image:</p>")
print("<img src=\"../img/test.jpg\" alt=\"Your Image\">")
print("</body>")
print("</html>")
