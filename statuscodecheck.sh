#!/bin/bash

# RED='\033[0;31m'
# GREEN='\033[0;32m'
# NC='\033[0m'

# Define the base URL of your web server
BASE_URL="http://localhost:8081"

cp image.jpg root

# Function to test a specific endpoint
test_endpoint() {
    local endpoint="$1"
    local url="$BASE_URL$endpoint"
    echo "Testing endpoint: $url"
    if [ $(curl -s -o /dev/null -w "%{http_code}\n" "$url") == $2 ]
    then
        echo "SUCCES: $(curl -s -o /dev/null -w "%{http_code}\n" "$url")"
    else
        echo "ERROR: $url returned status code" $(curl -s -o /dev/null -w "%{http_code}\n" "$url")
        exit 1
    fi
    echo
}

test_post_endpoint() {
    local endpoint="$1"
    local url="$BASE_URL$endpoint"
    local data="$2"
    echo "Testing POST endpoint: $url with data: $data"
    if [ $(curl -s -o /dev/null -w "%{http_code}\n" -X POST -d "$data" "$url") == $3 ]
    then
        echo "SUCCES: $(curl -s -o /dev/null -w "%{http_code}\n" -X POST -d "$data" "$url")"
    else
        echo "ERROR: $url returned status code" $(curl -s -o /dev/null -w "%{http_code}\n" -X POST -d "$data" "$url")
        exit 1
    fi
    echo
}

# Get tests
echo "GET TESTS"
echo

test_endpoint "/" 200

test_endpoint "/upload" 200

test_endpoint "/image.jpg" 200

test_endpoint "/non_existend" 404

test_endpoint "/non_existend.txt" 404

test_endpoint "/test" 405

test_endpoint "/teapot" 418

# Post tests

echo "POST TESTS"
echo

test_post_endpoint "/upload" "data=hey!" 200

test_post_endpoint "/upload" "" 200

test_post_endpoint "/non_existend" "hey!" 404

test_post_endpoint "/test" "hey!" 405

test_post_endpoint "/client_max_body_size_test" "dit zijn 9" 200

test_post_endpoint "/client_max_body_size_test" "dit zijn 10" 413

# Delete tests

echo "Testing complete."
