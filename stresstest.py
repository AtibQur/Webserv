import requests
import concurrent.futures
import time

# Define the target URL
url = "http://localhost:8080"  # Change this to your web server's URL

# Define the number of concurrent requests and the total number of requests
concurrent_requests = 100
total_requests = 1000

# Lists to store response times
response_times = []

# Function to send HTTP requests and measure response time
def send_request(request_number):
    start_time = time.time()
    try:
        response = requests.get(url)
        response_time = time.time() - start_time
        response_times.append(response_time)
        print(f"Request {request_number}: Status Code {response.status_code}, Response Time: {response_time:.2f} seconds")
    except Exception as e:
        print(f"Request {request_number}: Error - {str(e)}")

# Create a pool of workers for concurrent requests
with concurrent.futures.ThreadPoolExecutor(max_workers=concurrent_requests) as executor:
    # Submit the requests to the executor
    futures = [executor.submit(send_request, i) for i in range(total_requests)]

# Wait for all requests to complete
concurrent.futures.wait(futures)

# Calculate and print performance metrics
if response_times:
    average_response_time = sum(response_times) / len(response_times)
    response_times.sort()
    percentile_95 = response_times[int(0.95 * len(response_times))]
    throughput = total_requests / sum(response_times)

    print(f"Average Response Time: {average_response_time:.2f} seconds")
    print(f"95th Percentile Response Time: {percentile_95:.2f} seconds")
    print(f"Throughput (requests per second): {throughput:.2f}")
