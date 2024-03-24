#!/usr/bin/env python

import sys
import os

# Parse POST data
form = os.getenv('POST_DATA')

# Print HTTP headers
print("Content-Type: text/html")
print("")

# Start HTML response
print("<html>")
print("<head>")
print("<title>Python CGI Script</title>")
print("<link rel=\"stylesheet\" type=\"text/css\" href=\"/cgi-bin/css/post.css\">")
print("</head>")
print("<body>")
print("<div class=\"container\">")
print("<img src=\"/cgi-bin/img/indicating.jpg\" alt=\"hand\" class=\"hand left-hand\">")
print("<div class=\"centered-text\">")
print("<h1>Python CGI Script Output</h1>")
print("<p>POST data:</p>")
print("<pre>{}</pre>".format(str(form)))
print("</div>")
print("<img src=\"/cgi-bin/img/indicating.jpg\" alt=\"hand\" class=\"hand right-hand\">")
print("</div>")
print("</body>")
print("</html>")
