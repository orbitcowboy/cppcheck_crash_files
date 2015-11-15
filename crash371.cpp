typedef const struct mode_param
{
	int color;
	int depth;
	int scanmode;
} ModeParam, *pModeParam;


static pModeParam getModeList( Plustek_Scanner *scanner )
{
	pModeParam mp;

	if( 0 != scanner->val[OPT_EXT_MODE].w ) {
		mp = &mp[_TPAModeSupportMin];
	}

	return mp;
}
