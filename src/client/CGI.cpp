#include "../../inc/main.hpp"

// int Client::execute() {
//     const char* pythonPath = "/usr/bin/python3";
//     const char* pythonScript = "cgi-bin/cgi-script-2.py";  // should come from config file

//     std::string queryString = "name=" + m_name;

//     char *const argv[] = {const_cast<char*>(pythonPath), const_cast<char*>(pythonScript), const_cast<char*>(queryString.c_str()), nullptr};
//     char *const envp[] = {nullptr};

//     if (execve(pythonPath, argv, envp) == -1) {
//         std::cerr << "Error executing Python script." << std::endl;
//         return (1);
//     }    
//     return (0);
// }

int Client::execute() {
    const char* pythonPath = "/usr/bin/python3";
    const char* pythonScript = "cgi-bin/cgi-script-2.py";  // should come from config file

    std::string queryString = "name=" + m_name;
    
    char *const argv[] = {const_cast<char*>(pythonPath), const_cast<char*>(pythonScript), const_cast<char*>(queryString.c_str()), nullptr};
    char *const envp[] = {nullptr};

    const char* errorLogPath = "docs/error.log";
    int errorLogFd = open(errorLogPath, O_CREAT | O_RDWR | O_TRUNC, 0777);
    if (errorLogFd < 0) {
        std::cerr << "Error opening error log file" << std::endl;
        return 1;
    }

    int pip[2];
    if (pipe(pip) == -1) {
        std::cerr << "Error creating pipe for stderr redirection" << std::endl;
        close(errorLogFd);
        return 1;
    }

    int pid = fork();
    if (pid == 0) { // Child

        // Redirect stderr pipe
        dup2(pip[1], STDERR_FILENO);
        close(pip[0]);
        close(pip[1]);
        // Redirect stdout to tempfile
        int fd = open("docs/tmpfile.html", O_CREAT | O_RDWR | O_TRUNC, 0777);
        if (fd < 0) {
            std::cerr << "Error opening file" << std::endl;
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);

        execve(pythonPath, argv, envp);
        std::cerr << "Error executing Python script." << std::endl;
    } 
    else if (pid > 0) { // Parent
        close(pip[1]);

        // Read from the read end of the pipe and write to the error log
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(pip[0], buffer, sizeof(buffer))) > 0) {
            write(errorLogFd, buffer, bytesRead);
        }

        // Close the pipe and error log file
        close(pip[0]);
        close(errorLogFd);

        // Wait for the child process to finish
        waitpid(pid, NULL, 0);
    } else {
        std::cerr << "Error forking process" << std::endl;
        close(pip[0]);
        close(pip[1]);
        close(errorLogFd);
        return 1;
    }

    std::ifstream errorLogFile(errorLogPath);
    std::string errorLogContent((std::istreambuf_iterator<char>(errorLogFile)), (std::istreambuf_iterator<char>()));
    errorLogFile.close();

    if (!errorLogContent.empty()) {
        std::cerr << "Python script encountered errors" << std::endl;
        return (1);
    }
    return 0;
}


int Client::createCGI() {
    // execute();
    // const std::string tmpFile = "docs/tmpfile.html";
    // char *fileName = const_cast<char *>(tmpFile.c_str());
    // int fd = open (fileName, O_CREAT | O_RDWR | O_TRUNC, 0777);
    // if (fd < 0)
    // {
    //     std::cerr << "Error opening file" << std::endl;
    //     return (1);
    // }
    // int pid = fork();
    // if (pid == 0)
    // {
    //     if (dup2(fd, STDOUT_FILENO) < 0){
    //         std::cerr << "Dub2 Error" << std::endl;
    //         close(fd);
    //         return (1);
        // }
    if (execute()) {
        return (1);
    }
    // }
    // close(fd);
    // waitpid(pid, NULL, WUNTRACED);
    return (0);
}


int Client::handleCGI() {
    if (createCGI()) 
        return (1);

    Response clientResponse(m_socketFd, "200 OK");

    std::string filePath = "docs/tmpfile.html";
    std::ifstream htmlFile(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    if (!htmlFile.is_open()) {
        clientResponse.setContent("14\n\nFile not found");
    } else {
        std::string contentType = "text/plain";
        clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    }
    htmlFile.close();

    clientResponse.sendResponse();
    std::cout << "CGI response send" << std::endl;
    std::remove(filePath.c_str());

    return (0);
}

// change void to int for throw, then check if it works