#include "mumble_link.h"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdio.h>
    #include <wchar.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/mman.h>
    #include <fcntl.h> /* For O_* constants */
#endif // _WIN32


MumbleLink::MumbleLink()
{
#ifdef _WIN32
    HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
    if (hMapObject == NULL)
        return;

lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
    if (lm == NULL) {
        CloseHandle(hMapObject);
        hMapObject = NULL;
        return;
    }
#else
    char memname[256];
    snprintf(memname, 256, "/MumbleLink.%d", getuid());

    int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmfd < 0) {
        return;
    }

    this->lm = (LinkedMem *)(mmap(nullptr, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

    if (this->lm == (void *)(-1)) {
        this->lm = nullptr;
        return;
    }
#endif
}


void MumbleLink::updateMumble(const float pos_x, const float pos_y, const float pos_z,
                              const float at_x, const float at_y, const float at_z,
                              const float up_x, const float up_y, const float up_z)
{
    if (! this->lm)
        return;

    if(this->lm->uiVersion != 2) {
        wcsncpy(this->lm->name, L"TestLink", 256);
        wcsncpy(this->lm->description, L"TestLink is a test of the Link plugin.", 2048);
        this->lm->uiVersion = 2;
    }
    this->lm->uiTick++;

    // Left handed coordinate system.
    // X positive towards "right".
    // Y positive towards "up".
    // Z positive towards "front".
    //
    // 1 unit = 1 meter

    // Unit vector pointing out of the avatar's eyes aka "At"-vector.
    this->lm->fAvatarFront[0] = at_x;
    this->lm->fAvatarFront[1] = at_y;
    this->lm->fAvatarFront[2] = at_z;

    // Unit vector pointing out of the top of the avatar's head aka "Up"-vector (here Top points straight up).
    this->lm->fAvatarTop[0] = up_x;
    this->lm->fAvatarTop[1] = up_y;
    this->lm->fAvatarTop[2] = up_z;

    // Position of the avatar (here standing slightly off the origin)
    this->lm->fAvatarPosition[0] = pos_x/10.0;
    this->lm->fAvatarPosition[1] = pos_y/10.0;
    this->lm->fAvatarPosition[2] = pos_z/10.0;

    // Same as avatar but for the camera.
    this->lm->fCameraPosition[0] = pos_x/10.0;
    this->lm->fCameraPosition[1] = pos_y/10.0;
    this->lm->fCameraPosition[2] = pos_z/10.0;

    this->lm->fCameraFront[0] = at_x;
    this->lm->fCameraFront[1] = at_y;
    this->lm->fCameraFront[2] = at_z;

    this->lm->fCameraTop[0] = up_x;
    this->lm->fCameraTop[1] = up_y;
    this->lm->fCameraTop[2] = up_z;

    // Identifier which uniquely identifies a certain player in a context (e.g. the ingame name).
    wcsncpy(this->lm->identity, L"Yves", 256);
    // Context should be equal for players which should be able to hear each other positional and
    // differ for those who shouldn't (e.g. it could contain the server+port and team)

    memcpy(this->lm->context, "Minetest\0", 9);
    this->lm->context_len = 9;
}
