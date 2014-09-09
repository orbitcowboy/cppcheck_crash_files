

IMPLEMENT_ABSTRACT_CLASS(mpLayer, wxObject)

mpLayer::mpLayer()
    : m_type(mpLAYER_UNDEF)
{
    SetPen( *wxBLACK_PEN);
    SetFont( *wxNORMAL_FONT);
    m_pointType  = mpPOINT;
    m_continuous = FALSE; // Default
    m_showName   = TRUE;  // Default
    m_drawOutsideMargins = TRUE;
    m_visible = true;
    m_RectSize = wxSize(6, 6);
}

wxBitmap mpLayer::GetColourSquare(int side)
{
    wxBitmap square(side, side, -1);
    wxColour filler = m_pen.GetColour();
    wxBrush brush(filler, wxSOLID);
    wxMemoryDC dc;
    dc.SelectObject(square);
    dc.SetBackground(brush);
    dc.Clear();
    dc.SelectObject(wxNullBitmap);
    return square;
}

void mpFXY::Plot(wxDC & dc, mpWindow & w)
{
    wxCoord ix = 0, iy = 0;

    if (!m_name.IsEmpty() && m_showName)
    {
        wxCoord tx, ty;
        dc.GetTextExtent(m_name, &tx, &ty);

        // xxx implement else ... if (!HasBBox())
        {
            // const int sx = w.GetScrX();
            // const int sy = w.GetScrY();

        }
    }
}

