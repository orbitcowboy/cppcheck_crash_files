sal_Int32 impl_Export() {
    try {
            try  {
                uno::Sequence< uno::Any > aArgs(2);
                beans::NamedValue aValue;
                aValue.Name = "DocumentHandler";
                aValue.Value <<= xDocHandler;
                aArgs[0] <<= aValue;
                aValue.Name = "Model";
                aValue.Value <<= xDocumentComp;
                aArgs[1] <<= aValue;
            }
            catch (const uno::Exception&) {
            }
    }
    catch (const uno::Exception&)  {
    }
}
