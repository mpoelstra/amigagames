#ifndef SPARKPAW_CONTROL_OPTIONS_H
#define SPARKPAW_CONTROL_OPTIONS_H

enum SecondaryButtonAction {
    SECONDARY_BUTTON_JUMP,
    SECONDARY_BUTTON_FIRE
};

static int secondaryButtonAddsJump(enum SecondaryButtonAction action,
                                   int held)
{
    return action==SECONDARY_BUTTON_JUMP&&held;
}

static int secondaryButtonAddsFire(enum SecondaryButtonAction action,
                                   int held)
{
    return action==SECONDARY_BUTTON_FIRE&&held;
}

#endif
