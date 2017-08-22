#include "request.h"
#include "responder.h"
#include "connection.h"
#include <iostream>

using namespace fastcgi;

request::request(connection * writer, unsigned char t, unsigned int reqID, responder * res)
        : io(writer), type(t), requestId(reqID), handler(res)
{

}

int request::control(unsigned char *buffer, unsigned int bufferSize)
{
    contentData = buffer;
    contentLength = bufferSize;
    switch (type) {
        case 1:
            beginRequest();
            break;
        case 2:
            abortRequest();
            break;
        case 4:
            params();
            break;
        case 5:
            stdIn();
            break;
        case 8:
            data();
            break;
    }
    if (shouldClose)
        return -1; // in case of the need for closing connection!
    else
        return 0;
}

void request::beginRequest()
{
    std::cout << "---contentData is beginRequest" << std::endl;
    role = (contentData[0] << 8) + contentData[1];
    flags = contentData[2];
    delete [] contentData;
}

void request::abortRequest()
{

    std::cout << "---contentData is abortRequest" << std::endl;
    delete [] contentData;
}

void request::params()
{

    std::cout << "---contentData is params" << std::endl;
    delete [] contentData;
}

void request::stdIn()
{
    std::cout << "---contentData is stdIn" << std::endl;
    unsigned char test[196] =
            {
                    0x01, 0x06, 0x00, 0x01, 0x00, 0xa1, 0x03, 0x00,
                    0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d,
                    0x54, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65,
                    0x78, 0x74, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0x0a,
                    0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d,
                    0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x3a, 0x20,
                    0x31, 0x31, 0x36, 0x0a, 0x0a, 0x3c, 0x21, 0x44,
                    0x4f, 0x43, 0x54, 0x59, 0x50, 0x45, 0x20, 0x68,
                    0x74, 0x6d, 0x6c, 0x3e, 0x0a, 0x3c, 0x68, 0x74,
                    0x6d, 0x6c, 0x3e, 0x0a, 0x09, 0x3c, 0x68, 0x65,
                    0x61, 0x64, 0x3e, 0x0a, 0x09, 0x09, 0x3c, 0x74,
                    0x69, 0x74, 0x6c, 0x65, 0x3e, 0x57, 0x65, 0x6c,
                    0x6c, 0x63, 0x6f, 0x6d, 0x65, 0x21, 0x3c, 0x2f,
                    0x74, 0x69, 0x74, 0x6c, 0x65, 0x3e, 0x0a, 0x09,
                    0x3c, 0x2f, 0x68, 0x65, 0x61, 0x64, 0x3e, 0x0a,
                    0x09, 0x3c, 0x62, 0x6f, 0x64, 0x79, 0x3e, 0x0a,
                    0x09, 0x09, 0x3c, 0x68, 0x31, 0x3e, 0x48, 0x65,
                    0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c,
                    0x64, 0x21, 0x3c, 0x2f, 0x68, 0x31, 0x3e, 0x0a,
                    0x09, 0x3c, 0x2f, 0x62, 0x6f, 0x64, 0x79, 0x3e,
                    0x0a, 0x3c, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0x3e,
                    0x0a, 0x00, 0x00, 0x00, 0x01, 0x06, 0x00, 0x01,
                    0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x01,
                    0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00
            };
    io->write(test, 196);
    shouldClose = true;
    delete [] contentData;
}

void request::data()
{
    std::cout << "---contentData is data" << std::endl;

    delete [] contentData;
}

unsigned int request::getRequestId()
{
    return requestId;
}

void request::setType(unsigned char t)
{
    type = t;
}

