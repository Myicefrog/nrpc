#include <common.h>
#include <info_log_context.h>
#include "channel.h"
#include "controller.h"

#include "echo.pb.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: ./client <IP>\n");
        return -1;
    }
    // for test
    std::string error_log_file("client_error.log");
    ngxplus::InfoLogContext::set_log_file(error_log_file);
    //common::COMMON_TEST_FLAG = 1;

    nrpc::Channel channel;
    nrpc::ChannelOption option;
    if (!channel.init(argv[1], 8899, &option)) {
        return -1;
    }
    nrpc::EchoService_Stub stub(&channel);

    // the three element of RPC
    nrpc::Controller cntl;
    nrpc::EchoRequest req;
    nrpc::EchoResponse resp;
    nrpc::Student student;
    req.set_msg("hello client");

/*      this will call _channel.CallMethod(methoddes, cntl, req, resp, NULL)
 *        1.pack request
 *        2.send request buf
 *        3.recv response
 *        4.parse response
 */

    // 1.rpc call successly
    stub.Echo(&cntl, &req, &resp, NULL);
    if (cntl.Failed()) {
        LOG(ALERT, "rpc failed: %s", cntl.ErrorText().c_str());
        return -1;
    }
    std::cout << resp.res() << std::endl;

    // 2.rpc call failed
    stub.Reflect(&cntl, &req, &student, NULL);
    if (cntl.Failed()) {
        LOG(ALERT, "rpc failed: %s", cntl.ErrorText().c_str());
    }
    std::cout << student.name() << std::endl;

    return 0;
}
