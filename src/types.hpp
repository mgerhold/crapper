#pragma once

#ifdef _WIN32
using CurlOption = int;
using CurlSocket = std::uintptr_t;
using CurlSocketType = int;
using CurlErrorCode = int;
#else
using CurlOption = unsigned int;
using CurlSocket = int;
using CurlSocketType = unsigned int;
using CurlErrorCode = unsigned int;
#endif
