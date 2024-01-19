#include "../../inc/main.hpp"

int Client::execute() {
    const char* pythonPath = "/usr/bin/python3";
    const char* pythonScript = "cgi-bin/cgi-script.py";

    std::string queryString = "name=" + m_name;

    char *const argv[] = {const_cast<char*>(pythonPath), const_cast<char*>(pythonScript), const_cast<char*>(queryString.c_str()), nullptr};
    char *const envp[] = {nullptr};

    if (execve(pythonPath, argv, envp) == -1) {
        std::cerr << "Error executing Python script." << std::endl;
        return (1);
    }
    return (0);
}

int Client::createCGI() {
    const std::string tmpFile = "docs/tmpfile.html";
    char *fileName = const_cast<char *>(tmpFile.c_str());
    int fd = open (fileName, O_CREAT | O_RDWR | O_TRUNC, 0777);
    if (fd < 0)
    {
        perror("cgi error");
        return (1);
    }
    int pid = fork();
    if (pid == 0)
    {
        if (dup2(fd, STDOUT_FILENO) < 0){
            perror ("dub error");
            return (1);
        }
        if (execute())
            return (1);
    }
    close(fd);
    waitpid(pid, NULL, WUNTRACED);
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
        clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    }
    htmlFile.close();

    clientResponse.sendResponse();
    std::cout << "CGI response send" << std::endl;
    return (0);
}

// change void to int for throw, then check if it works