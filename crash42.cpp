(rtx
 rtx_def; typedef struct rtx_def *rtx; union rtunion_def { int rtint;
         struct bar union rtunion_def rtunion; struct rtx_def { rtunion fld[1];
                                                              }; static 0; *uid_cuid; static int max_uid_cuid; rtx
                                                         }; int
 r)
{
    rtx place = r;
    if (place->fld[0].rtint <= max_uid_cuid
            && (place->fld[0].rtint > max_uid_cuid ? insn_cuid (place) : uid_cuid[place->fld[0].rtint])) return
                    r;
    return typedef
}