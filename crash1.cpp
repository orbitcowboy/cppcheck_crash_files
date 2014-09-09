
BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_LEFT_UP (MyCanvas::OnMouseUp)
END_EVENT_TABLE()

void MyCanvas::OnMouseUp()
{
    ReleaseMouse();
    {
    }
}

// https://sourceforge.net/apps/trac/cppcheck/ticket/3761

// $ cppcheck Crash1.cpp
//Checking Crash1.cpp...
//Speicherzugriffsfehler



