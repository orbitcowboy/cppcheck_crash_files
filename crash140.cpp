p_param(ParamOptions*
        int gboolean; typedef struct { gboolean names typedef 1; gboolean types
                                       : 1;
                                     } ParamOptions; int : o)
{
    return o->types &&
           o->names;
}