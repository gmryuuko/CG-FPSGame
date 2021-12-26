#include "Audio.h"

namespace Audio {
    void Shoot(int gun) {
        switch (gun)
        {
            case 1:
                // mciSendString("close shoot1", NULL, 0, NULL);
                // mciSendString("open ../resources/audio/shoot1.wav alias shoot1", NULL, 0, NULL);
                // mciSendString("play ../resources/audio/shoot1.wav", NULL, 0, NULL);
                PlaySound("../resources/audio/shoot1.wav", NULL, SND_FILENAME | SND_ASYNC);
                break;
            case 2:
                // mciSendString("close shoot2", NULL, 0, NULL);
                // mciSendString("open ../resources/audio/shoot2.wav alias shoot2", NULL, 0, NULL);
                // mciSendString("play shoot2", NULL, 0, NULL);
                PlaySound("../resources/audio/shoot2.wav", NULL, SND_FILENAME | SND_ASYNC);
                break;
        }
    }
}