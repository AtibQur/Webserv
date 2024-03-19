#include "Client.hpp"

pid_t child_pid;

void alarmHandler(int signal)
{
	(void)signal;
	if (child_pid > 0)
	{
		kill(child_pid, SIGKILL);
	}
}

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

    std::string _queryString = "name=" + m_cgiBody;

    char *const argv[] = {const_cast<char *>(pythonPath), const_cast<char *>(_pytyhonScript.c_str()), const_cast<char *>(_queryString.c_str()), nullptr};
    char *const envp[] = {const_cast<char *>(_query.c_str()), const_cast<char *>(_pathInfo.c_str()), NULL};

    pid_t pid = fork();
    if (pid == -1) {
        throw (std::invalid_argument("500 fork error"));
    }
    if (pid == CHILD)
    {

		signal(SIGALRM, alarmHandler);
		alarm(1);
		child_pid = getpid();
        if (close(m_cgiToServer.m_pipeFd[READ]) == -1)
            throw (std::invalid_argument("500"));
        if (dup2(m_cgiToServer.m_pipeFd[WRITE], STDOUT_FILENO) == -1) // Dup the write end of pipe2 to stdout
            throw (std::invalid_argument("500"));
        if (close(m_cgiToServer.m_pipeFd[WRITE]) == -1)
            throw (std::invalid_argument("500"));

        execve(pythonPath, argv, envp);
		alarm(0);
        std::cerr << "Error executing python script" << std::endl;
        throw (std::invalid_argument("500"));
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			throw (std::invalid_argument("500"));
        }
    }
    else
    {
        std::cerr << "Error forking process" << std::endl;;
        return 1;
    }
    return 0;
}

void    Client::extractcgiUri() 
{

    std::string path = "root" + getUri();
    size_t py = path.find(".py");
    std::string script = path.substr(0, py);

    if (std::filesystem::exists(script + ".py"))
    {
        _pytyhonScript = script + ".py";
    }
    else
        throw std::invalid_argument("404");
    std::string remaining = path.substr(py + 3);

    // TODO substr PATH_INFO= van de slash to de question mark /*
	size_t questionMarkPos = remaining.find("?");
	if (remaining[0] == '/'){
		_pathInfo = "PATH_INFO=" + remaining.substr(0, questionMarkPos);
	}
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
