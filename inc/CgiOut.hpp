#ifndef CGIOUT_HPP
#define CGIOUT_HPP

#include "main.hpp"

#define READ 0
#define WRITE 1

class CgiOut : public Socket {
private:
public:
    int m_pipeFd[2]{};
    CgiOut();
    ~CgiOut();
};

#endif