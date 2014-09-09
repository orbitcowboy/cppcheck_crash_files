namespace foo
{

const cl_idecoded_float (integer_decode_float) (const cl_DF& x)
{

    return cl_idecoded_float(
               Q_to_I(mant), // Mantisse (>0, <2^53) als Bignum
               L_to_FN(exp-(DF_mant_len+1)), // e-53 als Fixnum
               (sign>=0 ? cl_I(1) : cl_I(-1)) // (-1)^s erzeugen
           );

}
}
