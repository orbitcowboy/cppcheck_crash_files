class TRH {
public:
    TRH();
    int getET() const;
};

void calc() {
        TRH trh;
        int zv = trh.getET();
        int zz = trh.getET();
}
// from https://trac.cppcheck.net/ticket/8624
