class  vtkCellLabelAnnotation
{
        int Extent[6];
        vtkSetVector6Macro(Extent, int);
        void ComputeLabelPositions()
        {
                if (this->Extent[0])
                {
                        return;
                }
        }
};
void RenderOpaqueGeometry()
{
}
// from https://trac.cppcheck.net/ticket/8842
