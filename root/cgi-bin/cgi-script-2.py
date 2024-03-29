#!/usr/bin/env python

import os
print("<!DOCTYPE html>")

path_info = os.environ.get('PATH_INFO', '')
query_string = os.environ.get('QUERY_STRING', '')

print(f"Content-Type: text/plain<br>")
print(f"Query String: {query_string}<br>")
print(f"Path Info: {path_info}<br>")

while(1):
    print("hoi")