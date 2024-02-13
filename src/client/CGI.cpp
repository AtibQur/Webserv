#include "../../inc/main.hpp"

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

    int cgiInputPipe[2];
    int cgiOutputPipe[2];
    pipe(cgiInputPipe);
    pipe(cgiOutputPipe);

    // int pip[2];
    // if (pipe(pip) == -1) {
    //     std::cerr << "Error creating pipe for stderr redirection" << std::endl;
    //     close(errorLogFd);
    //     return 1;
    // }

    int pid = fork();
    if (pid == 0) { // Child

        // Redirect stderr pipe
        close(cgiInputPipe[1]);
        close(cgiOutputPipe[0]);
        
        dup2(cgiInputPipe[0], STDIN_FILENO);
        dup2(cgiOutputPipe[1], STDOUT_FILENO);

        // // Redirect stdout to tempfile
        // int fd = open("docs/tmpfile.html", O_CREAT | O_RDWR | O_TRUNC, 0777);
        // if (fd < 0) {
        //     std::cerr << "Error opening file" << std::endl;
        //     exit(EXIT_FAILURE);
        // }
        // dup2(fd, STDOUT_FILENO);
        // close(fd);

        execve(pythonPath, argv, envp);
        std::cerr << "Error executing Python script." << std::endl;
    } 
    else if (pid > 0) { // Parent

        // Read from the read end of the pipe and write to the error log
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(cgiOutputPipe[1], buffer, sizeof(buffer))) > 0) {
            write(errorLogFd, buffer, bytesRead);
        }
        close(cgiInputPipe[0]);
        close(cgiOutputPipe[1]);
        close(errorLogFd);

        // addCGIProcessToEpoll(cgiOutputPipe[0]);
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

    // substr executable file
    // substr PATH_INFO= van de slash to de question mark
    // substr QUERY_STRING= vanaf de question mark
    // cgi start

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

void Client::addCGIProcessToEpoll(int cgiOutputPipe) {
    struct epoll_event event;
    event.events = EPOLLIN;
    Socket *ptr;
    event.data.ptr = this; //! not sure about this

    if (epoll_ctl(this->m_epoll, EPOLL_CTL_ADD, cgiOutputPipe, &event) == -1) {
        perror("epoll_ctl cgi_output_pipe");
        exit(EXIT_FAILURE);
    }
    else
        std::cerr << "add to epoll" << std::endl;
}