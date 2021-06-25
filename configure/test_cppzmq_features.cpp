#include <zmq.hpp>

int main(int, char**)
{
  zmq::context_t c(1);
  zmq::socket_t s(c, ZMQ_REQ);
  void* plain_socket = s.handle();

  s.disconnect("some endpoint");

  zmq::message_t m, n;
  zmq::send_result_t sr = s.send(m, zmq::send_flags::none);
  zmq::recv_result_t rr = s.recv(m, zmq::recv_flags::none);

  m.copy(n);
  m.move(n);

  s.set(zmq::sockopt::linger, 0);
  int val = s.get(zmq::sockopt::linger);
}
