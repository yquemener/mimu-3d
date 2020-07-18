#ifndef MUMBLE_LINK_H
#define MUMBLE_LINK_H
#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdint.h>
    #include <stddef.h>
#endif // _WIN32


struct LinkedMem {
#ifdef _WIN32
    UINT32	uiVersion;
    DWORD	uiTick;
#else
    uint32_t uiVersion;
    uint32_t uiTick;
#endif
    float	fAvatarPosition[3];
    float	fAvatarFront[3];
    float	fAvatarTop[3];
    wchar_t	name[256];
    float	fCameraPosition[3];
    float	fCameraFront[3];
    float	fCameraTop[3];
    wchar_t	identity[256];
#ifdef _WIN32
    UINT32	context_len;
#else
    uint32_t context_len;
#endif
    unsigned char context[256];
    wchar_t description[2048];
};

class MumbleLink{
private:
    LinkedMem* lm;

public:
    MumbleLink();

    void updateMumble(const float pos_x, const float pos_y, const float pos_z,
                      const float at_x, const float at_y, const float at_z,
                      const float up_x, const float up_y, const float up_z);
};

#endif // MUMBLE_LINK_H
