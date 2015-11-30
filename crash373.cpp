IAsyncOperation<IVectorView<int>^>^ OpenCVLib::ProcessAsync(IVector<int>^ input, int width, int height)
{
    return create_async([=]() -> IVectorView<int>^
    {});
}
