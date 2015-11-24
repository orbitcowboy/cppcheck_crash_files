class ComponentDC{
    ::Component * getComponent();
};

::Component * ComponentDC::getComponent() {
    return ((::Component *)myComponent);
}

class  MultiComponentDC : public ComponentDC {
    virtual void addChild(InterfaceNode *);
};

void MultiComponentDC::addChild(InterfaceNode *childNode) {
    ComponentDC *cdc = dynamic_cast<ComponentDC *>(childNode);
    if (cdc) {
        ::Component *c = cdc->getComponent();
    }
}
