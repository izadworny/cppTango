#include <zmq.hpp>

#ifdef _TG_WINDOWS_
    #define __IS_UNUSED__(var) var
#else
    #define __IS_UNUSED__(var) var __attribute__((unused))
#endif

int main(int, char**)
{
  zmq::context_t c(1);
  zmq::socket_t s(c, ZMQ_REQ);
  void* __IS_UNUSED__(plain_socket) = s.handle();

  s.disconnect("some endpoint");

  zmq::message_t m, n;
  zmq::send_result_t __IS_UNUSED__(sr)= s.send(m, zmq::send_flags::none);
  zmq::recv_result_t __IS_UNUSED__(rr) = s.recv(m, zmq::recv_flags::none);

  m.copy(n);
  m.move(n);

  s.set(zmq::sockopt::linger, 0);
  int __IS_UNUSED__(val) = s.get(zmq::sockopt::linger);
  return 0;
}
