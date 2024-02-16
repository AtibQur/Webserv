#ifndef CGIIN_HPP
#define CGIIN_HPP

#include "main.hpp"

#define READ 0
#define WRITE 1

class CgiIn : public Socket {
public:
    int        m_pipeFd[2]{};

    CgiIn();
    ~CgiIn();

};

#endif