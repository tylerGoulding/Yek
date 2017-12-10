

#ifndef FPS_C_h
#define FPS_C_h


#ifdef __cplusplus
extern "C" {
#endif

    
typedef struct FPS_GT511C3 FPS_GT511C3;
void fps_New(void);
void fps_Open(void);
void fps_SetLED(bool on);
bool fps_IsPressFinger(void);
bool fps_CaptureFinger(bool highquality);
int fps_Identify1_N(void);
bool fps_DeleteAll(void);
bool fps_CheckEnrolled(int id);
int fps_Enroll1(void);
int fps_Enroll2(void);
int fps_Enroll3(void);
int fps_EnrollStart(int enrollid);


#ifdef __cplusplus
}
#endif


#endif