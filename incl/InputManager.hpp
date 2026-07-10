#ifndef InpMGR_H
#define InpMGR_H

enum KeyState {
    RELEASED,
    PRESSED
    // HOLD, DOUBLE 등 추가 가능
};

struct InputEv{
    int mouseX;
    int mouseY;

    bool mouseLButtonPressed;
    bool mouseRButtonPressed;

    KeyState keyStateArr[256];
};

extern InputEv inputEV;

#endif /* InpMGR_H */