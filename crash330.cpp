void compute(int eflag, int vflag) {
    if (force->newton_pair) return eval<0,0,1>();
}

template < int EVFLAG, int EFLAG, int NEWTON_PAIR >
void PairLJCharmmCoulLongOpt::eval() {
    if (NEWTON_PAIR || j < nlocal) {
    }
}
