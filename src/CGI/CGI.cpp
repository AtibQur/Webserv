#include "Client.hpp"

int Client::execute() {
    std::string cgiScriptPath = "";
    if (!std::filesystem::exists("root" + getUri()))
    {
        Location clientLocation = m_server.getConf()->getLocation("/cgi-bin");
        std::vector<std::string> cgiPath = clientLocation.getCgi();
        if (!cgiPath.empty()) {
            std::cout << "getCgi " << cgiPath[0] << std::endl;
            cgiScriptPath = "root/cgi-bin/" + cgiPath[0];
        }
    }
    else
        cgiScriptPath = "root/" + getUri();

    const char* pythonPath = "/usr/bin/python3";
    const char* pythonScript = const_cast<char*>(cgiScriptPath.c_str());

    std::string _queryString = "name=" + m_name;

    char *const argv[] = {const_cast<char*>(pythonPath), const_cast<char*>(_pytyhonScript.c_str()), const_cast<char*>(_queryString.c_str()), nullptr};
    char *const envp[] = {const_cast<char*>(_query.c_str()), NULL};

    const char* errorLogPath = "docs/error.log";
    int errorLogFd = open(errorLogPath, O_CREAT | O_RDWR | O_TRUNC, 0777);
    if (errorLogFd < 0) {
        std::cerr << "Error opening error log file" << std::endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) { // Child

        if (close(m_cgiToServer.m_pipeFd[READ]) == -1)
            perror ("500 close read pipe");
        if (dup2(m_cgiToServer.m_pipeFd[WRITE], STDOUT_FILENO) == -1) // Dup the write end of pipe2 to stdout
            perror ("500 dub2");
        if (close(m_cgiToServer.m_pipeFd[WRITE]) == -1)
            perror ("500 close write pipe");

        execve(pythonPath, argv, envp);
        std::cerr << "Error executing Python script." << std::endl;
    } 
    else if (pid > 0) { // Parent
        close(m_cgiToServer.m_pipeFd[WRITE]);
        std::cout << "read end is: " << m_cgiToServer.m_pipeFd[READ] << std::endl;
    
        // Read from the read end of the pipe and write to the error log
        // char buffer[4096];
        // ssize_t bytesRead;
        // while ((bytesRead = read(cgiToServerputPipe[1], buffer, sizeof(buffer))) > 0) {
        //     write(errorLogFd, buffer, bytesRead);
        // }
        // close(serverToCgiputPipe[0]);
        // close(cgiToServerputPipe[1]);
        // close(errorLogFd);
    }
    else 
    {
        std::cerr << "Error forking process" << std::endl;
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

int Client::handleCGI() {

    // substr executable file
    // substr PATH_INFO= van de slash to de question mark
    // substr QUERY_STRING= vanaf de question mark
    // cgi start 

    //? make pipein class and is called in handle write. it added to epollIN and executed
    //? make pipeout class and is called in handle read. read the pipe bytes and put that in m_response
    //? then remove read piperead from epoll

    std::string path = "root" + getUri();
    size_t py = path.find(".py");
    std::string script = path.substr(0, py);

    if (std::filesystem::exists(script + ".py")){
        _pytyhonScript = script + ".py";
    }
    else
        return 2;
    std::string remaining = path.substr(py + 3);

    size_t questionMarkPos = remaining.find("?");
    if (questionMarkPos != std::string::npos)
    {
        _query = "QUERY_STRING=" + remaining.substr(questionMarkPos + 1);
    }

    if (execute()) {
        return (1);
    }

    // Response clientResponse(m_socketFd, "200 OK");

    // std::string filePath = "docs/tmpfile.html";
    // std::ifstream htmlFile(filePath);
    // std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    // if (!htmlFile.is_open()) {
    //     clientResponse.setContent("14\n\nFile not found");
    // } else {
    //     std::string contentType = "text/plain";
    //     clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    // }
    // htmlFile.close();

    // clientResponse.sendResponse();
    // std::cout << "CGI response send" << std::endl;
    // std::remove(filePath.c_str());

    return (0);
}

void Client:: addCGIProcessToEpoll(Socket *ptr, int events, int fd) {
    struct epoll_event event;
    event.events = events;
    event.data.ptr = ptr;

    if (epoll_ctl(this->m_epoll, EPOLL_CTL_ADD, fd, &event) == -1) {
        perror("epoll_ctl cgi_output_pipe");
        // exit(EXIT_FAILURE);
    }
    else
        std::cerr << "add cgi pipe to epoll" << std::endl;
}
