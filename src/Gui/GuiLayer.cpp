#include "GuiLayer.h"

void GuiLayer::show(bool show)
{
    container->setVisible(show);
}

void GuiLayer::enable(bool enable)
{
    container->setEnabled(enable);
}

const bool GuiLayer::isShown()
{
    return container->isVisible();
}

const bool GuiLayer::isEnabled()
{
    return container->isEnabled();
}
