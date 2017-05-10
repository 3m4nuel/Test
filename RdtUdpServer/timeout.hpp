#ifndef TIMEOUT_HPP_INCLUDED
#define TIMEOUT_HPP_INCLUDED

#include <netinet/in.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include "rdt.hpp"

int callTimeout(int filedes, unsigned int seconds);

#endif // TOUT_H_INCLUDED
