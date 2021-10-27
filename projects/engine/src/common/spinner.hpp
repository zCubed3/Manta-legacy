#ifndef MANTA_SPINNER_H
#define MANTA_SPINNER_H

// Does the spinny thing in the console, idk what else to say
class Spinner {
public:
    char spinChars[4] = {'|', '/', '-', '\\'};
    int spinIndex = 0;

    char character = ' ';

    void Spin() {
        if (spinIndex++ >= 4)
            spinIndex = 0;

        character = spinChars[spinIndex];
    }
};

#endif
