struct ServerEntry {
    std::string address;
    unsigned httpPort;
    unsigned httpsPort;
};
std::string address;
unsigned httpPort = INTERNET_DEFAULT_HTTP_PORT;
unsigned httpsPort = INTERNET_DEFAULT_HTTPS_PORT;
int main() {
   ServerEntry entry = { std::move(address),static_cast<unsigned>(httpPort), static_cast<unsigned>(httpsPort) };
}
// from https://github.com/danmar/cppcheck/pull/1058#issuecomment-360896459
