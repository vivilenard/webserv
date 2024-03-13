#!/usr/bin/env python

import sys
import os
# import cgi

# Parse POST data
form = os.getenv('POST_DATA')

# Print HTTP headers
print("Content-Type: text/html")
print("")

# Start HTML response
print("<html>")
print("<head><title>Python CGI Script</title></head>")
print("<body>")
print("<h1>Python CGI Script Output</h1>")
print("<p>POST data:</p>")
print("<pre>{}</pre>".format(str(form)))

# Check if post_param exists in form data
# if 'post_param' in form:
#     sys.stderr.write("Received post_param value: {}\n".format(form))  # Print to stderr
#     print("<p>Value of post_param:</p>")
#     print("<pre>{}</pre>".format(form))
# else:
#     sys.stderr.write("post_param not found in form data\n")  # Print to stderr
#     print("<p>post_param not found in form data</p>")

print("</body>")
print("</html>")

