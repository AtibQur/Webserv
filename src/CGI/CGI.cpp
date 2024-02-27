#include "Client.hpp"

int Client::execute()
{
    std::string cgiScriptPath = "";
    if (!std::filesystem::exists("root" + getUri()))
    {
        Location clientLocation = m_server.getConf()->getLocation("/cgi-bin");
        std::vector<std::string> cgiPath = clientLocation.getCgi();
        if (!cgiPath.empty())
        {
            cgiScriptPath = "root/cgi-bin/" + cgiPath[0];
        }
    }
    else
        cgiScriptPath = "root/" + getUri();

    const char *pythonPath = "/usr/bin/python3";
    const char *pythonScript = const_cast<char *>(cgiScriptPath.c_str());

    std::string _queryString = "name=" + m_cgiBody;

    char *const argv[] = {const_cast<char *>(pythonPath), const_cast<char *>(_pytyhonScript.c_str()), const_cast<char *>(_queryString.c_str()), nullptr};
    char *const envp[] = {const_cast<char *>(_query.c_str()), NULL};

    // const char *errorLogPath = "docs/error.log";
    // int errorLogFd = open(errorLogPath, O_CREAT | O_RDWR | O_TRUNC, 0777);
    // if (errorLogFd < 0)
    // {
    //     std::cerr << "Error opening error log file" << std::endl;
    //     return 1;
    // }        

    pid_t pid = fork();
    if (pid == -1) {
        throw (std::invalid_argument("500 fork error"));
    }
    if (pid == CHILD)
    {
        if (close(m_cgiToServer.m_pipeFd[READ]) == -1)
            throw (std::invalid_argument("500 close read error"));

        std::cerr << "m_pipeFd[WRITE] " << m_cgiToServer.m_pipeFd[WRITE] << std::endl;
        if (dup2(m_cgiToServer.m_pipeFd[WRITE], STDOUT_FILENO) == -1) // Dup the write end of pipe2 to stdout
            throw (std::invalid_argument("500 dub2 error"));
        if (close(m_cgiToServer.m_pipeFd[WRITE]) == -1)
            throw (std::invalid_argument("500 close write error"));

        execve(pythonPath, argv, envp);
        std::cerr << "Error executing Python script." << std::endl;
        throw (std::invalid_argument("500 execve error"));
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);

        //? Read from the read end of the pipe and write to the error log to check if py script has errors
        /* char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(cgiToServerputPipe[1], buffer, sizeof(buffer))) > 0) {
            write(errorLogFd, buffer, bytesRead);
        }
        close(errorLogFd); */


        // dup2(og_in, STDIN_FILENO);
        // dup2(og_out, STDOUT_FILENO);
        // close(og_in);
        // close(og_out);
    }
    else
    {
        std::cerr << "Error forking process" << std::endl;
        // close(errorLogFd);
        return 1;
    }
    // std::ifstream errorLogFile(errorLogPath);
    // std::string errorLogContent((std::istreambuf_iterator<char>(errorLogFile)), (std::istreambuf_iterator<char>()));
    // errorLogFile.close();
    // if (!errorLogContent.empty())
    // {
    //     std::cerr << "Python script encountered errors" << std::endl;
    //     return (1);
    // }
    return 0;
}

void    Client::extractcgiUri() 
{
    // TODO substr PATH_INFO= van de slash to de question mark    /*

    std::string path = "root" + getUri();
    size_t py = path.find(".py");
    std::string script = path.substr(0, py);

    if (std::filesystem::exists(script + ".py"))
    {
        _pytyhonScript = script + ".py";
    }
    else
        throw std::invalid_argument("404 Not Found");
    std::string remaining = path.substr(py + 3);

    size_t questionMarkPos = remaining.find("?");
    if (questionMarkPos != std::string::npos)
    {
        _query = "QUERY_STRING=" + remaining.substr(questionMarkPos + 1);
    }
}

void Client::handleCGI()
{
    try {
        extractcgiUri();
        execute();
    }
    catch (std::exception &e)
    {
        throw (std::invalid_argument(e.what()));
    }
}

void Client::addCGIProcessToEpoll(Socket *ptr, int events, int fd)
{
    struct epoll_event event;
    event.events = events;
    event.data.ptr = ptr;

    if (epoll_ctl(this->m_epoll, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        throw (std::invalid_argument("500 cgi epoll Error"));
    }
}
