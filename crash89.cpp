// librpc/src/Getrpcen.c
/**********************************************************************
 * ONC RPC for WIN32.
 * 1997 by WD Klotz
 * ESRF, BP 220, F-38640 Grenoble, CEDEX
 * klotz-tech@esrf.fr
 *
 * SUN's ONC RPC for Windows NT and Windows 95. Ammended port from
 * Martin F.Gergeleit's distribution. This version has been modified
 * and cleaned, such as to be compatible with Windows NT and Windows 95.
 * Compiler: MSVC++ version 4.2 and 5.0.
 *
 * Users may use, copy or modify Sun RPC for the Windows NT Operating
 * System according to the Sun copyright below.
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 **********************************************************************/
/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 * Users may use, copy or modify Sun RPC for the Windows NT Operating
 * System according to the Sun copyright below.
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/

/* @(#)getrpcent.c  2.2 88/07/29 4.0 RPCSRC */
static char RPCDB[1024];
struct rpcent * getrpcent()
{
    char *key = NULL, *val = NULL;
    register struct rpcdata *d = _rpcdata();

    if (d->rpcf == NULL && (d->rpcf = fopen(RPCDB, "r")) == NULL)
        return (NULL);
    return interpret(d->line, strlen(d->line));
}

static struct rpcent *
interpret(val, len)
char *val;
{
    register struct rpcdata *d = _rpcdata();
    char *p;
    register char *cp, **q;

    if (d == 0)
        return NULL;
    strncpy(d->line, val, len);
    p = d->line;
    d->line[len] = '\n';
    if (*p == '#')
        return (getrpcent());
    cp = index(p, '#');
    if (cp == NULL)
    {
        cp = index(p, '\n');
        if (cp == NULL)
            return (getrpcent());
    }
    *cp = '\0';
    cp = index(p, ' ');
    if (cp == NULL)
    {
        cp = index(p, '\t');
        if (cp == NULL)
            return (getrpcent());
    }
    *cp++ = '\0';
    /* THIS STUFF IS INTERNET SPECIFIC */
    d->rpc.r_name = d->line;
    while (*cp == ' ' || *cp == '\t')
        cp++;
    d->rpc.r_number = atoi(cp);
    q = d->rpc.r_aliases = d->rpc_aliases;
    cp = index(p, ' ');
    if (cp != NULL)
        *cp++ = '\0';
    else
    {
        cp = index(p, '\t');
        if (cp != NULL)
            *cp++ = '\0';
    }
    while (cp && *cp)
    {
        if (*cp == ' ' || *cp == '\t')
        {
            cp++;
            continue;
        }
        if (q < & (d->rpc_aliases[MAXALIASES - 1]))
            *q++ = cp;
        cp = index(p, ' ');
        if (cp != NULL)
            *cp++ = '\0';
        else
        {
            cp = index(p, '\t');
            if (cp != NULL)
                *cp++ = '\0';
        }
    }
    *q = NULL;
    return (&d->rpc);
}

static struct rpcdata *
_rpcdata()
{
    register struct rpcdata *d = rpcdata;

#ifdef WIN32
    char *str;

    if ((RPCDB[0] == '\0') && (str = getenv("SystemRoot")))
    {
        strcpy(RPCDB, str);
        strcat(RPCDB, "\\system32\\drivers\\etc\\rpc");
    }
#endif

    if (d == 0)
    {
        d = (struct rpcdata *)calloc(1, sizeof (struct rpcdata));
        rpcdata = d;
    }
    return (d);
}

struct rpcent *
getrpcbynumber(number)
register int number;
{
    register struct rpcdata *d = _rpcdata();
    register struct rpcent *p;
    char *val = NULL;

    if (d == 0)
        return (0);
    setrpcent(0);
    while (p = getrpcent())
    {
        if (p->r_number == number)
            break;
    }
    endrpcent();
    return (p);
}


