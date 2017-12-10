

#include "fps_c.h"
#include "FPS_GT511C3.h"


#ifdef __cplusplus
extern "C" {
#endif



static FPS_GT511C3 *fps = NULL;


void fps_New() {
    if (fps == NULL) {
        // throw it on the stack we dont need heap runtime issues
        fps = new FPS_GT511C3();
    }
}
void fps_Open() {
    fps->Open();
}

void fps_SetLED(bool on){
    fps->SetLED(on);
}
bool fps_IsPressFinger(){
    return (bool) fps->IsPressFinger();
}
bool fps_CaptureFinger(bool highquality){
    return fps->CaptureFinger(highquality);
}
int fps_Identify1_N(){
    return fps->Identify1_N();
}
bool fps_DeleteAll(){
    return fps->DeleteAll();
}

int fps_EnrollStart(int enrollid){
    return fps->EnrollStart(enrollid);
}

bool fps_CheckEnrolled(int id){
    return fps->CheckEnrolled(id);
}
int fps_Enroll1(){
    return fps->Enroll1();
}
int fps_Enroll2(){
    return fps->Enroll2();
}
int fps_Enroll3(){
    return fps->Enroll3();  
}

#ifdef __cplusplus
}
#endif
