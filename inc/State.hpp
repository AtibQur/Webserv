#pragma once

#include "main.hpp"

enum State {
    CGI_Start,
    CGI_Read,
    CGI_Write,
};