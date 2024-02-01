#!/usr/bin/env python

import os
print("<!DOCTYPE html>")

# Get the full path from PATH_INFO
path_info = os.environ.get('PATH_INFO', '')

request_method = os.environ.get('REQUEST_METHOD', '')
query_string = os.environ.get('QUERY_STRING', '')

print(f"Content-Type: text/plain\n")
print(f"Request Method: {request_method}")
print(f"Query String: {query_string}")

# Split the path into components
path_components = path_info.split('/')

# Extract relevant information
if len(path_components) >= 3 and path_components[1] == 'show':
    item_id = path_components[2]
    print(f"Content-type: text/html\n\n")
    print(f"Showing details for item with ID: {item_id}")
else:
    print(f"Content-type: text/html\n\n")
    print("Invalid request")
