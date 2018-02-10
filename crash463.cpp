struct ServerEntry {
    std::string address;
    unsigned httpPort;
    unsigned httpsPort;
};
std::string address;
unsigned httpPort  = 42;
unsigned httpsPort = 42;
ServerEntry foo() {
   ServerEntry e = { std::move(address),static_cast<unsigned>(httpPort), static_cast<unsigned>(httpsPort) };
   return e;
}
// from https://trac.cppcheck.net/ticket/8393
